// hidcomp/hidconfig, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "common.h"

#include "hiddevice.h"
#include "hidparser.h"
#include <libusb-1.0/libusb.h>
#include <poll.h>
#include <unistd.h>

#include "hidtypes.h"
#include "log.h"

#include <assert.h>

#define TIMEOUT_ERROR	LIBUSB_ERROR_TIMEOUT

// This is crappy - one global flag for all threads.
// This was added to stop scribbles on shutdown (from usb callbacks)
volatile bool HIDDeviceThread::m_bRunning = false;

HIDDeviceThread::HIDDeviceThread( QMutex &SendBufferMutex, QList<QVector<byte> > &SendBuffer, QMutex &ReceiveBufferMutex, QList<QVector<byte> > &ReceiveBuffer, int nLongestInReport, int nLongestOutReport, HIDDevice *pDevice )
: m_SendBufferMutex( SendBufferMutex )
, m_SendBuffer( SendBuffer )
, m_ReceiveBufferMutex( ReceiveBufferMutex )
, m_ReceiveBuffer( ReceiveBuffer )
, m_nLongestInReport( nLongestInReport )
, m_nLongestOutReport( nLongestOutReport )
, m_pDevice( pDevice )
, m_Logger( QCoreApplication::applicationName(), "HIDDeviceThread" )
, m_nReadPipe(0)
, m_nWritePipe(0)
{
    m_bRunning = false;
}

HIDDeviceThread::~HIDDeviceThread(void)
{
}

void HIDDeviceThread::_ReadCallback(struct libusb_transfer *transfer)
{
    if ( m_bRunning )
    {
        HIDDeviceThread *pThis = static_cast<HIDDeviceThread *>( transfer->user_data );
        pThis->ReadCallback( transfer );
    }
}

void HIDDeviceThread::ReadCallback(struct libusb_transfer *transfer)
{
    LOG_MSG( m_Logger, LogTypes::Debug, QString("Got callback status=%1, transfered=%2").arg(transfer->status).arg(transfer->actual_length) );

    if ( transfer->status == LIBUSB_TRANSFER_COMPLETED ||
	 transfer->status == LIBUSB_TRANSFER_TIMED_OUT )
    {
	if ( transfer->actual_length > 0 )
	{
	    QVector<byte> buf(transfer->actual_length);
	    memcpy( buf.data(), transfer->buffer, transfer->actual_length );

	    QMutexLocker lock(&m_ReceiveBufferMutex);
	    m_ReceiveBuffer.push_back( buf );
	}    
    }

    // resubmit
    int n = libusb_submit_transfer( transfer );
    if ( n != 0 )
	LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to resubmit transfer packet %1").arg(n) );
}


void HIDDeviceThread::_WriteCallback(struct libusb_transfer *transfer)
{
    if ( m_bRunning )
    {
        HIDDeviceThread *pThis = static_cast<HIDDeviceThread *>( transfer->user_data );
        pThis->WriteCallback( transfer );
    }
}

void HIDDeviceThread::WriteCallback(struct libusb_transfer *transfer)
{
    LOG_MSG( m_Logger, LogTypes::Debug, QString("Got write callback status=%1, transfered=%2").arg(transfer->status).arg(transfer->actual_length) );
    // todo clean up
    m_bSendBufferFree = true;
}

void HIDDeviceThread::FDRemoved(int, void *user_data)
{
    if ( m_bRunning )
    {
        HIDDeviceThread *pThis = reinterpret_cast<HIDDeviceThread *>( user_data );
        pThis->m_bRebuild_fds = true;
    }
}

void HIDDeviceThread::FDAdded(int, short, void *user_data)
{
    if ( m_bRunning )
    {
        HIDDeviceThread *pThis = reinterpret_cast<HIDDeviceThread *>( user_data );
        pThis->m_bRebuild_fds = true;
    }
}



void HIDDeviceThread::run()
{
    LOG_MSG( m_Logger, LogTypes::Debug, "Starting HID device thread" );

    m_bRunning = true;

    try
    {
	// Queue an interrupt read
	for ( int i = 0; i < 3; i++ )
	{
	    LOG_MSG( m_Logger, LogTypes::Debug, "Creating transfer buffer" );

	    libusb_transfer *pTransfer = libusb_alloc_transfer(0);
	    if ( pTransfer == NULL )
	    {
		LOG_MSG( m_Logger, LogTypes::Error, "Failed to allocate interrupt transfer packet" );
		return;
	    }
	    else
	    {
		m_TransferBuffers.push_back( pTransfer );
                byte Endpoint, TransferType;
                if ( !m_pDevice->GetInputEndpoint(Endpoint, TransferType) ||
                     TransferType == LIBUSB_TRANSFER_TYPE_CONTROL ||
                     TransferType == LIBUSB_TRANSFER_TYPE_ISOCHRONOUS )
                {
                    LOG_MSG( m_Logger, LogTypes::Error, "Only support bulk and interrupt read transfer types" );
                    return;
                }
                else
                {
                    if ( TransferType == LIBUSB_TRANSFER_TYPE_BULK )
                    {
                        libusb_fill_bulk_transfer( pTransfer, m_pDevice->m_hDev, Endpoint, new byte[m_nLongestInReport], m_nLongestInReport, &_ReadCallback, this, 0x7FFFFFFF );
                    }
                    else // TransferType == LIBUSB_TRANSFER_TYPE_INTERRUPT
                    {
                        libusb_fill_interrupt_transfer( pTransfer, m_pDevice->m_hDev, Endpoint, new byte[m_nLongestInReport], m_nLongestInReport, &_ReadCallback, this, 0x7FFFFFFF );
                    }

                    int n = libusb_submit_transfer( pTransfer );
                    if ( n != 0 )
                    {
                        LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to submit transfer packet %1" ).arg(n) );
                        return;
                    }
                }
	    }
	}

	// We have one send packet that gets sent in the main loop.
	libusb_transfer *pSendTransfer = libusb_alloc_transfer(0);
        int send_buffer_offset = 0;
	if ( pSendTransfer == NULL )
	{
            LOG_MSG( m_Logger, LogTypes::Error, "Failed to allocate write packet" );
	    return;
	}
	else
	{
	    m_TransferBuffers.push_back( pSendTransfer );

            byte Endpoint, TransferType;
            if ( !m_pDevice->GetOutputEndpoint(Endpoint, TransferType) || TransferType == LIBUSB_TRANSFER_TYPE_CONTROL )
            {
                byte *send_buf = new byte[m_nLongestOutReport + 8];
                send_buffer_offset = 8;

                libusb_fill_control_setup( send_buf, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                           SET_REPORT, 0x0200, m_pDevice->InterfaceNumber(), m_nLongestOutReport );
                libusb_fill_control_transfer( pSendTransfer, m_pDevice->m_hDev, send_buf, &_WriteCallback, this, 1000 );
            }
            else if ( TransferType == LIBUSB_TRANSFER_TYPE_BULK )
            {
                byte *send_buf = new byte[m_nLongestOutReport];
                libusb_fill_bulk_transfer( pSendTransfer, m_pDevice->m_hDev, Endpoint, send_buf, m_nLongestOutReport, &_WriteCallback, this, 1000 );
            }
            else if ( TransferType == LIBUSB_TRANSFER_TYPE_INTERRUPT )
            {
                byte *send_buf = new byte[m_nLongestOutReport];
                libusb_fill_interrupt_transfer( pSendTransfer, m_pDevice->m_hDev, Endpoint, send_buf, m_nLongestOutReport, &_WriteCallback, this, 1000 );
            }

	}
	m_bSendBufferFree = true;

	int pipe_fds[2];
	int n = pipe( pipe_fds );
	if ( n != 0 )
	{
	    LOG_MSG( m_Logger, LogTypes::Error, "Failed to create signal pipe" );
	    return;
	}
	m_nReadPipe = pipe_fds[0];
	m_nWritePipe = pipe_fds[1];

	libusb_set_pollfd_notifiers( NULL, &FDAdded,  &FDRemoved, this );

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 50000;
	fd_set fds_read;
	fd_set fds_write;
        int nFds = 0;
	m_bRebuild_fds = true;

	struct timeval tv_zero = {0,0};
        while ( m_bRunning )
        {
	    //LOG_MSG( m_Logger, LogTypes::Debug, "Tick" );
	    //libusb_handle_events_timeout(NULL, &tv);

	    if ( m_bRebuild_fds )
	    {
		m_bRebuild_fds = false;

		FD_ZERO(&fds_read);
		FD_ZERO(&fds_write);

		FD_SET( m_nReadPipe, &fds_read );
		nFds = m_nReadPipe;

		const libusb_pollfd **pFds = libusb_get_pollfds( NULL );
		const libusb_pollfd **ptr = pFds;
		while ( *ptr != NULL )
		{
		    if ( (*ptr)->events & POLLIN )
		    {
			FD_SET( (*ptr)->fd, &fds_read );
			LOG_MSG( m_Logger, LogTypes::Debug, QString("Watching libusb read fd %1").arg((*ptr)->fd) );
		    }
		    if ( (*ptr)->events & POLLOUT )
		    {
			FD_SET( (*ptr)->fd, &fds_write );
			LOG_MSG( m_Logger, LogTypes::Debug, QString("Watching libusb write fd %1").arg((*ptr)->fd) );
		    }
		    if ( (*ptr)->fd > nFds )
			nFds = (*ptr)->fd;
		    ptr++;
		}
		free( pFds );
		nFds++;
	    }

	    struct timeval tv;
	    n = libusb_get_next_timeout( NULL, &tv );
        if ( n == 0 )
        { 
            tv.tv_sec = 0;
            tv.tv_usec = 20000; // default to 20ms if usb has nothing to do.
        }
	    fd_set rd = fds_read;
	    fd_set wr = fds_write;
	    LOG_MSG( m_Logger, LogTypes::Debug, QString("Select will wait for %1.%2").arg(tv.tv_sec).arg(tv.tv_usec, 6, 10, QChar('0')) );

	    n = select( nFds, &rd, &wr, NULL, &tv );
	    LOG_MSG( m_Logger, LogTypes::Debug, QString("Select returned %1").arg(n) );
	    
	    if ( m_bRunning )
		libusb_handle_events_timeout(NULL, &tv_zero);


	    if ( m_bRunning && m_bSendBufferFree && !m_SendBuffer.isEmpty() )
	    {
		QVector<byte> msg;

		{
		    QMutexLocker lock(&m_SendBufferMutex);
		    if ( !m_SendBuffer.isEmpty() )
			msg = m_SendBuffer.takeFirst();
		}

                if ( m_Logger.WillLog( LogTypes::Debug ) )
                {
	            QString  s = QString("Sending %1 bytes - ").arg(msg.count());
	            for ( int i = 0; i < msg.count(); i++ )
			s += QString("%1 ").arg(msg[i], 2, 16, QChar('0') );
	            LOG_MSG( m_Logger, LogTypes::Debug, s );
                }

		assert( msg.count() <= m_nLongestOutReport );
		if ( msg.count() > m_nLongestOutReport )
		{
	            LOG_MSG( m_Logger, LogTypes::Error, QString("Trying to send a message longer than the longest output report %1>%2").arg(msg.count()).arg(m_nLongestOutReport) );
		}
		else
		{
                    memcpy( pSendTransfer->buffer + send_buffer_offset, msg.data(), msg.count() );
                    pSendTransfer->length = msg.count()+send_buffer_offset;
//                    {
//                        QString  s = QString("Buffer - ").arg(msg.count()+send_buffer_offset);
//                        for ( int i = 0; i < msg.count() + send_buffer_offset; i++ )
//                            s += QString("%1 ").arg(pSendTransfer->buffer[i], 2, 16, QChar('0') );
//                        LOG_MSG( m_Logger, LogTypes::Debug, s );
//                    }

		    m_bSendBufferFree = false;
		    int n = libusb_submit_transfer( pSendTransfer );
		    if ( n != 0 )
			LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to submit transfer packet %1" ).arg(n) );
		}
	    }

	    if ( m_bRunning && FD_ISSET( m_nReadPipe, &rd ) )
	    {
                LOG_MSG( m_Logger, LogTypes::Debug, "Read pipe signalled"  );

                char buf;
                read( m_nReadPipe, &buf, 1 );   // Swallow the signal byte
	    }

	    if ( m_bRunning && m_Logger.WillLog( LogTypes::Debug ) )
	    {
		const libusb_pollfd **pFds = libusb_get_pollfds( NULL );
		const libusb_pollfd **ptr = pFds;
		while ( *ptr != NULL )
		{
		    if ( (*ptr)->events & POLLIN )
		    {
			if ( FD_ISSET( (*ptr)->fd, &rd ) )
			    LOG_MSG( m_Logger, LogTypes::Debug, QString("read fd %1 triggered").arg((*ptr)->fd) );
		    }
		    if ( (*ptr)->events & POLLOUT )
		    {
			if ( FD_ISSET( (*ptr)->fd, &wr ) )
			    LOG_MSG( m_Logger, LogTypes::Debug, QString("write fd %1 triggered").arg((*ptr)->fd) );
		    }
		    ptr++;
		}
		free( pFds );
	    }
        }

	// Clean up
	for ( int i = 0; i < m_TransferBuffers.count(); i++ )
	{
	    // libusb_cancel_transfer( m_TransferBuffers[i] ); - I don't think this is needed, as we never call libusb_handle_events again
	    //delete m_TransferBuffers[i]->buffer;
	    //libusb_free_transfer( m_TransferBuffers[i] );
	}
	m_TransferBuffers.clear();
    }
    catch (...)
    {
    }

    m_bRunning = false;
}


void HIDDeviceThread::Signal()
{
    char buf=0;
    write( m_nWritePipe, &buf, 1 );
}


void HIDDeviceThread::Stop()
{
    m_bRunning = false;
    Signal();
}

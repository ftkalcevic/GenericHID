#include "stdafx.h"
#include "programdlg.h"
#include "usb.h"

const int DEVICE_POLL_PERIOD = 500;	// ms

const unsigned short GenericHID_VID = 0x1c40;
const unsigned short GenericHID_PID = 0x04d9;

const unsigned short At90USB1287_VID = 0x03eb;
const unsigned short At90USB1287_PID = 0x2FFB;

ProgramDlg::ProgramDlg(QWidget *parent)
: QDialog(parent)
, m_Logger( QCoreApplication::applicationName(), "ProgramDlg" )
, m_bMultipleWarning( false )
{
    ui.setupUi(this);

    SetMode( false, false, false );

    // timer
    m_timer.setSingleShot( true );
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateDeviceStatus()));
    m_timer.start( 0 );	// first time expires immediately (that is, after the window pops up)
}

ProgramDlg::~ProgramDlg()
{

}

void ProgramDlg::SetMode( bool bDevice, bool bBootloader, bool bHID)
{
    ui.lightUSBDeviceLocated->setPixmap( bDevice ? QPixmap(":/GenericHID/greenlight") : QPixmap(":/GenericHID/redlight") );
    ui.lightBootloaderMode->setPixmap( bBootloader ? QPixmap(":/GenericHID/greenlight") : QPixmap(":/GenericHID/redlight") );
    ui.lightHIDDeviceMode->setPixmap( bHID ? QPixmap(":/GenericHID/greenlight") : QPixmap(":/GenericHID/redlight") );

    ui.btnStartBootloader->setEnabled( bHID );
    ui.btnProgram->setEnabled( bBootloader );
    ui.btnRestartDevice->setEnabled( bBootloader );
}

bool ProgramDlg::FindDevices( int &nGenericHIDs, int &nAt90DFUs )
{
    nGenericHIDs = 0;
    nAt90DFUs = 0;

    usb_find_busses();
    usb_find_devices();

    /* Walk the tree and find our devices. */
    for( struct usb_bus *usb_bus = usb_get_busses(); NULL != usb_bus; usb_bus = usb_bus->next ) 
    {
        for( struct usb_device *device = usb_bus->devices; NULL != device; device = device->next) 
	{
	    LOG_MSG( m_Logger, LogTypes::Error, QString("VID=%1 PID=%2").arg(device->descriptor.idVendor,4,16,QChar('0')).arg(device->descriptor.idProduct,4,16,QChar('0')) );
            if( device->descriptor.idVendor == GenericHID_VID && device->descriptor.idProduct == GenericHID_PID )
		nGenericHIDs++;
            else if( device->descriptor.idVendor == At90USB1287_VID && device->descriptor.idProduct == At90USB1287_PID )
		nAt90DFUs++;
        }
    }
    return true;
}

void ProgramDlg::updateDeviceStatus()
{
    // Check for USB devices on the BUS.
    int nGenericHIDs = 0;
    int nAt90DFUs = 0;
    if ( FindDevices( nGenericHIDs, nAt90DFUs ) )
    {
	// check for too many devices
	if ( nGenericHIDs + nAt90DFUs > 1 )
	{
	    //
	    SetMode( false, false, false );
	    if ( !m_bMultipleWarning )
	    {
		m_bMultipleWarning = true;
		QMessageBox::warning( this, "Multiple Devices Found", "More than one GenericHID device (GenericHID or AT90USB128 in DFU mode) found in the system.  Only one can be present during programming." );
	    }
	}
	else
	{
	    m_bMultipleWarning = false;
	    if ( nGenericHIDs > 0 )
	    {
		SetMode( true, false, true );
	    }
	    else if ( nAt90DFUs > 0 )
	    {
		SetMode( true, true, false );
	    }
	    else
	    {
		SetMode( false, false, false );
	    }
	}
    }

    m_timer.start( DEVICE_POLL_PERIOD );
}

void ProgramDlg::onStartBootloader()
{
}

void ProgramDlg::onProgram()
{
}

void ProgramDlg::onRestartDevice()
{
}

void ProgramDlg::onClose()
{
}

void ProgramDlg::onRefresh()
{
}

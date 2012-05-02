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


#ifndef _HIDDEVICETHREAD_H_
#define _HIDDEVICETHREAD_H_

#include <QString>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>

#include "hidparser.h"
#include "hiddevice.h"

class HIDDevice;

class HIDDeviceThread: public QThread
{
public:
    HIDDeviceThread( QMutex &SendBufferMutex, QList<QVector<byte> > &SendBuffer, QMutex &ReceiveBufferMutex, QList<QVector<byte> > &ReceiveBuffer, int nLongestInReport, int nLongestOutReport, HIDDevice *pDevice );
    ~HIDDeviceThread(void);

    void Signal();
    void Stop();

private:
    virtual void run();
    void ReadCallback(struct libusb_transfer *transfer);
    static void _ReadCallback(struct libusb_transfer *transfer);
    static void FDRemoved(int fd, void *user_data);
    static void FDAdded(int fd, short events, void *user_data);

    QMutex &m_SendBufferMutex;
    QList<QVector<byte> > &m_SendBuffer;
    QMutex &m_ReceiveBufferMutex;
    QList<QVector<byte> > &m_ReceiveBuffer;
    volatile bool m_bRunning;
    int m_nLongestInReport;
    int m_nLongestOutReport;
    HIDDevice *m_pDevice;
    Logger m_Logger;
    static void _WriteCallback(struct libusb_transfer *transfer);
    void WriteCallback(struct libusb_transfer *transfer);
    bool m_bSendBufferFree;
    bool m_bRebuild_fds;
    int m_nReadPipe;
    int m_nWritePipe;

    QList<struct libusb_transfer *> m_TransferBuffers;
};

#endif

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


#ifndef _HIDDEVICE_H_
#define _HIDDEVICE_H_

#include "hidparser.h"

#ifdef _WIN32
#pragma warning(push, 1)
#endif

#include <QString>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>

#ifdef _WIN32
#pragma warning(pop)
#pragma warning(disable:4251)
#endif

#ifndef _WIN32
    #define HAS_ASYNC
    #include "hiddevicethread.h"
#endif

#define USB_TIMEOUT 1000		// default 1sec timeout

class HIDDevice: protected QThread
{
public:
#ifdef _WIN32
    HIDDevice(struct usb_device *dev, byte nInterface, byte nConfig);
#else
    HIDDevice(struct libusb_device *dev, byte nInterface, byte nConfig);
#endif
    ~HIDDevice(void);

public:
    bool Open();
    bool Close();
    bool Claim();
    bool Unclaim();

    bool hasOpenErrors() { return m_bOpenErrors; }
    QString Manufacturer();
    QString Product();
    QString SerialNumber();
    QString SystemId();
    unsigned short VID();
    unsigned short PID();
    HID_ReportInfo_t &ReportInfo() { return m_ReportInfo; }
    bool PreprocessReportData();
    QString GetDescriptorString( unsigned char index );
    int InterruptRead( byte *buf, int len, int timeout = USB_TIMEOUT );
    int InterruptWrite( const byte *buf, int len, int timeout = USB_TIMEOUT );
    bool GetReport( byte nReportId, HID_ReportItemTypes_t nReportType, byte *buf, int len, int timeout = USB_TIMEOUT );
    int GetConfiguration();
    byte OutputEndpoint();
    byte InputEndpoint();
    byte GetEndpoint( byte nDirection, byte nType );
    HID_ReportItem_t *FindReportItem( HID_ReportItemTypes_t type, unsigned short nUsagePage, unsigned short nUsage );

#ifdef HAS_ASYNC
    bool StartAsync();
    void StopAsync();
    bool RequestReport( byte nReportId, HID_ReportItemTypes_t nReportType, int nBufLen, int timeout = USB_TIMEOUT );
    int AsyncInterruptWrite( const byte *buf, int len  );
    int AsyncInterruptRead( byte *buf, int len );
#endif

private:
    QMutex m_SendBufferMutex;
    QList<QVector<byte> > m_SendBuffer;
    QMutex m_ReceiveBufferMutex;
    QList<QVector<byte> > m_ReceiveBuffer;
    Logger m_Logger;
#ifdef _WIN32
    struct usb_device *m_dev;
    struct usb_dev_handle *m_hDev;
#else
    struct libusb_device *m_dev;
    struct libusb_device_handle *m_hDev;
#endif
    int m_nInterfaceNumber;
    byte m_nInterface;
    byte m_nConfig;
    bool m_bOpen;
    bool m_bOpenErrors;
    int m_nClaimed;	// Use libusb return codes to represent claimed.  claimed=0, error!=0
    HID_ReportInfo_t m_ReportInfo;
    std::map<byte,QString> m_DescriptorStrings;
    QString m_sSystemId;
#ifdef HAS_ASYNC
    bool m_bRunning;
    bool m_bAsyncStarted;
    static void _WriteCallback(struct libusb_transfer *transfer);
    void WriteCallback(struct libusb_transfer *transfer);
    void StopThread();
    class HIDDeviceThread *m_pThread;
friend class HIDDeviceThread;
#endif
};

#endif

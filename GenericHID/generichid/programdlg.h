// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

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

#ifndef PROGRAMDLG_H
#define PROGRAMDLG_H

#include <QDialog>
#include "ui_programdlg.h"
#include "programmer.h"
#include "programmerthread.h"
#include "programmingstatusdlg.h"

struct USBDevice
{
    struct usb_dev_handle *hDevice;
    struct usb_device *pDevice;
    unsigned char nEndpoint;
};

class ProgramDlg : public QDialog
{
    Q_OBJECT

public:
    ProgramDlg(QWidget *parent = 0);
    ~ProgramDlg();
    void setEEPROM( const QString &sIntelFormat );
    void setFirmwareFile( const QString &sFilename );

public slots:
    void updateDeviceStatus();
    void onStartBootloader();
    void onProgram();
    void onRestartDevice();
    void onClose();
    void onRefresh();
    void onUpdateStatus( int status );
    void onCompletionStatus( int nPercentComplete );

private:
    Logger m_Logger;
    Ui::ProgramDlgClass ui;
    QTimer m_timer;
    bool m_bMultipleWarning;
    ProgrammingStatusDlg *m_pStatusDlg;
    QString m_sEeprom;
    QString m_sFirmwareFile;

    void SetMode( bool bDevice, bool bBootloader, bool bHID);
    bool FindDevices( int &nGenericHIDs, int &nAt90DFUs );
    USBDevice GetGenericHIDDevice();
};

#endif // PROGRAMDLG_H

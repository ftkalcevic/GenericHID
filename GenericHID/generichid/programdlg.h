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

    void SetMode( bool bDevice, bool bBootloader, bool bHID);
    bool FindDevices( int &nGenericHIDs, int &nAt90DFUs );
    USBDevice GetGenericHIDDevice();
};

#endif // PROGRAMDLG_H

#ifndef PROGRAMDLG_H
#define PROGRAMDLG_H

#include <QDialog>
#include "ui_programdlg.h"

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

private:
    Logger m_Logger;
    Ui::ProgramDlgClass ui;
    QTimer m_timer;
    bool m_bMultipleWarning;

    void SetMode( bool bDevice, bool bBootloader, bool bHID);
    bool FindDevices( int &nGenericHIDs, int &nAt90DFUs );
};

#endif // PROGRAMDLG_H

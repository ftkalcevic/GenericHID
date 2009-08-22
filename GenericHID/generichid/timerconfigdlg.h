#ifndef TIMERCONFIGDLG_H
#define TIMERCONFIGDLG_H

#include <QDialog>
#include "ui_timerconfigdlg.h"

class TimerConfigDlg : public QDialog
{
    Q_OBJECT

public:
    TimerConfigDlg(int nBits, QStringList &sPrescales, QWidget *parent = 0);
    ~TimerConfigDlg();
    void setValue( const QString &sTimerDetails );
    QString value() const;

public slots:
    void ComputeTimer();
    void AutoComputeTimer();
    void onShowAdvanced();

private:
    Ui::TimerConfigDlgClass ui;
    int m_nClockFrequency;
    int m_nBits;
    QStringList m_sPrescales;
};

#endif // TIMERCONFIGDLG_H

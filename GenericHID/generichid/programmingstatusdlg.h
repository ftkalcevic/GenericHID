#ifndef PROGRAMMINGSTATUSDLG_H
#define PROGRAMMINGSTATUSDLG_H

#include <QDialog>
#include "ui_programmingstatusdlg.h"

class ProgrammingStatusDlg : public QDialog
{
    Q_OBJECT

public:
    ProgrammingStatusDlg(QWidget *parent = 0);
    ~ProgrammingStatusDlg();

    void setPercentage( int n ) { ui.progressBar->setValue( n ); update(); }
    void setLabel( const QString &s ) { ui.label->setText( s ); update(); }

private:
    Ui::ProgrammingStatusDlgClass ui;
};

#endif // PROGRAMMINGSTATUSDLG_H

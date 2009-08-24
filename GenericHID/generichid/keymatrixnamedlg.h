#ifndef KEYMATRIXNAMEDLG_H
#define KEYMATRIXNAMEDLG_H

#include <QDialog>
#include "ui_keymatrixnamedlg.h"

class KeyMatrixNameDlg : public QDialog
{
    Q_OBJECT

public:
    KeyMatrixNameDlg(QWidget *parent = 0);
    ~KeyMatrixNameDlg();
    void setRC( int nRows, int nColumns );
    void setValue( const QString &sNames );
    QString value() const;

private:
    virtual void resizeEvent( QResizeEvent * event );
    virtual void showEvent( QShowEvent * event );

    Ui::KeyMatrixNameDlgClass ui;
    int m_nRows;
    int m_nColumns;
};

#endif // KEYMATRIXNAMEDLG_H



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



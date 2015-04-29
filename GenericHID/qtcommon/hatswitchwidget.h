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

#ifndef HATSWITCHWIDGET_H
#define HATSWITCHWIDGET_H

#include <QWidget>
#include "log.h"

class HatSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    HatSwitchWidget(QWidget *parent=NULL);
    ~HatSwitchWidget();

    void setValue( int nValue ) { m_nValue = nValue; update(); }

private:
    Logger m_Logger;
    QPixmap *m_pDot;
    QPixmap *m_pDotHighlight;
    QPixmap *m_pArrow;
    QPixmap *m_pArrowHighlight;
    int m_nValue;

    virtual void paintEvent( QPaintEvent * evnt );
};

#endif // HATSWITCHWIDGET_H

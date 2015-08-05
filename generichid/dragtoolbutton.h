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

#ifndef DRAGTOOLBUTTON_H
#define DRAGTOOLBUTTON_H

#include "shape.h"

class DragToolButton : public QToolButton
{
    Q_OBJECT

public:
    DragToolButton(QWidget *parent, const Shape *pShape);
    ~DragToolButton();

private:
    virtual void mousePressEvent(QMouseEvent *event);
    const Shape *m_pShape;
};

#endif // DRAGTOOLBUTTON_H

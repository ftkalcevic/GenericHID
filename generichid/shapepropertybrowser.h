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

#ifndef SHAPEPROPERTYBROWSER_H
#define SHAPEPROPERTYBROWSER_H

#include <QtTreePropertyBrowser>

class ShapePropertyBrowser : public QtTreePropertyBrowser
{
    Q_OBJECT

public:
    ShapePropertyBrowser(QWidget *parent);
    ~ShapePropertyBrowser();

signals:
    void itemDataChanged( QtBrowserItem * item );

private:
    virtual void itemChanged ( QtBrowserItem * item )
    {
	QtTreePropertyBrowser::itemChanged( item );
	emit itemDataChanged( item );
    }
};

#endif // SHAPEPROPERTYBROWSER_H

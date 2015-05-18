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

#ifndef SHAPEMIMEDATA_H
#define SHAPEMIMEDATA_H

#include "shape.h"

class ShapeMimeData : public QMimeData
{
    Q_OBJECT

public:
    ShapeMimeData( const Shape *);
    ~ShapeMimeData();
    const Shape *shape() const { return m_pShape; }

    virtual bool hasFormat( const QString & ) const { return false; }
    virtual QStringList formats () const { return QStringList(); }
    virtual QVariant retrieveData ( const QString & , QVariant::Type ) const { return QVariant(); }

private:
    const Shape *m_pShape;
};

#endif // SHAPEMIMEDATA_H

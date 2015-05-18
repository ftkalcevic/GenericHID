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

#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include "editor.h"
#include "shape.h"
#include "shapecollection.h"


class ShapeItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT	    // Need Q_OBJECT to support signals and slots

public:
    ShapeItem(const Shape *pShape, int id, Editor *pEditor, QGraphicsItem *parent=NULL);
    ~ShapeItem();
    
    enum { Type = UserType + UserTypes::ShapeItemType };
    int type() const { return Type; }
    const Shape *shapeData() const { return m_pShape; }
    QList<PinItem *> pins() { return m_pins; }
    bool mirror() const { return m_bMirror; }
    void setMirror( bool b ) { m_bMirror = b; DoTransform(); }
    double rotation() const { return m_dRotate; }
    void setRotation( double d ) { m_dRotate = d; while ( m_dRotate > 360.0 ) m_dRotate -= 360.0; DoTransform(); }
    QList<PropertyValue *> &values() { return m_values; }
    int id() const { return m_nId; }
    void WriteXML( QDomElement &node ) const;
    bool Verify( QString &sError ) const;
    void MakeDeviceXML( QDomElement &elem, int nCurrent, const QString &sPowerPin ) const;
    void MakeControlsXML( QDomElement &elem ) const;
    void PropertyChanged( QtBrowserItem * item );
    void populateProperties();
    void retrieveProperties();
    void wireAddedEvent( WireItem *pWire );

    QString GetPropertyValueString( const QString &sName, const QString &sDefault ) const;

    static ShapeItem *CreateFromXML( ShapeCollection *pCol, Editor *, QDomElement & );
    int current() const;

signals:
    void itemChange( QGraphicsItem *item, QGraphicsItem::GraphicsItemChange change, const QVariant & value );

private:
    const Shape *m_pShape;
    Editor *m_pEditor;
    double m_dRotate;
    bool m_bMirror;
    QList<PinItem *> m_pins;
    int m_nId;
    QList<PropertyValue *> m_values;

    void DoTransform();
    virtual QVariant itemChange( GraphicsItemChange change, const QVariant & value );
    void CreateProperties();
    bool CreateGraphics();
};

#endif // SHAPEITEM_H

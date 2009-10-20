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

#ifndef SHAPESCENE_H
#define SHAPESCENE_H

#include "editor.h"
#include "shape.h"
#include "shapeitem.h"

class ShapeScene : public QGraphicsScene
{
    Q_OBJECT

public:
    ShapeScene( Editor *pEditor, qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
    ~ShapeScene();
    
    inline void SetCursor( QCursor & cur ) { this->views()[0]->viewport()->setCursor( cur ); }
    bool CanAdd( const Shape *pShape, QString &sError );
    ShapeItem *CreateNewShape( const Shape *pShape, Editor *pEditor, QPointF pos );
    const QList<ShapeItem *> &shapeItems() const { return m_ShapeItems; }
    QString makeXML();
    bool loadXML( QDomDocument &doc, ShapeCollection *pCol, QString & );
    bool VerifyShapes( QString &sError ) const;
    QString MakeDeviceXML() const;
    bool isMouseDown() const { return m_bMouseDown; }

public slots:
    void onViewItemChanged( QGraphicsItem *item, QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    void clear();
signals:
    void statusChanged( const QString &sMsg );

private:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void keyPressEvent( QKeyEvent * keyEvent );

    void UpdateWires( ShapeItem *pItem );
    PinItem *PinUnderCursor( QPointF pos );
    ShapeItem *ShapeUnderCursor( QPointF pos );
    void RemoveWire( WireItem *pItem );
    void RemoveShape( ShapeItem *pItem );
    int GetUniqueShapeId();
    void SetStatusMsg( const QString &sMsg );

    Logger m_Logger;
    Editor *m_pEditor;
    QList<ShapeItem *> m_ShapeItems;
    QList<WireItem *> m_WireItems;
    bool m_bMouseDown;
};

#endif // SHAPESCENE_H

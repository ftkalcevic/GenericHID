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
    bool loadXML( QDomDocument &doc, ShapeCollection *pCol );

public slots:
    void onViewItemChanged( QGraphicsItem *item, QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    void clear();

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

    Editor *m_pEditor;
    QList<ShapeItem *> m_ShapeItems;
    QList<WireItem *> m_WireItems;
};

#endif // SHAPESCENE_H

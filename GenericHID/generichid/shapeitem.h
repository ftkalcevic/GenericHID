#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include "editor.h"


class ShapeItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT	    // Need Q_OBJECT to support signals and slots

public:
    ShapeItem(const QString &fileName, Editor *pEditor, QGraphicsItem *parent=0);
    ~ShapeItem();

private:
    Editor *m_pEditor;
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
};

#endif // SHAPEITEM_H

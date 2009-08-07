#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include <QGraphicsPixmapItem>


class ShapeItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT	    // Need Q_OBJECT to support signals and slots

public:
    ShapeItem(const QString &fileName, QGraphicsItem *parent=0);
    ~ShapeItem();

private:

};

#endif // SHAPEITEM_H

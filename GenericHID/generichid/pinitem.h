#ifndef PINITEM_H
#define PINITEM_H

#include <QGraphicsRectItem>

class PinItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    PinItem( QRectF rc, QGraphicsItem *parent = 0 );
    ~PinItem();

private:

};

#endif // PINITEM_H

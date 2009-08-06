#ifndef SHAPEMIMEDATA_H
#define SHAPEMIMEDATA_H

#include "shape.h"

class ShapeMimeData : public QMimeData
{
    Q_OBJECT

public:
    ShapeMimeData( const Shape *);
    ~ShapeMimeData();
    const Shape *shape() { return m_pShape; }

    virtual bool hasFormat( const QString & mimeType ) const { return false; }
    virtual QStringList formats () const { return QStringList(); }
    virtual QVariant retrieveData ( const QString & mimeType, QVariant::Type type ) const { return QVariant(); }

private:
    const Shape *m_pShape;
};

#endif // SHAPEMIMEDATA_H

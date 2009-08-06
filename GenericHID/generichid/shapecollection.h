#ifndef _SHAPECOLLECTION_H_
#define _SHAPECOLLECTION_H_

#include "shape.h"

class ShapeCollection
{
private:
    ShapeCollection(void);
public:
    virtual ~ShapeCollection(void);

    static ShapeCollection *LoadShapeCollection( const QString &sPath );
    const QList<Shape *> &shapes() { return m_Shapes; }

private:
    QList<Shape *> m_Shapes;
};

#endif

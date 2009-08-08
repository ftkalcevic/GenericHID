#ifndef _SHAPEINSTANCECOLLECTION_H_
#define _SHAPEINSTANCECOLLECTION_H_

#include "shape.h"
#include "shapeinstance.h"

class ShapeInstanceCollection
{
public:
    ShapeInstanceCollection(void);
    ~ShapeInstanceCollection(void);

    bool CanAdd( const Shape *pShape, QString &sError );
    ShapeInstance *CreateNewShape( const Shape *pShape, Editor *pEditor );
    const QList<ShapeInstance *> &instances() const { return m_Instances; }
private:
    QList<ShapeInstance *> m_Instances;
};

#endif

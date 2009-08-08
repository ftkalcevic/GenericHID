#ifndef _SHAPEPROPERTIES_H_
#define _SHAPEPROPERTIES_H_

#include "shapeproperty.h"

class ShapeProperties
{
public:
    ShapeProperties(void);
    ~ShapeProperties(void);

    void add( ShapeProperty *pProperty );
    QtProperty *topItem() const { return m_topItem; }
    void populate(class ShapeItem *pItem) const;
    void retreive(class ShapeItem *pItem) const;
    
private:
    QtProperty *m_topItem;
    QList<ShapeProperty *> m_Propeties;
};

#endif

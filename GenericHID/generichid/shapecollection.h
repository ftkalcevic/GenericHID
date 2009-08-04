#ifndef _SHAPECOLLECTION_H_
#define _SHAPECOLLECTION_H_

class ShapeCollection
{
private:
    ShapeCollection(void);
public:
    virtual ~ShapeCollection(void);

    static ShapeCollection *LoadShapeCollection( const QString &sPath );
};

#endif

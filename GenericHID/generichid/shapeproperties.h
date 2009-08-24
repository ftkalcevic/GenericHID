#ifndef _SHAPEPROPERTIES_H_
#define _SHAPEPROPERTIES_H_

#include "shapeproperty.h"

class ShapeProperties
{
public:
    ShapeProperties(const QString &sShapeName, const QString &sDescription);
    ~ShapeProperties(void);

    void add( ShapeProperty *pProperty );
    QtProperty *topItem() const { return m_topItem; }
    void populate(const QList<PropertyValue *> &Values) const;
    void retreive(const QList<PropertyValue *> &Values) const;
    const QList<ShapeProperty *> &items() const { return m_Properties; }
    int find( const QString &sName ) const;
    QtProperty *findQt( const QString &sName ) const;
    
private:
    QtProperty *m_topItem;
    QString m_sName;
    QString m_sDescription;
    QList<ShapeProperty *> m_Properties;
    QList<QtProperty *> m_QtProperties;
};

#endif

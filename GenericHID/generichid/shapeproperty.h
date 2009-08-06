#ifndef _SHAPEPROPERTY_H_
#define _SHAPEPROPERTY_H_


namespace  PropertyType
{
    enum PropertyType
    {
	String,
        Enum,
        Bool,
        Int,
        UsagePage,
        Usage,
	None
    };
    extern enum PropertyType fromString( const QString &s );
};

class ShapeProperty
{
protected:
    ShapeProperty( const QString &sName, const QString &sDescription );

public:
    virtual ~ShapeProperty(void);
    static ShapeProperty *CreateShapeProperty( QDomElement &node, const QString &sName, const QString &sDescription, PropertyType::PropertyType ePropType );

protected:
    virtual bool Configure( QDomElement &node ) { return true; }	// Extra configuration

    QString m_sName;
    QString m_sDescription;
};


#endif

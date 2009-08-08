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
    static void SetBrowserFactory( QtAbstractPropertyBrowser *browser );
    static void Init();
    static QtProperty *MakeGroupItem( const QString &s );
    virtual QtProperty *getQtProperty() = 0;

protected:
    virtual bool Configure( QDomElement &node ) { return true; }	// Extra configuration

    static QtGroupPropertyManager *m_groupManager;
    static QtStringPropertyManager *m_stringManager;
    static QtPointFPropertyManager *m_pointfScaleManager;
    static QtPointFPropertyManager *m_pointfManager;
    static QtDoublePropertyManager *m_doubleManager;
    static QtEnumPropertyManager *m_enumManager;
    static QtBoolPropertyManager *m_boolManager;
    static QtIntPropertyManager *m_intManager;
    
    static bool m_bInitialised;

    QString m_sName;
    QString m_sDescription;
};


#endif

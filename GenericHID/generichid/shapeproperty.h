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
	Timer13,
	Timer2,
	None
    };
    extern enum PropertyType fromString( const QString &s );
};

class PropertyValue
{
public:
    PropertyValue() {}
    virtual ~PropertyValue() {}
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
    virtual PropertyValue *createValue() const = 0;
    virtual void setValue(QtProperty *, PropertyValue *) const = 0;
    virtual void getValue(QtProperty *, PropertyValue *) const = 0;
    virtual QDomElement WriteXML(QDomElement &node, PropertyValue *) const;
    virtual void getXML(QDomElement &node, PropertyValue *) const = 0;

    const QString &name() const { return m_sName; }
    const QString &description() const { return m_sDescription; }

protected:
    virtual bool Configure( QDomElement & /*node*/ ) { return true; }	// Extra configuration

    static QtGroupPropertyManager *m_groupManager;
    static QtStringPropertyManager *m_stringManager;
    static QtPointFPropertyManager *m_pointfScaleManager;
    static QtPointFPropertyManager *m_pointfManager;
    static QtDoublePropertyManager *m_doubleManager;
    static QtEnumPropertyManager *m_enumManager;
    static QtBoolPropertyManager *m_boolManager;
    static QtIntPropertyManager *m_intManager;
    static UsagePropertyManager *m_usageManager;
    static QtStringPropertyManager *m_timerManager;
    static QtStringPropertyManager *m_timer13Manager;
    static QtStringPropertyManager *m_timer2Manager;
    
    static bool m_bInitialised;

    QString m_sName;
    QString m_sDescription;
};


#endif

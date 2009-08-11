#include "stdafx.h"
#include "usagepropertymanager.h"
#include "usages.h"

UsagePropertyManager::UsagePropertyManager(QObject *parent)
: QtAbstractPropertyManager(parent)
, m_settingValue(false)
{
    m_listPropertyManager = new ListPropertyManager(this);
    connect(m_listPropertyManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(slotEnumChanged(QtProperty *, int)));
    connect(m_listPropertyManager, SIGNAL(propertyDestroyed(QtProperty *)), this, SLOT(slotPropertyDestroyed(QtProperty *)));

    // Build the usagepage and usages lists
    const std::vector<UsagePage*> usagePages = Usages::GetUsagesPages();
    for ( std::vector<UsagePage*>::const_iterator itUsagePage = usagePages.begin(); itUsagePage != usagePages.end(); itUsagePage++ )
    {
	m_UsagePages.append( EnumItem( (*itUsagePage)->id(), (*itUsagePage)->name() ) );

	ListEnumList *sUsages = new ListEnumList;
	const std::vector<Usage*> usages = (*itUsagePage)->usages();
	for ( std::vector<Usage*>::const_iterator itUsage = usages.begin(); itUsage != usages.end(); itUsage++ )
	{
	    sUsages->append( EnumItem( (*itUsage)->id(), (*itUsage)->name() ));
	}
	m_UsagesMap.insert( (*itUsagePage)->id(), sUsages );
    }
}

UsagePropertyManager::~UsagePropertyManager()
{
    clear();
}

ListPropertyManager *UsagePropertyManager::subListPropertyManager() const
{
    return m_listPropertyManager;
}

QString UsagePropertyManager::value(const QtProperty *property) const
{
    return m_values.value(property, QString());
}

static void ExtractUsage( QString s, unsigned short &nUsagePage, unsigned short &nUsage )
{
    nUsagePage = 1;
    nUsage = 1;
    QStringList l = s.split(QChar(':'));
    if ( l.count() > 0 )
	nUsagePage = l[0].toUShort();
    if ( l.count() > 1 )
	nUsage = l[1].toUShort();
}


void UsagePropertyManager::setValue(QtProperty *property, const QString &val)
{
    const PropertyValueMap::iterator it = m_values.find(property);
    if (it == m_values.end())
        return;

    const QString oldVal = it.value();
    if (oldVal == val)
        return;

    it.value() = val;

    unsigned short nUsagePage;
    unsigned short nUsage;
    ExtractUsage( val, nUsagePage, nUsage );

    UpdateUsagePages( m_propertyToUsage[property], nUsagePage );

    bool settingValue = m_settingValue;
    m_settingValue = true;
    m_listPropertyManager->setValue(m_propertyToUsagePage[property], nUsagePage );
    m_listPropertyManager->setValue(m_propertyToUsage[property],  nUsage );
    m_settingValue = settingValue;

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void UsagePropertyManager::UpdateUsagePages( QtProperty *propUsage, unsigned short nUsagePage )
{
    QMap<int,ListEnumList *>::const_iterator it = m_UsagesMap.constFind( nUsagePage );
    if ( it != m_UsagesMap.constEnd() )
	m_listPropertyManager->setEnums( propUsage, *(it.value()) );
    else
	m_listPropertyManager->setEnums( propUsage, ListEnumList() );
}

void UsagePropertyManager::slotEnumChanged(QtProperty *property, int value)	// property == listproperty
{
    if (m_settingValue)
	return;

    if (QtProperty *prop = m_usagePageToProperty.value(property, 0)) 
    {
	// When the usage page changes, update the list of usages
        QtProperty *propUsagePage = property; 
        QtProperty *propUsage = m_propertyToUsage.value(prop, 0);
	int nUsagePage = value;
	int nUsage = m_listPropertyManager->value( propUsage );
	UpdateUsagePages( propUsage, nUsagePage );
	setValue(prop,QString("%1:%2").arg(nUsagePage).arg(nUsage) );
    }
    else if (QtProperty *prop = m_usageToProperty.value(property, 0)) 
    {
        QtProperty *propUsagePage = m_propertyToUsagePage.value(prop, 0);
        QtProperty *propUsage = property; 
	int nUsagePage = m_listPropertyManager->value( propUsagePage );
	int nUsage = m_listPropertyManager->value( propUsage );
	setValue(prop,QString("%1:%2").arg(nUsagePage).arg(nUsage) );
    }
}


void UsagePropertyManager::slotPropertyDestroyed(QtProperty *property)
{
    if (QtProperty *pProp = m_propertyToUsage.value(property, 0)) 
    {
        m_usageToProperty[pProp] = 0;
        m_propertyToUsage.remove(property);
    }
    else if (QtProperty *pProp = m_propertyToUsagePage.value(property, 0)) 
    {
        m_usagePageToProperty[pProp] = 0;
        m_propertyToUsagePage.remove(property);
    }
}


QString UsagePropertyManager::valueText(const QtProperty *property) const
{
    const PropertyValueMap::const_iterator it = m_values.constFind(property);
    if (it == m_values.constEnd())
        return QString();

    unsigned short nUsagePage;
    unsigned short nUsage;
    ExtractUsage( it.value(), nUsagePage, nUsage );

    QString sPage, sUsage;
    Usages::GetUsages( nUsagePage, nUsage, sPage, sUsage );
    return QString("%1:%2").arg(sPage,sUsage);
}

void UsagePropertyManager::initializeProperty(QtProperty *property) // property == UsageProperty
{
    QString val("1:1");
    m_values[property] = val;

    unsigned short nUsagePage;
    unsigned short nUsage;
    ExtractUsage( val, nUsagePage, nUsage );

    QtProperty *usagePageProp = m_listPropertyManager->addProperty();
    usagePageProp->setPropertyName(tr("Usage Page"));
    // Fill UsagePages.
    m_propertyToUsagePage.insert(property, usagePageProp );
    m_usagePageToProperty.insert(usagePageProp, property );
    property->addSubProperty(usagePageProp);

    QtProperty *usageProp = m_listPropertyManager->addProperty();
    usageProp->setPropertyName(tr("Usage"));
    // Usages are filled when on the change event
    m_propertyToUsage[property] = usageProp;
    m_usageToProperty[usageProp] = property;
    property->addSubProperty(usageProp);

    m_listPropertyManager->setEnums(usagePageProp, m_UsagePages);
    m_listPropertyManager->setValue(usagePageProp, nUsagePage);
    m_listPropertyManager->setValue(usageProp, nUsage);
}


void UsagePropertyManager::uninitializeProperty(QtProperty *property)
{
    QtProperty *usagePageProp = m_propertyToUsagePage[property];
    if (usagePageProp) 
    {
        m_propertyToUsagePage.remove(usagePageProp);
        delete usagePageProp;
    }
    m_propertyToUsagePage.remove(property);

    QtProperty *usageProp = m_propertyToUsage[property];
    if (usageProp) 
    {
        m_propertyToUsage.remove(usageProp);
        delete usageProp;
    }
    m_propertyToUsage.remove(property);

    m_values.remove(property);
}

#include "stdafx.h"
#include "usagepropertymanager.h"

UsagePropertyManager::UsagePropertyManager(QObject *parent)
: QtAbstractPropertyManager(parent)
, m_settingValue(false)
{
    m_enumPropertyManager = new QtEnumPropertyManager(this);
    connect(m_enumPropertyManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(slotEnumChanged(QtProperty *, int)));
    connect(m_enumPropertyManager, SIGNAL(propertyDestroyed(QtProperty *)), this, SLOT(slotPropertyDestroyed(QtProperty *)));
}

UsagePropertyManager::~UsagePropertyManager()
{
    clear();
}

QtEnumPropertyManager *UsagePropertyManager::subEnumPropertyManager() const
{
    return m_enumPropertyManager;
}

QString UsagePropertyManager::value(const QtProperty *property) const
{
    return m_values.value(property, QString());
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

    bool settingValue = m_settingValue;
    m_settingValue = true;
    int idx = 0;    // todo - ensure combos are populated.  find right ?
    m_enumPropertyManager->setValue(m_propertyToUsagePage[property], idx);
    m_enumPropertyManager->setValue(m_propertyToUsage[property], idx);
    m_settingValue = settingValue;

    emit propertyChanged(property);
    emit valueChanged(property, val);
}


void UsagePropertyManager::slotEnumChanged(QtProperty *property, int value)
{
    if (m_settingValue)
	return;

    if (QtProperty *prop = m_propertyToUsagePage.value(property, 0)) 
    {
	QString s = m_values[prop];
	// do something with changed usage page
        //f.setFamily(m_familyNames.at(value));
	setValue(prop, s);  // todo this isn't right.  Need to setValue with usagepage:usage
    }
    else if (QtProperty *prop = m_propertyToUsage.value(property, 0)) 
    {
	QString s = m_values[prop];
	// do something with changed usage page
	//f.setFamily(m_familyNames.at(value));
	setValue(prop, s);  // todo this isn't right.  Need to setValue with usagepage:usage
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

    // is this the usagepage:usage formatter
    return it.value();
}

QIcon UsagePropertyManager::valueIcon(const QtProperty *property) const
{
    const PropertyValueMap::const_iterator it = m_values.constFind(property);
    if (it == m_values.constEnd())
        return QIcon();

    // todo icon?
    return QIcon(); //fontValueIcon(it.value());
}

void UsagePropertyManager::initializeProperty(QtProperty *property)
{
    QString val;
    m_values[property] = val;

    QtProperty *usagePageProp = m_enumPropertyManager->addProperty();
    usagePageProp->setPropertyName(tr("Usage Page"));
    // Fill Usage pages.
    //m_enumPropertyManager->setEnumNames(usagePageProp, m_familyNames);
    //int idx = d_ptr->m_familyNames.indexOf(val.family());
    //if (idx == -1)
    //    idx = 0;
    //d_ptr->m_enumPropertyManager->setValue(familyProp, idx);
    m_propertyToUsagePage[property] = usagePageProp;
    m_usagePageToProperty[usagePageProp] = property;
    property->addSubProperty(usagePageProp);

    QtProperty *usageProp = m_enumPropertyManager->addProperty();
    usageProp->setPropertyName(tr("Usage"));
    // Fill Usages.
    //m_enumPropertyManager->setEnumNames(usageProp, m_familyNames);
    //int idx = d_ptr->m_familyNames.indexOf(val.family());
    //if (idx == -1)
    //    idx = 0;
    //d_ptr->m_enumPropertyManager->setValue(usageProp, idx);
    m_propertyToUsage[property] = usageProp;
    m_usageToProperty[usageProp] = property;
    property->addSubProperty(usageProp);
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

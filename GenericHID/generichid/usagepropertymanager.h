#ifndef _USAGEPROPERTYMANAGER_H_
#define _USAGEPROPERTYMANAGER_H_

#include "listpropertymanager.h"


class UsagePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT

public:
    UsagePropertyManager(QObject *parent = 0);
    ~UsagePropertyManager();

    ListPropertyManager *subListPropertyManager() const;
    QString value(const QtProperty *property) const;

public slots:
    void setValue(QtProperty *property, const QString &val);
    void slotEnumChanged(QtProperty *property, int value);
    void slotPropertyDestroyed(QtProperty *property);
public:
    typedef QMap<const QtProperty *, QString> PropertyValueMap;
    PropertyValueMap m_values;

    ListPropertyManager *m_listPropertyManager;

    QMap<const QtProperty *, QtProperty *> m_propertyToUsage;
    QMap<const QtProperty *, QtProperty *> m_propertyToUsagePage;

    QMap<const QtProperty *, QtProperty *> m_usageToProperty;
    QMap<const QtProperty *, QtProperty *> m_usagePageToProperty;

    ListEnumList m_UsagePages;
    QMap<int,ListEnumList *> m_UsagesMap;

    bool m_settingValue;

signals:
    void valueChanged(QtProperty *property, const QString &val);
protected:

    QString valueText(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
    void UpdateUsagePages( QtProperty *propUsage, unsigned short nUsagePage );
};

#endif // QTUSAGEPROPERTYMANAGER_H

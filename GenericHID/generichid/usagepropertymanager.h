#ifndef _USAGEPROPERTYMANAGER_H_
#define _USAGEPROPERTYMANAGER_H_

#include <QtProperty>

class UsagePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT

public:
    UsagePropertyManager(QObject *parent = 0);
    ~UsagePropertyManager();

    QtEnumPropertyManager *subEnumPropertyManager() const;
    QString value(const QtProperty *property) const;

public slots:
    void setValue(QtProperty *property, const QString &val);
    void slotEnumChanged(QtProperty *property, int value);
    void slotPropertyDestroyed(QtProperty *property);
public:
    typedef QMap<const QtProperty *, QString> PropertyValueMap;
    PropertyValueMap m_values;

    QtEnumPropertyManager *m_enumPropertyManager;

    QMap<const QtProperty *, QtProperty *> m_propertyToUsage;
    QMap<const QtProperty *, QtProperty *> m_propertyToUsagePage;

    QMap<const QtProperty *, QtProperty *> m_usageToProperty;
    QMap<const QtProperty *, QtProperty *> m_usagePageToProperty;

    bool m_settingValue;

signals:
    void valueChanged(QtProperty *property, const QString &val);
protected:

    QString valueText(const QtProperty *property) const;
    QIcon valueIcon(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
};

#endif // QTUSAGEPROPERTYMANAGER_H

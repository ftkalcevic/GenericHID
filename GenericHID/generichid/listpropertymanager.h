#ifndef LISTPROPERTYMANAGER_H
#define LISTPROPERTYMANAGER_H

class EnumItem
{
public:
    EnumItem() : i(0) {}
    EnumItem( int _i, const QString &_s ) : i(_i), s(_s) {}
    EnumItem( const EnumItem &other ) : i(other.i), s(other.s) {}
    QString s;
    int i;
};

typedef QList<EnumItem> ListEnumList;
typedef QMap<int,EnumItem> ListEnumsMap;

class ListPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT

public:
    ListPropertyManager(QObject *parent);
    ~ListPropertyManager();

    int value(const QtProperty *property) const;
    ListEnumList enums(const QtProperty *property) const;

public slots:
    void setValue(QtProperty *property, int i);
    void setEnums(QtProperty *property, const ListEnumList &names);
signals:
    void valueChanged(QtProperty *property, const int );
    void enumsChanged(QtProperty *property);
protected:
    QString valueText(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private:
    struct Data
    {
	Data() : val(0) {}
        int val;
        ListEnumList enums;
	ListEnumsMap enumsMap;
    };

    typedef QMap<const QtProperty *, Data> PropertyValueMap;
    PropertyValueMap m_values;
};



#endif // LISTPROPERTYMANAGER_H

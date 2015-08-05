// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef LISTPROPERTYMANAGER_H
#define LISTPROPERTYMANAGER_H

class EnumItem
{
public:
    EnumItem() : i(0) {}
    EnumItem( int _i, const QString &_s ) : i(_i), s(_s) {}
    EnumItem( const EnumItem &other ) : i(other.i), s(other.s) {}
    int i;
    QString s;
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

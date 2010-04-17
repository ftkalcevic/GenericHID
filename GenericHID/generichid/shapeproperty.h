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

#ifndef _SHAPEPROPERTY_H_
#define _SHAPEPROPERTY_H_

#include "timereditorfactory.h"
#include "keymatrixeditorfactory.h"

namespace  PropertyType
{
    enum PropertyType
    {
	String,
        Enum,
        Bool,
        Int,
	RCNames,
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
    ShapeProperty( const QString &sName, const QString &sDescription, bool bEnabled );

public:
    virtual ~ShapeProperty(void);
    static ShapeProperty *CreateShapeProperty( QDomElement &node, const QString &sName, const QString &sDescription, PropertyType::PropertyType ePropType, bool bEnabled, QString &sError );
    static void SetBrowserFactory( QtAbstractPropertyBrowser *browser, unsigned int nCpuClock );
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
    const bool enabled() const { return m_bEnabled; }

public:
    static QtGroupPropertyManager *m_groupManager;
    static QtStringPropertyManager *m_stringManager;
    static QtPointFPropertyManager *m_pointfScaleManager;
    static QtPointFPropertyManager *m_pointfManager;
    static QtDoublePropertyManager *m_doubleManager;
    static QtEnumPropertyManager *m_enumManager;
    static QtBoolPropertyManager *m_boolManager;
    static QtIntPropertyManager *m_intManager;
    static UsagePropertyManager *m_usageManager;
    static TimerStringPropertyManager *m_timer13Manager;
    static TimerStringPropertyManager *m_timer2Manager;
    static KeyMatrixStringPropertyManager *m_keyMatrixNameManager;

    static KeyMatrixEditorFactory *m_keyMatrixNameFactory;

protected:
    virtual bool Configure( QDomElement & /*node*/ ) { return true; }	// Extra configuration

    static bool m_bInitialised;

    QString m_sName;
    QString m_sDescription;
    bool m_bEnabled;
};


#endif

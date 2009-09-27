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

#include "stdafx.h"
#include "timereditorfactory.h"
#include "timereditwidget.h"
#include "timercounter.h"


QString TimerStringPropertyManager::valueText(const QtProperty *property) const 
{ 
    QString sDetails = QtStringPropertyManager::valueText( property ); 
    return TimerCounter::MakeUserDisplay( sDetails );
}

    
// ---------- EditorFactoryPrivate :
// Base class for editor factory private classes. Manages mapping of properties to editors and vice versa.

template <class Editor>
class EditorFactoryPrivate
{
public:

    typedef QList<Editor *> EditorList;
    typedef QMap<QtProperty *, EditorList> PropertyToEditorListMap;
    typedef QMap<Editor *, QtProperty *> EditorToPropertyMap;

    Editor *createEditor(QtProperty *property, QWidget *parent);
    void initializeEditor(QtProperty *property, Editor *e);
    void slotEditorDestroyed(QObject *object);

    PropertyToEditorListMap  m_createdEditors;
    EditorToPropertyMap m_editorToProperty;
};

template <class Editor>
Editor *EditorFactoryPrivate<Editor>::createEditor(QtProperty *property, QWidget *parent)
{
    Editor *editor = new Editor(parent);
    initializeEditor(property, editor);
    return editor;
}

template <class Editor>
void EditorFactoryPrivate<Editor>::initializeEditor(QtProperty *property, Editor *editor)
{
    Q_TYPENAME PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(property, EditorList());
    it.value().append(editor);
    m_editorToProperty.insert(editor, property);
}

template <class Editor>
void EditorFactoryPrivate<Editor>::slotEditorDestroyed(QObject *object)
{
    const Q_TYPENAME EditorToPropertyMap::iterator ecend = m_editorToProperty.end();
    for (Q_TYPENAME EditorToPropertyMap::iterator itEditor = m_editorToProperty.begin(); itEditor !=  ecend; ++itEditor) {
        if (itEditor.key() == object) {
            Editor *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            const Q_TYPENAME PropertyToEditorListMap::iterator pit = m_createdEditors.find(property);
            if (pit != m_createdEditors.end()) {
                pit.value().removeAll(editor);
                if (pit.value().empty())
                    m_createdEditors.erase(pit);
            }
            m_editorToProperty.erase(itEditor);
            return;
        }
    }
}

// TimerEditorFactoryPrivate

class TimerEditorFactoryPrivate : public EditorFactoryPrivate<TimerEditWidget>
{
    TimerEditorFactory *q_ptr;
    Q_DECLARE_PUBLIC(TimerEditorFactory)
public:

    void slotPropertyChanged(QtProperty *property, const QString &value);
    void slotSetValue(const QString &value);
};

void TimerEditorFactoryPrivate::slotPropertyChanged(QtProperty *property, const QString &value)
{
    const PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        return;
    QListIterator<TimerEditWidget *> itEditor(it.value());

    while (itEditor.hasNext())
        itEditor.next()->setValue(value);
}

void TimerEditorFactoryPrivate::slotSetValue(const QString &value)
{
    QObject *object = q_ptr->sender();
    const EditorToPropertyMap::ConstIterator ecend = m_editorToProperty.constEnd();
    for (EditorToPropertyMap::ConstIterator itEditor = m_editorToProperty.constBegin(); itEditor != ecend; ++itEditor)
        if (itEditor.key() == object) 
	{
            QtProperty *property = itEditor.value();
            TimerStringPropertyManager *manager = q_ptr->propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
}


TimerEditorFactory::TimerEditorFactory(int nBits, QStringList sPrescales, QObject *parent) 
: QtAbstractEditorFactory<TimerStringPropertyManager>(parent)
, d_ptr(new TimerEditorFactoryPrivate())
, m_nBits( nBits )
, m_sPrescales( sPrescales )

{
    d_ptr->q_ptr = this;
}

TimerEditorFactory::~TimerEditorFactory()
{
    qDeleteAll(d_ptr->m_editorToProperty.keys());
    delete d_ptr;
}

void TimerEditorFactory::connectPropertyManager(TimerStringPropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(slotPropertyChanged(QtProperty*,QString)));
}

QWidget *TimerEditorFactory::createEditor(TimerStringPropertyManager *manager, QtProperty *property, QWidget *parent)
{
    TimerEditWidget *editor = d_ptr->createEditor(property, parent);
    editor->setValue(manager->value(property));
    editor->setBits( m_nBits );
    editor->setPrescales( m_sPrescales );

    connect(editor, SIGNAL(valueChanged(QString)), this, SLOT(slotSetValue(QString)));
    connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void TimerEditorFactory::disconnectPropertyManager(TimerStringPropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(slotPropertyChanged(QtProperty*,QString)));
}


void TimerEditorFactory::slotPropertyChanged(QtProperty *p, const QString &s)
{
    d_ptr->slotPropertyChanged(p, s);
}

void TimerEditorFactory::slotEditorDestroyed(QObject *o)
{
    d_ptr->slotEditorDestroyed(o);
}

void TimerEditorFactory::slotSetValue(const QString &s)
{
    d_ptr->slotSetValue(s);
}


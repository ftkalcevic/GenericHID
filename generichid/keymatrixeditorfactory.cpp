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
#include "keymatrixeditorfactory.h"
#include "keymatrixeditwidget.h"

QString KeyMatrixStringPropertyManager::MakeValueText(const QString &s)
{ 
    QString sDetails = s;

    // strip out the [r,c] bits to make it look pretty.
    sDetails.replace( QRegExp("\\[\\d+,\\d+\\]"), "," );
    while ( sDetails.startsWith(",") )
	sDetails = sDetails.mid(1);

    return sDetails;
}

QString KeyMatrixStringPropertyManager::valueText(const QtProperty *property) const 
{ 
    QString sDetails = QtStringPropertyManager::valueText( property ); 
    // strip out the [r,c] bits to make it look pretty.
    return KeyMatrixStringPropertyManager::MakeValueText( sDetails );
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

// KeyMatrixEditorFactoryPrivate

class KeyMatrixEditorFactoryPrivate : public EditorFactoryPrivate<KeyMatrixEditWidget>
{
    KeyMatrixEditorFactory *q_ptr;
    Q_DECLARE_PUBLIC(KeyMatrixEditorFactory)
public:

    void slotPropertyChanged(QtProperty *property, const QString &value);
    void slotSetValue(const QString &value);
};

void KeyMatrixEditorFactoryPrivate::slotPropertyChanged(QtProperty *property, const QString &value)
{
    const PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        return;
    QListIterator<KeyMatrixEditWidget *> itEditor(it.value());

    while (itEditor.hasNext())
        itEditor.next()->setValue(value);
}

void KeyMatrixEditorFactoryPrivate::slotSetValue(const QString &value)
{
    QObject *object = q_ptr->sender();
    const EditorToPropertyMap::ConstIterator ecend = m_editorToProperty.constEnd();
    for (EditorToPropertyMap::ConstIterator itEditor = m_editorToProperty.constBegin(); itEditor != ecend; ++itEditor)
        if (itEditor.key() == object) 
	{
            QtProperty *property = itEditor.value();
            KeyMatrixStringPropertyManager *manager = q_ptr->propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
}


KeyMatrixEditorFactory::KeyMatrixEditorFactory(QObject *parent) 
: QtAbstractEditorFactory<KeyMatrixStringPropertyManager>(parent)
, d_ptr(new KeyMatrixEditorFactoryPrivate())
, m_nRows(1)
, m_nColumns(1)
{
    d_ptr->q_ptr = this;
}

KeyMatrixEditorFactory::~KeyMatrixEditorFactory()
{
    qDeleteAll(d_ptr->m_editorToProperty.keys());
    delete d_ptr;
}

void KeyMatrixEditorFactory::connectPropertyManager(KeyMatrixStringPropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(slotPropertyChanged(QtProperty*,QString)));
}

QWidget *KeyMatrixEditorFactory::createEditor(KeyMatrixStringPropertyManager *manager, QtProperty *property, QWidget *parent)
{
    KeyMatrixEditWidget *editor = d_ptr->createEditor(property, parent);
    editor->setValue(manager->value(property));
    editor->setRC( m_nRows, m_nColumns );

    connect(editor, SIGNAL(valueChanged(QString)), this, SLOT(slotSetValue(QString)));
    connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void KeyMatrixEditorFactory::disconnectPropertyManager(KeyMatrixStringPropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(slotPropertyChanged(QtProperty*,QString)));
}


void KeyMatrixEditorFactory::slotPropertyChanged(QtProperty *p, const QString &s)
{
    d_ptr->slotPropertyChanged(p, s);
}

void KeyMatrixEditorFactory::slotEditorDestroyed(QObject *o)
{
    d_ptr->slotEditorDestroyed(o);
}

void KeyMatrixEditorFactory::slotSetValue(const QString &s)
{
    d_ptr->slotSetValue(s);
}


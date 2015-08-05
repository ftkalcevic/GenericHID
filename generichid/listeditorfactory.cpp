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
#include "listeditorfactory.h"



template <class Editor>
Editor *_EditorFactoryPrivate<Editor>::createEditor(QtProperty *property, QWidget *parent)
{
    Editor *editor = new Editor(parent);
    initializeEditor(property, editor);
    return editor;
}

template <class Editor>
void _EditorFactoryPrivate<Editor>::initializeEditor(QtProperty *property, Editor *editor)
{
    Q_TYPENAME PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(property, EditorList());
    it.value().append(editor);
    m_editorToProperty.insert(editor, property);
}

template <class Editor>
void _EditorFactoryPrivate<Editor>::slotEditorDestroyed(QObject *object)
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


ListEditorFactory::ListEditorFactory(QObject *parent)
: QtAbstractEditorFactory<ListPropertyManager>(parent)
{
}

ListEditorFactory::~ListEditorFactory()
{
}


void ListEditorFactory::slotPropertyChanged(QtProperty *property, int /*value*/)
{
    if (!m_createdEditors.contains(property))
        return;

    updateProperty( property );
    //QListIterator<QComboBox *> itEditor(m_createdEditors[property]);
    //while (itEditor.hasNext()) 
    //{
    //    QComboBox *editor = itEditor.next();
    //    editor->blockSignals(true);
    //    editor->setCurrentIndex(value);
    //    editor->blockSignals(false);
    //}
}

void ListEditorFactory::slotEnumNamesChanged(QtProperty *property)
{
    if (!m_createdEditors.contains(property))
        return;

    ListPropertyManager *manager = propertyManager(property);
    if (!manager)
        return;

    ListEnumList pList = manager->enums(property);
    QListIterator<QComboBox *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) 
    {
        QComboBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->clear();
        foreach (const EnumItem &item, pList )
	    editor->addItem( item.s );
        editor->blockSignals(false);
	//updateProperty( QtProperty *property )
 //       int value = manager->value(property);
	//bool bDone = false;
 //       foreach (const EnumItem &item, pList )
	//    if ( item.i == value )
	//    {
	//	editor->findText( item.s );
	//	bDone = true;
	//	break;
	//    }
	//if ( !bDone )
	//    editor->setEditText( QString::number(value) );
    }
    updateProperty( property );
}

void ListEditorFactory::updateProperty( QtProperty *property )
{
    ListPropertyManager *manager = propertyManager(property);
    if (!manager)
        return;

    ListEnumList pList = manager->enums(property);
    QListIterator<QComboBox *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) 
    {
        QComboBox *editor = itEditor.next();
        editor->blockSignals(true);

        int value = manager->value(property);
	bool bDone = false;
        foreach (const EnumItem &item, pList )
	    if ( item.i == value )
	    {
		int nComboIndex = editor->findText( item.s );
		editor->setCurrentIndex( nComboIndex );
		bDone = true;
		break;
	    }

	if ( !bDone )
	    editor->setEditText( QString::number(value) );

        editor->blockSignals(false);
    }
}


void ListEditorFactory::comboIndexChanged(int value)
{
    QObject *object = sender();
    const  QMap<QComboBox *, QtProperty *>::ConstIterator ecend = m_editorToProperty.constEnd();
    for (QMap<QComboBox *, QtProperty *>::ConstIterator itEditor = m_editorToProperty.constBegin(); itEditor != ecend; ++itEditor)
        if (itEditor.key() == object) 
	{
            QtProperty *property = itEditor.value();
            ListPropertyManager *manager = propertyManager(property);
            if (!manager)
                return;

	    QString sComboText = itEditor.key()->currentText();

	    ListEnumList pList = manager->enums(property);
	    bool bFound = false;
	    foreach (const EnumItem &item, pList )
		if ( item.s.compare( sComboText, Qt::CaseInsensitive ) == 0 )
		{
		    value = item.i;
		    bFound = true;
		    break;
		}
	    if ( !bFound )
		value = sComboText.toUShort();

            manager->setValue(property, value);
            return;
        }
}

void ListEditorFactory::connectPropertyManager(ListPropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(slotPropertyChanged(QtProperty *, int)));
    connect(manager, SIGNAL(enumsChanged(QtProperty *)), this, SLOT(slotEnumNamesChanged(QtProperty *)));
}

QWidget *ListEditorFactory::createEditor(ListPropertyManager * /*manager*/, QtProperty *property, QWidget *parent)
{
    QComboBox *editor = _EditorFactoryPrivate<QComboBox>::createEditor(property, parent);
    editor->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    editor->view()->setTextElideMode(Qt::ElideRight);
    editor->setEditable( true );
    slotEnumNamesChanged(property);

    connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboIndexChanged(int)));
    connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void ListEditorFactory::disconnectPropertyManager(ListPropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(slotPropertyChanged(QtProperty *, int)));
    disconnect(manager, SIGNAL(enumsChanged(QtProperty *)), this, SLOT(slotEnumNamesChanged(QtProperty *)));
}


void ListEditorFactory::slotEditorDestroyed(QObject *o)
{
    _EditorFactoryPrivate<QComboBox>::slotEditorDestroyed(o);
}


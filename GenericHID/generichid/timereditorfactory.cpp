#include "stdafx.h"
#include "timereditorfactory.h"
#include "timereditwidget.h"

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
            QtStringPropertyManager *manager = q_ptr->propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
}


TimerEditorFactory::TimerEditorFactory(int nBits, QStringList sPrescales, QObject *parent) 
: QtAbstractEditorFactory<QtStringPropertyManager>(parent)
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

void TimerEditorFactory::connectPropertyManager(QtStringPropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(slotPropertyChanged(QtProperty*,QString)));
}

QWidget *TimerEditorFactory::createEditor(QtStringPropertyManager *manager, QtProperty *property, QWidget *parent)
{
    TimerEditWidget *editor = d_ptr->createEditor(property, parent);
    editor->setValue(manager->value(property));
    editor->setBits( m_nBits );
    editor->setPrescales( m_sPrescales );

    connect(editor, SIGNAL(valueChanged(QString)), this, SLOT(slotSetValue(QString)));
    connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void TimerEditorFactory::disconnectPropertyManager(QtStringPropertyManager *manager)
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


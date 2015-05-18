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

#ifndef LISTEDITORFACTORY_H
#define LISTEDITORFACTORY_H


template <class Editor>
class _EditorFactoryPrivate
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



class ListEditorFactory : public QtAbstractEditorFactory<ListPropertyManager>, public _EditorFactoryPrivate<QComboBox>
{
    Q_OBJECT

public:
    ListEditorFactory(QObject *parent=0);
    ~ListEditorFactory();

protected:
    void connectPropertyManager(ListPropertyManager *manager);
    QWidget *createEditor(ListPropertyManager *manager, QtProperty *property, QWidget *parent);
    void disconnectPropertyManager(ListPropertyManager *manager);
public slots:
    void slotPropertyChanged(QtProperty *property, int value);
    void slotEnumNamesChanged(QtProperty *property);
    void comboIndexChanged(int value);
    void updateProperty( QtProperty *property );

private:
    //Q_DECLARE_PRIVATE(QtEnumEditorFactory)
    //Q_DISABLE_COPY(QtEnumEditorFactory)
    //Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    //Q_PRIVATE_SLOT(d_func(), void slotEnumNamesChanged(QtProperty *,
    //                    const ListEnumList &))
    //Q_PRIVATE_SLOT(d_func(), void slotEnumIconsChanged(QtProperty *,
    //                    const QMap<int, QIcon> &))
    //Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
private slots:
    void slotEditorDestroyed(QObject *);

};

#endif // LISTEDITORFACTORY_H

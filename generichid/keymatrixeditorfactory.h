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

#ifndef KEYMATRIXEDITORFACTORY_H
#define KEYMATRIXEDITORFACTORY_H

#include <QtAbstractEditorFactoryBase>
#include <QtAbstractPropertyManager>

class KeyMatrixEditorFactoryPrivate;

class KeyMatrixStringPropertyManager: public QtStringPropertyManager
{
    Q_OBJECT

public:
    KeyMatrixStringPropertyManager(QObject *parent=0) : QtStringPropertyManager(parent) {}
    QString valueText(const QtProperty *property) const;
    static QString MakeValueText(const QString &s);
};

class KeyMatrixEditorFactory : public QtAbstractEditorFactory<KeyMatrixStringPropertyManager>
{
    Q_OBJECT

public:
    KeyMatrixEditorFactory(QObject *parent=0);
    ~KeyMatrixEditorFactory();

    void setRows( int nRows) { m_nRows = nRows; }
    void setColumns( int nColumns ) { m_nColumns = nColumns; }

public:
    void connectPropertyManager(KeyMatrixStringPropertyManager *manager);
    QWidget *createEditor(KeyMatrixStringPropertyManager *manager, QtProperty *property, QWidget *parent);
    void disconnectPropertyManager(KeyMatrixStringPropertyManager *manager);
private:
    KeyMatrixEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(KeyMatrixEditorFactory)
    Q_DISABLE_COPY(KeyMatrixEditorFactory)
    int m_nRows;
    int m_nColumns;

public slots:
    void slotPropertyChanged(QtProperty *, const QString &);
    void slotEditorDestroyed(QObject *);
    void slotSetValue(const QString &);
};


#endif // KeyMatrixEditorFactory_H



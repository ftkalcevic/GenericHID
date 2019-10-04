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

#ifndef TIMEREDITORFACTORY_H
#define TIMEREDITORFACTORY_H

#include <QtAbstractEditorFactoryBase>
#include <QtAbstractPropertyManager>

class TimerEditorFactoryPrivate;

class TimerStringPropertyManager: public QtStringPropertyManager
{
    Q_OBJECT

public:
    TimerStringPropertyManager(QObject *parent=0) : QtStringPropertyManager(parent) {}
    QString valueText(const QtProperty *property) const;
};

class TimerEditorFactory : public QtAbstractEditorFactory<TimerStringPropertyManager>
{
    Q_OBJECT

public:
    TimerEditorFactory(int nBits, QStringList sPrescales, unsigned int nClock, QObject *parent=0);
    ~TimerEditorFactory();

public:
    void connectPropertyManager(TimerStringPropertyManager *manager);
    QWidget *createEditor(TimerStringPropertyManager *manager, QtProperty *property, QWidget *parent);
    void disconnectPropertyManager(TimerStringPropertyManager *manager);
private:
    TimerEditorFactoryPrivate *d_ptr;
    int m_nBits;
    QStringList m_sPrescales;
    unsigned int m_nCPUClockFrequency;
    Q_DECLARE_PRIVATE(TimerEditorFactory)
    Q_DISABLE_COPY(TimerEditorFactory)

public slots:
    void slotPropertyChanged(QtProperty *, const QString &);
    void slotEditorDestroyed(QObject *);
    void slotSetValue(const QString &);
};


#endif // TIMEREDITORFACTORY_H



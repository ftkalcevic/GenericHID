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

#ifndef _MRU_H_
#define _MRU_H_

#include <QObject>
#include <QObject>
#include <QAction>
#include <QActionGroup>
#include <QList>

const int MAX_MRU = 4;


class MRU: public QObject
{
    Q_OBJECT
public:
    MRU(void);
    ~MRU(void);
    void setMenu( QMenu *pMenu );
    void append( const QString &s );
    int count() const { return m_MRU.count(); }
    QString operator []( int i ) const { if ( i < m_MRU.count() ) return m_MRU[i]; else return QString(); }
    void setEnabled( bool bEnabled );

signals:
    void MRUSelected(const QString &sFile);
private slots:
    void onActionTriggered(QAction*);
private:
    void Update();
    QList<QString> m_MRU;
    QAction *m_pSeparator;
    QList<QAction *> m_Actions;
    QActionGroup *m_pActionGroup;
};

#endif

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

#ifndef PROGRAMMERTHREAD_H
#define PROGRAMMERTHREAD_H

#include <QThread>
#include "programmer.h"

class ProgrammerThread : public QThread, public Programmer
{
    Q_OBJECT

public:
    ProgrammerThread(QObject *parent);
    ~ProgrammerThread();
    bool StartProgram( const QString &sEeprom, const QString &sFirmwarePath );
    bool success() const { return m_bSuccess; }

signals:
    void onUpdateStatus( int n );
    void onCompletionStatus( int nPercentage );

private:
    virtual void UpdateStatus( ProgramState::ProgramState status );
    virtual void CompletionStatus( int nPercentComplete );
    virtual void run();
    QString m_sEeprom;
    QString m_sFirmwarePath;
    bool m_bSuccess;
};

#endif // PROGRAMMERTHREAD_H

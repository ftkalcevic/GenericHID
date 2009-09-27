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
#include "programmerthread.h"

ProgrammerThread::ProgrammerThread(QObject *parent)
	: QThread(parent)
{

}

ProgrammerThread::~ProgrammerThread()
{

}

void ProgrammerThread::UpdateStatus( ProgramState::ProgramState status )
{
    emit onUpdateStatus( (int)status );
}

void ProgrammerThread::CompletionStatus( int nPercentComplete )
{
    emit onCompletionStatus( nPercentComplete );
}


bool ProgrammerThread::StartProgram( const QString &sEeprom, const QString &sFirmwarePath )
{
    m_bSuccess = false;
    m_sEeprom = sEeprom;
    m_sFirmwarePath = sFirmwarePath;

    start();
    return true;
}


void ProgrammerThread::run()
{
    m_bSuccess = Programmer::Program( m_sEeprom, m_sFirmwarePath );
    UpdateStatus( ProgramState::Done );
}

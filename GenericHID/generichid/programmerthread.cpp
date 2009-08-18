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
    m_sEeprom = sEeprom;
    m_sFirmwarePath = sFirmwarePath;

    start();
    return true;
}


void ProgrammerThread::run()
{
    Programmer::Program( m_sEeprom, m_sFirmwarePath );
    UpdateStatus( ProgramState::Done );
}

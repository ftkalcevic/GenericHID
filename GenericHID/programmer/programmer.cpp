#include "stdafx.h"
#include "programmer.h"
#include "flip.h"


Programmer::Programmer()
: m_flip( NULL )
{

}

Programmer::~Programmer()
{
    if ( m_flip != NULL )
	delete m_flip;
}

bool Programmer::Init()
{
    m_flip = new Flip();

    if ( !m_flip->Initialise() )
	return false;
    return true;
}


bool Programmer::Program( const QString &sEepromPath, const QString &sFirmwarePath )
{
    UpdateStatus( ProgramState::ErasingDevice );
    if ( !m_flip->EraseDevice() )
	return false;

    UpdateStatus( ProgramState::ProgrammingEEPROM );
    if ( !m_flip->StartProgramming(MemoryType::EEPROM, sEepromPath ) )
	return false;

    UpdateStatus( ProgramState::VerifyingEEPROM );
    if ( !m_flip->StartVerify() )
	return false;

    UpdateStatus( ProgramState::ProgrammingFlash );
    if ( !m_flip->StartProgramming(MemoryType::FLASH, sFirmwarePath ))
	return false;

    UpdateStatus( ProgramState::VerifyingFlash );
    if ( !m_flip->StartVerify())
	return false;

    UpdateStatus( ProgramState::Done );
    return true;
}

bool Programmer::RunFirmware()
{
    if ( !m_flip->EnterApplicationMode(ResetMode::Hard, 0 ) )
	return false;
    return true;
}

const QString &Programmer::LastError()
{
    return m_flip->LastError();
}

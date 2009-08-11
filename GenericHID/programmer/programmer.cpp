#include "stdafx.h"
#include "programmer.h"
#include "dfuprogrammer.h"


Programmer::Programmer()
: m_programmer( NULL )
{

}

Programmer::~Programmer()
{
    Terminate();
}

void Programmer::completion_callback( void *user_data, int percent )
{
    Programmer *that = reinterpret_cast<Programmer *>( user_data );
    that->CompletionStatus( percent );
}

bool Programmer::Init()
{
    if ( m_programmer == NULL )
    {
	m_programmer = new DFUProgrammer( tar_at90usb1287 );
	m_programmer->RegisterCallback( &completion_callback, this );
    }

    if ( !m_programmer->GetDevice() )
	return false;
    return true;
}

bool Programmer::Terminate()
{
    if ( m_programmer != NULL )
    {
	m_programmer->ReleaseDevice();
	delete m_programmer;
	m_programmer = NULL;
    }

    return true;
}

bool Programmer::Program( const QString &sEepromPath, const QString &sFirmwarePath )
{
    if ( m_programmer == NULL )
	return false;

    IntelHexBuffer eeprom = m_programmer->LoadHex(MemoryType::EEPROM, sEepromPath);
    if ( eeprom.isEmpty() )
	return false;

    IntelHexBuffer firmware = m_programmer->LoadHex(MemoryType::FLASH, sFirmwarePath);
    if ( firmware.isEmpty() )
	return false;

    UpdateStatus( ProgramState::ErasingDevice );
    if ( !m_programmer->EraseDevice() )
	return false;

    UpdateStatus( ProgramState::ProgrammingEEPROM );
    if ( !m_programmer->StartProgramming( eeprom ) )
	return false;

    UpdateStatus( ProgramState::ProgrammingFlash );
    if ( !m_programmer->StartProgramming( firmware ))
	return false;

    UpdateStatus( ProgramState::VerifyingEEPROM );
    if ( !m_programmer->StartVerify( eeprom ) )
	return false;

    UpdateStatus( ProgramState::VerifyingFlash );
    if ( !m_programmer->StartVerify( firmware ))
	return false;

    UpdateStatus( ProgramState::Done );

    return true;
}

bool Programmer::RunFirmware()
{
 //   if ( !m_flip->EnterApplicationMode(ResetMode::Hard, 0 ) )
	//return false;
    if ( !m_programmer->EnterApplicationMode(ResetMode::Hard, 0 ) )
	return false;
    return true;
}

QString Programmer::LastError()
{
    //return m_flip->LastError();
    return QString();
}

#include "stdafx.h"
#include "programmer.h"
#include "dfuprogrammer.h"


Programmer::Programmer()
: m_programmer( NULL )
{

}

Programmer::~Programmer()
{
}

bool Programmer::Init()
{
    m_programmer = new DFUProgrammer( tar_at90usb1287 );

    if ( !m_programmer->GetDevice() )
	return false;
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

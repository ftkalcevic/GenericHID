#include "stdafx.h"
#include "programmer.h"
//#include "flip.h"
#include "dfuprogrammer.h"


Programmer::Programmer()
//: m_flip( NULL )
: m_programmer( NULL )
{

}

Programmer::~Programmer()
{
 //   if ( m_flip != NULL )
	//delete m_flip;
}

bool Programmer::Init()
{
 //   m_flip = new Flip();

 //   if ( !m_flip->Initialise() )
	//return false;
 //   return true;
    m_programmer = new DFUProgrammer( tar_at90usb1287 );

    if ( !m_programmer->GetDevice() )
	return false;
    return true;
}


bool Programmer::Program( const QString &sEepromPath, const QString &sFirmwarePath )
{
 //   UpdateStatus( ProgramState::ErasingDevice );
 //   if ( !m_flip->EraseDevice() )
	//return false;

 //   UpdateStatus( ProgramState::ProgrammingEEPROM );
 //   if ( !m_flip->StartProgramming(MemoryType::EEPROM, sEepromPath ) )
	//return false;

 //   UpdateStatus( ProgramState::VerifyingEEPROM );
 //   if ( !m_flip->StartVerify() )
	//return false;

 //   UpdateStatus( ProgramState::ProgrammingFlash );
 //   if ( !m_flip->StartProgramming(MemoryType::FLASH, sFirmwarePath ))
	//return false;

 //   UpdateStatus( ProgramState::VerifyingFlash );
 //   if ( !m_flip->StartVerify())
	//return false;

 //   UpdateStatus( ProgramState::Done );

    UpdateStatus( ProgramState::ErasingDevice );
    if ( !m_programmer->EraseDevice() )
	return false;

    UpdateStatus( ProgramState::ProgrammingEEPROM );
    if ( !m_programmer->StartProgramming(MemoryType::EEPROM, sEepromPath ) )
	return false;

 //   UpdateStatus( ProgramState::VerifyingEEPROM );
 //   if ( !m_programmer->StartVerify() )
	//return false;

    UpdateStatus( ProgramState::ProgrammingFlash );
    if ( !m_programmer->StartProgramming(MemoryType::FLASH, sFirmwarePath ))
	return false;

 //   UpdateStatus( ProgramState::VerifyingFlash );
 //   if ( !m_programmer->StartVerify())
	//return false;

    UpdateStatus( ProgramState::Done );

    return true;
}

bool Programmer::RunFirmware()
{
 //   if ( !m_flip->EnterApplicationMode(ResetMode::Hard, 0 ) )
	//return false;
    if ( !m_programmer->EnterApplicationMode(ResetMode::Soft, 0 ) )
	return false;
    return true;
}

QString Programmer::LastError()
{
    //return m_flip->LastError();
    return QString();
}

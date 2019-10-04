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
#include "programmer.h"
#include "dfuprogrammer.h"
#include "teensyprogrammer.h"


Programmer::Programmer()
    : m_Logger( QCoreApplication::applicationName(), "Programmer" )
    , m_programmer( NULL )
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

bool Programmer::Init( ProgrammerType::ProgrammerType type )
{
    LOG_DEBUG( m_Logger, QString("Initialising programmer type '%1'").arg(type) );
    if ( m_programmer == NULL )
    {
        switch ( type )
        {
        case ProgrammerType::DFU:
            m_programmer = new DFUProgrammer( tar_at90usb1287 );
            break;
            
        case ProgrammerType::HalfK:
            m_programmer = new TeensyProgrammer();
            break;
        }
        m_programmer->RegisterCallback( &completion_callback, this );
    }
    
    if ( !m_programmer->GetDevice() )
    {
        LOG_ERROR( m_Logger, QString("Failed to get device") );
        return false;
    }
    LOG_DEBUG( m_Logger, QString("Initialisation Complete") );
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

bool Programmer::Program( const QString &sEeprom, const QString &sFirmwarePath )
{
    LOG_DEBUG( m_Logger, QString("Programming. Firmware=%1").arg(sFirmwarePath) );
    LOG_DEBUG( m_Logger, QString("Eeprom=%1").arg(sEeprom) );
    
    if ( m_programmer == NULL )
        return false;
    
    LOG_DEBUG( m_Logger, QString("Loading flash") );
    IntelHexBuffer firmware = m_programmer->LoadHexFile(MemoryType::FLASH, sFirmwarePath);
    if ( firmware.isEmpty() )
        return false;
    
    LOG_DEBUG( m_Logger, QString("Apending EEPROM flash") );
    m_programmer->AppendHex(firmware, MemoryType::FLASH, sEeprom);
    if ( firmware.isEmpty() )
        return false;
    
    UpdateStatus( ProgramState::ErasingDevice );
    if ( !m_programmer->EraseDevice() )
        return false;
    
    //   UpdateStatus( ProgramState::ProgrammingEEPROM );
    //   if ( !m_programmer->StartProgramming( eeprom ) )
    //return false;
    
    UpdateStatus( ProgramState::ProgrammingFlash );
    if ( !m_programmer->StartProgramming( firmware ))
        return false;
    
    //   UpdateStatus( ProgramState::VerifyingEEPROM );
    //   if ( !m_programmer->StartVerify( eeprom ) )
    //return false;
    
    UpdateStatus( ProgramState::VerifyingFlash );
    if ( !m_programmer->StartVerify( firmware ))
        return false;
    
    UpdateStatus( ProgramState::Done );
    
    return true;
}

bool Programmer::RunFirmware()
{
    if ( !m_programmer->EnterApplicationMode(ResetMode::Hard, 0 ) )
        return false;
    return true;
}

QString Programmer::LastError()
{
    //return m_flip->LastError();
    return QString();
}

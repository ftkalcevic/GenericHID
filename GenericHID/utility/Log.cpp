// hidcomp/hidconfig, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

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

#include "log.h"
#include "logcore.h"

// Logger implementation

Logger::Logger( const QString &sModule, const QString &sComponent )
: m_sModule( sModule )
, m_sComponent( sComponent )
{
    LogCore::Init();
}

void Logger::LogMsg( LogTypes::LogTypes type, const char *sFile, int nLine, const QString &sMsg )
{
    QDateTime now = QDateTime::currentDateTime();
    if ( WillLog(type) )
        LogCore::LogMsg( type, now, m_sModule, m_sComponent, sFile, nLine, sMsg );
}

bool Logger::WillLog(LogTypes::LogTypes type)
{
    return LogCore::WillLog( m_sModule, m_sComponent, type );
}

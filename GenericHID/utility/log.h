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

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <QString>
#include "logtypes.h"

#ifdef _WIN32
#pragma warning(disable:4512)
#endif

#define ENABLE_LOGGING
#ifdef ENABLE_LOGGING

#define LOG_MSG( ClassLogger, Type, Msg )   { if ( (ClassLogger).WillLog(Type) ) (ClassLogger).LogMsg( Type, __FILE__, __LINE__, Msg ); }


class Logger
{
private:
    QString m_sModule;
    QString m_sComponent;
public:
    Logger( const QString &sModule, const QString &sComponent );
    void LogMsg( LogTypes::LogTypes type, const char *sFile, int nLine, const QString &sMsg );
    bool WillLog(LogTypes::LogTypes type);
};

#else

#define LOG_MSG( ClassLogger, Type, Msg )

#endif



#endif

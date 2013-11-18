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

#include "logcore.h"
#include "log.h"
#include <QCoreApplication>
#include <QThread>

#ifdef _WIN32
    #include <atlbase.h>
#endif

// LogCore statics 

LogCore *LogCore::m_LogCore = NULL;
int LogCore::m_nRefCount = 0;
bool LogCore::m_bLog = false;
QMap<QThread *,int> LogCore::m_ThreadMap;


LogCore::LogCore()
{
}


void LogCore::Init()
{
    if ( m_LogCore == NULL )
    {
        LogCore *core = new LogCore();
        if ( m_LogCore != NULL )
            m_LogCore = core;
    }
}


static const char *toString(LogTypes::LogTypes type)
{
    switch ( type )
    {
        case LogTypes::Error:        return "Error";
        case LogTypes::Warning:      return "Warning";
        case LogTypes::Info:         return "Info";
        case LogTypes::Debug:        return "Debug";
        case LogTypes::Audit:        return "Audit";
        case LogTypes::FuncEnter:    return "FuncEnter";
        case LogTypes::FuncExit:     return "FuncExit";
        default:                     return "";
    }
}

// Qt doesn't give a very presentable thread id, so, as the id doesn't mean anything,
// and all we want is to distinguish between threads, we map the thread pointer to an integer.
int LogCore::GetThreadId()
{
    QMap<QThread *,int>::iterator it = m_ThreadMap.find( QThread::currentThread() );

    // Already there.
    if ( it != m_ThreadMap.end() )
	return it.value();

    // Create new id, starting at 1
    m_ThreadMap.insert( QThread::currentThread(), m_ThreadMap.count()+1 );
    return m_ThreadMap.count();
}

void LogCore::LogMsg( LogTypes::LogTypes type, QDateTime timeStamp, const QString &sModule, const QString &sComponent, const QString &sFile, int nLine, const QString &sMsg )
{
    QString s = QString("%1,%2,%3,%4:%5,%6,%7,%8,%9\n").arg(timeStamp.toString("yyyy/MM/dd hh:mm:ss.zzz")).arg(QCoreApplication::applicationPid()).arg(GetThreadId()).arg(sFile).arg(nLine).arg(toString(type)).arg(sModule).arg(sComponent).arg(sMsg);
    // Make sure we only have 1 \n
    while ( s.length() > 1 && s[s.length()-2] == QChar('\n') )
        s.remove( s.length()-1, 1 );

    #ifdef _WIN32
	OutputDebugStringW( (LPCWSTR)s.utf16() ); 
    #else
        fputs( s.toAscii().constData(), stderr );
        fflush( stderr );
    #endif
}


bool LogCore::WillLog( const QString &/*sModule*/, const QString /*sComponent*/, LogTypes::LogTypes type )
{
    if ( type == LogTypes::Error || type == LogTypes::Warning || type == LogTypes::Audit )
        return true;
    return m_bLog;  // todo
}




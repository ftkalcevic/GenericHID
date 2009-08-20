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

#ifndef _LOGCORE_H_
#define _LOGCORE_H_

#ifdef _WIN32
#pragma warning(push,1)
#endif
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <QMap>
#include <QThread>
#ifdef _WIN32
#pragma warning(pop)
#endif

#include "logtypes.h"

class LogCore
{
    static QMutex m_LogMutex;
    static int m_nRefCount;
    static LogCore *m_LogCore;
    static bool m_bLog;
    static QMap<QThread *,int> m_ThreadMap;

    static int GetThreadId();

public:
    LogCore();
    static void SetLog( bool b ) { m_bLog = b; }
    static void Init();
    static void LogMsg( LogTypes::LogTypes type, QDateTime timeStamp, const QString &sModule, const QString &sComponent, const QString &sFile, int nLine, const QString &sMsg );
    static bool WillLog( const QString &sModule, const QString sComponent, LogTypes::LogTypes type );
};


#endif

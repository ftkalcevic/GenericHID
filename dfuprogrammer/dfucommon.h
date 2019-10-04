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

#ifndef _DFU_COMMON_H_
#define _DFU_COMMON_H_

#include "inttypes.h"

#ifdef _WIN32
#pragma warning(push, 1)
#endif


#include <QVector>
#include <QIODevice>
#include <QString>
#include <QVector>
#include <QFile>
#include <QBuffer>
#include "log.h"

extern Logger g_Logger;
extern QString h_sLastError;

//#define DEBUG(...)  dfu_debug( __FILE__, __FUNCTION__, __LINE__, DFU_DEBUG_THRESHOLD, __VA_ARGS__ )
//#define TRACE(...)  dfu_debug( __FILE__, __FUNCTION__, __LINE__, DFU_TRACE_THRESHOLD, __VA_ARGS__ )
//#define MSG_DEBUG(...)  dfu_debug( __FILE__, __FUNCTION__, __LINE__, DFU_MESSAGE_DEBUG_THRESHOLD, __VA_ARGS__ )
#define DEBUG_MSG(s)	    LOG_MSG( g_Logger, LogTypes::Debug, s )
#define ERROR_MSG(s)	    LOG_MSG( g_Logger, LogTypes::Error, s )

#ifdef _WIN32
#pragma warning(pop)
//#pragma warning(disable:4251)
#endif

#endif

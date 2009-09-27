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

#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <assert.h>

#ifdef _WIN32
#pragma warning(push, 1)
#endif

#include <QString>
#include <QStringList>
#include <QDomDocument>
#include <QFile>
#include <QByteArray>
#include <QVector>
#include <QCoreApplication>

#ifdef _WIN32
#pragma warning(pop)
//#pragma warning(disable:4251)
#endif

#include "xmlutility.h"

#include "datatypes.h"
#include "common.h"
#include "log.h"

#ifdef _WIN32
#include <atlbase.h>
#endif

#endif

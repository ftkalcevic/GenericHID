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

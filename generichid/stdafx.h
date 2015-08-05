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

#ifdef _WIN32
#include <atlbase.h>
#pragma warning(push, 1)
#endif

#include <QtGui>
#include <QTimer>
#include <QDomDocument>
#include <QDomElement>
#include <QMimeData>
#include <QVariant>
#include <QToolButton>
#include <QtGui/QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QSettings>

#include <QtLineEditFactory>
#include <QtDoubleSpinBoxFactory>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtpropertymanager.h"
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include <QtTreePropertyBrowser>
#include <QtAbstractPropertyManager>
#include "usagepropertymanager.h"
#include <QtProperty>

#ifdef _WIN32
#pragma warning(pop)
#endif

#include "common.h"
#include "datatypes.h"
#include "log.h"
#include "xmlutility.h"
#include <assert.h>


namespace UserTypes
{
    enum UserTypes
    {
	PinItemType = 1,
	ShapeItemType, 
	WireItemType
    };
}

#endif


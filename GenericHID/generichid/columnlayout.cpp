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

// This is based on the QT example, FlowLayout.  Rather than organise from 
// left to right, top to bottom, we organise from top to bottom, then left
// to right, like newspaper columns.  Each column will be as wide as the 
// widest widget in each column

/****************************************************************************
 **
 ** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 ** Contact: Qt Software Information (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain
 ** additional rights. These rights are described in the Nokia Qt LGPL
 ** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at qt-sales@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "stdafx.h"
#include "columnlayout.h"

#include <QtGui>

#include "columnlayout.h"

ColumnLayout::ColumnLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
: QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setMargin(margin);
}

ColumnLayout::ColumnLayout(int margin, int hSpacing, int vSpacing)
: m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setMargin(margin);
}

ColumnLayout::~ColumnLayout()
{
    clear();
}

void ColumnLayout::clear()
{
    QLayoutItem *item;
    while ( (item = takeAt(0)) != NULL )
	delete item;
}

void ColumnLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int ColumnLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
	return m_hSpace;
    } else {
	return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int ColumnLayout::verticalSpacing() const
{
    if (m_vSpace >= 0) {
	return m_vSpace;
    } else {
	return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int ColumnLayout::count() const
{
    return itemList.size();
}

QLayoutItem *ColumnLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *ColumnLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
	return itemList.takeAt(index);
    else
	return 0;
}

Qt::Orientations ColumnLayout::expandingDirections() const
{
    return 0;
}

bool ColumnLayout::hasHeightForWidth() const
{
    return true;
}

int ColumnLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void ColumnLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize ColumnLayout::sizeHint() const
{
    return minimumSize();
}

QSize ColumnLayout::minimumSize() const
{
    QSize size;
    QLayoutItem *item;
    foreach (item, itemList)
	size = size.expandedTo(item->minimumSize());

    size += QSize(2*margin(), 2*margin());
    return size;
}

int ColumnLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int colWidth = 0;

    QLayoutItem *item;
    foreach (item, itemList) 
    {
	QWidget *wid = item->widget();
	int spaceX = horizontalSpacing();
	if (spaceX == -1)
	    spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
	int spaceY = verticalSpacing();
	if (spaceY == -1)
	    spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

	int nextY = y + item->sizeHint().height() + spaceY;
	if (nextY - spaceY > effectiveRect.bottom() && colWidth > 0) 
	{
	    // go back and make the width of each item in this column, the specified width.
	    foreach (QLayoutItem *item2, itemList) 
		if ( item2->geometry().x() == x )
		    item2->setGeometry( QRect(item2->geometry().topLeft(), QSize(colWidth,item2->geometry().height()) ) );

	    y = effectiveRect.y();
	    x = x + colWidth + spaceX;
	    nextY = y + item->sizeHint().height() + spaceY;
	    colWidth = 0;
	}

	if (!testOnly)
	    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

	y = nextY;
	colWidth = qMax(colWidth, item->sizeHint().width());
    }

    foreach (QLayoutItem *item2, itemList) 
	if ( item2->geometry().x() == x )
	    item2->setGeometry( QRect(item2->geometry().topLeft(), QSize(colWidth,item2->geometry().height()) ) );

    return x + colWidth - rect.x() + right;
}

int ColumnLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent) {
	return -1;
    } else if (parent->isWidgetType()) {
	QWidget *pw = static_cast<QWidget *>(parent);
	return pw->style()->pixelMetric(pm, 0, pw);
    } else {
	return static_cast<QLayout *>(parent)->spacing();
    }
}
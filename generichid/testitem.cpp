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
#include "hiddevice.h"
#include "hidparser.h"
#include "usages.h"
#include "testitem.h"
#include "testinputbutton.h"
#include "testinputvalue.h"
#include "testinputhatswitch.h"
#include "testoutputled.h"
#include "testoutputvalue.h"

TestItem::TestItem(const char *sClass, byte nReportId, QWidget *parent)
: QWidget(parent)
, m_nReportId(nReportId) 
, m_Logger(QCoreApplication::applicationName(), sClass )
{

}

TestItem::~TestItem()
{
}


TestItem *TestItem::CreateItem( class HIDDevice *pDevice, struct HID_ReportItem_t *pHIDItem )
{
    TestItem *pItem = NULL;
    if ( (pHIDItem->ItemFlags & IOF_CONSTANT) == 0 )
    {
	QString sName;
	if ( pHIDItem->Attributes.StringIndex != 0 )
	    sName = pDevice->GetDescriptorString( pHIDItem->Attributes.StringIndex );
	else
	{
	    const UsagePage *page = Usages::GetUsagePage( pHIDItem->Attributes.UsagePage );
	    if ( page != NULL )
	    {
		const Usage *usage = page->GetUsage( pHIDItem->Attributes.Usage );
		QString sUsage = page->GetUsageString( pHIDItem->Attributes.Usage );
		if ( usage == NULL )
		    sName = page->name() + QString(":") + sUsage;
		else
		    sName = sUsage;
	    }
	    else
	    {
		QString sUsage, sPage;
		Usages::GetUsages( pHIDItem->Attributes.UsagePage, pHIDItem->Attributes.Usage, sPage, sUsage );
		sName = sPage + ":" + sUsage;
	    }
	}
	if ( pHIDItem->ItemType == REPORT_ITEM_TYPE_In )
	{
	    // Define a button as any input with a bit size of 1.
	    if ( pHIDItem->BitSize == 1 )
		pItem = new TestInputButton(sName,pHIDItem);
	    else if ( pHIDItem->Attributes.UsagePage == USAGEPAGE_GENERIC_DESKTOP_CONTROLS && pHIDItem->Attributes.Usage == USAGE_HATSWITCH )
		pItem = new TestInputHatSwitch(sName,pHIDItem);
	    else
		pItem = new TestInputValue(sName,pHIDItem);
	}
	else if ( pHIDItem->ItemType == REPORT_ITEM_TYPE_Out )
	{
	    if ( pHIDItem->Attributes.UsagePage == USAGEPAGE_ALPHANUMERIC_DISPLAY )
	    {
		//continue;       // ignore displays - we need a collection
	    }
	    else
	    {
		// Define an LED as any output with a bit size of 1.
		if ( pHIDItem->BitSize == 1 )
		    pItem = new TestOutputLED(sName,pHIDItem);
		else
		    pItem = new TestOutputValue(sName,pHIDItem);
	    }
	}
    }
    return pItem;
}


void TestItem::paintEvent( QPaintEvent * event )
{
    event->ignore();
    QPainter p2( this );
    //if ( hasFocus() )
    {
	p2.drawRect( rect().adjusted(0,0,-1,-1) );
    }

}

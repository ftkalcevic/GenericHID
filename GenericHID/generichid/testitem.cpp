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

TestItem::TestItem(byte nReportId, QWidget *parent)
: QWidget(parent)
, m_nReportId(nReportId) 
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

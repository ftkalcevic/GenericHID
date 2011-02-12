// hidcomp/hidconfig/generichid, HID device interface for emc
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

#include "hidkbdevice.h"
#include "usages.h"

HIDKBDevice::HIDKBDevice(HIDDevice *pDevice, HID_CollectionPath_t *pCol)
: m_Logger(QCoreApplication::applicationName(), "HIDLCDDevice" )
, m_pDevice( pDevice )
, m_pKBCol( pCol )
, m_bInitialised( false )
{
}

HIDKBDevice::~HIDKBDevice(void)
{
}

bool HIDKBDevice::Init()
{
    bool bFailed = !FindModifiers();
    bFailed = bFailed || !FindKeys();
    m_bInitialised = !bFailed;
    return m_bInitialised;
}

bool HIDKBDevice::FindKeys()
{
    // Keyboard keys are an array of input values.  Unlike a variables, where each variable contains
    // the status of a control (each bit contains the state of a key), the array contains the status
    // of at max. n controls at a time (n keys pressed).

    // We need to find the array that stores the keystrokes
    for ( unsigned int i = 0; i < m_pKBCol->ReportItems.size(); i++ )
    {
	HID_ReportItem_t *item = m_pKBCol->ReportItems[i];
	if ( item->ItemType == REPORT_ITEM_TYPE_In &&
	     item->Attributes.UsagePage == USAGEPAGE_KEYBOARD_KEYPAD &&
	     (item->ItemFlags & IOF_VARIABLE) == 0 &&
	     (item->ItemFlags & IOF_CONSTANT) == 0 )
	{
	    m_keys.append( item );
	}

    }
    return true;
}

bool HIDKBDevice::FindModifiers()
{
    // I'm not sure what the best way of handling this is.  Modifier keys will appear as input bits with
    // usages of KEYBOARD_LEFTCONTROL to KEYBOARD_RIGHT.  For now, I'll just search for any bit with
    // usagepage = Keyboard, and usages in the above range.

    for ( unsigned int i = 0; i < m_pKBCol->ReportItems.size(); i++ )
    {
	HID_ReportItem_t *item = m_pKBCol->ReportItems[i];
	if ( item->ItemType == REPORT_ITEM_TYPE_In &&
	     item->Attributes.UsagePage == USAGEPAGE_KEYBOARD_KEYPAD &&
	     item->Attributes.Usage >= KEYBOARD_LEFTCONTROL &&
	     item->Attributes.Usage <= KEYBOARD_RIGHT )
	{
	    m_modifiers.append( item );
	}
    }
    return true;
}

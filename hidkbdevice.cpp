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

HIDKBDevice::HIDKBDevice(HID_CollectionPath_t *pCol)
: m_Logger(QCoreApplication::applicationName(), "HIDKBDevice" )
, m_bInitialised( false )
, m_pKBCol( pCol )
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
    // Keyboard keys are an array of input values.  Unlike variables, where each variable contains
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
    LOG_MSG( m_Logger, LogTypes::Error, QString("Found %1 key slots\n").arg(m_keys.count()) );
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
            m_modifiers[item->Attributes.Usage] = item;
            m_ModifiersDown[item->Attributes.Usage] = false;
	}
    }
    return true;
}


// Read the keyboard data
bool HIDKBDevice::ProcessKeyboardData()
{
    bool bChanged = false;
    QVector<int> keysdown;

    for ( int i = 0; i < m_keys.count(); i++  )
    {
        HID_ReportItem_t *key_item = m_keys[i];

        if ( key_item->Value == KEYBOARD_ERRORROLLOVER )
        {
            // Too many keys down.   Just wait for the next message.
        }
        else if ( key_item->Value >= KEYBOARD_A )
        {
            keysdown.append( key_item->Value );
        }
    }
    qSort( keysdown );

    if ( keysdown.count() != m_KeysDown.count() )
        bChanged = true;
    else
    {
        for ( int i = 0; i < keysdown.count(); i++)
        {
            if ( keysdown[i] != m_KeysDown[i] )
            {
                bChanged =true;
                break;
            }
        }
    }

    if ( bChanged )
    {
        m_KeysDown = keysdown;
    }

    foreach ( HID_ReportItem_t *modifier_item, m_modifiers  )
    {
        if ( (modifier_item->Value && !m_ModifiersDown[modifier_item->Attributes.Usage]) ||
             (!modifier_item->Value && m_ModifiersDown[modifier_item->Attributes.Usage]) )
        {
            // Change
            m_ModifiersDown[modifier_item->Attributes.Usage] = modifier_item->Value;
            bChanged = true;
        }
    }

    return bChanged;
}

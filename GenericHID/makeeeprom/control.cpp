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
#include "control.h"
#include "controlanalogencoder.h"
#include "controlbicolourled.h"
#include "controlcounter.h"
#include "controldigitalencoder.h"
#include "controldirectionalswitch.h"
#include "controlkeymatrix.h"
#include "controllcd.h"
#include "controllcd_spi.h"
#include "controlled.h"
#include "controlpotentiometer.h"
#include "controlpwm.h"
#include "controlrgbled.h"
#include "controlrotaryswitch.h"
#include "controlswitch.h"
#include "controltricolourled.h"

Control::Control(void)
{
}

Control::~Control(void)
{
}


Control *Control::MakeControl( const QDomElement &elem, QString *sError )
{
    Control *pControl = NULL;
    if ( elem.nodeName().compare( "DirectionalSwitch", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlDirectionalSwitch();
    else if ( elem.nodeName().compare( "AnalogIn", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlPotentiometer();
    else if ( elem.nodeName().compare( "Switch", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlSwitch();
    else if ( elem.nodeName().compare( "TricolourLED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlTriColourLED();
    else if ( elem.nodeName().compare( "DigitalEncoder", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlDigitalEncoder();
    else if ( elem.nodeName().compare( "KeyMatrix", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlKeyMatrix();
    else if ( elem.nodeName().compare( "RotarySwitch", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlRotarySwitch();
    else if ( elem.nodeName().compare( "LCD", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlLCD();
    else if ( elem.nodeName().compare( "LCD_SPI", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlLCDSPI();
    else if ( elem.nodeName().compare( "LED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlLED();
    else if ( elem.nodeName().compare( "BicolourLED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlBiColourLED();
    else if ( elem.nodeName().compare( "RGBLED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlRGBLED();
    else if ( elem.nodeName().compare( "PWM", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlPWM();
    else if ( elem.nodeName().compare( "Counter", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlCounter();
    else
    {
	if ( sError != NULL )
	    *sError = QString( "Unknown node type <%1>").arg(elem.nodeName());
    }

    if ( pControl != NULL )
    {
	if ( !pControl->Load( elem, sError ) )
	{
	    delete pControl;
	    pControl = NULL;
	}
    }

    return pControl;
}


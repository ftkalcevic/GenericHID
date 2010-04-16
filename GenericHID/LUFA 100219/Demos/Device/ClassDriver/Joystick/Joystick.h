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



/*
MyUSB Library
Copyright (C) Dean Camera, 2007.

dean [at] fourwalledcubicle [dot] com
www.fourwalledcubicle.com

Released under the GPL Licence, Version 3
*/

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

/* Includes: */
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "Descriptors.h"

#include <LUFA/Drivers/USB/USB.h>            // USB Functionality
#include <LUFA/Scheduler/Scheduler.h>        // Simple scheduler for task management
#include "USBKey_Bicolour.h"

/* Task Definitions: */
//TASK(USB_Joystick_Report);

/* Event Handlers: */
//HANDLES_EVENT(USB_CreateEndpoints);


extern volatile uint16_t milliseconds;				// use to count 1000ms to clock 1 sec.
extern volatile uint8_t seconds_counter;			// second counter
extern volatile uint8_t milliseconds_counter;		// msec counter - wraps at 0xFF
extern volatile uint16_t milliseconds_counter16;	// msec counter - wraps at 0xFFFF
extern byte DynamicHIDData[];
extern byte bSerialDebug;

extern uint16_t g_nCounterTop;
extern uint16_t g_nCounterCount;
extern volatile uint16_t g_nCounterCounter;
extern uint16_t g_nCounterMask;


#endif

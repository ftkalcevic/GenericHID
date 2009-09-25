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

#include <MyUSB/Common/ButtLoadTag.h>         // PROGMEM tags readable by the ButtLoad project
#include <MyUSB/Drivers/USB/USB.h>            // USB Functionality
#include <MyUSB/Drivers/USBKEY/Bicolour.h>    // Bicolour LEDs driver for the USBKEY
#include <MyUSB/Scheduler/Scheduler.h>        // Simple scheduler for task management

/* Task Definitions: */
TASK(USB_Joystick_Report);

/* Event Handlers: */
HANDLES_EVENT(USB_CreateEndpoints);


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

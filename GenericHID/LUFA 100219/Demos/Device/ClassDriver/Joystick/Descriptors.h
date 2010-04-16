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

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

/* Includes: */
#include <avr/pgmspace.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/USB/Class/HID.h>


/* Type Defines: */
/* Macros: */
#define JOYSTICK_IN_EPNUM            1
#define JOYSTICK_OUT_EPNUM           2
#define JOYSTICK_IN_EPSIZE           64
#define JOYSTICK_OUT_EPSIZE          64

#define DTYPE_HID                    0x21
#define DTYPE_Report                 0x22

#define JOYSTICK_INTERFACE_NUMBER    0
#define JOYSTICK_INTERFACE_ALTERNATE 0
#define JOYSTICK_INTERFACE_ENDPOINTS 2
#define JOYSTICK_INTERFACE_CLASS     0x03
#define JOYSTICK_INTERFACE_SUBCLASS  0x00
#define JOYSTICK_INTERFACE_PROTOCOL  0x00

/* Function Prototypes: */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex, void** const DescriptorAddress)
                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif

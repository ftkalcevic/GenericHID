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
#include <MyUSB/Drivers/USB/USB.h>

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
bool USB_GetDescriptor(const uint8_t Type, const uint8_t Index,
		       void** const DescriptorAddr, uint16_t* const Size)
		       ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3, 4);

#endif

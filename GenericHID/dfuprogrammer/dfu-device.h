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

#ifndef __DFU_DEVICE_H__
#define __DFU_DEVICE_H__

#include "inttypes.h"
#include "usb.h"

enum atmel_device_class_t
{
    adc_8051,
    adc_AVR,
    adc_AVR32
};

struct dfu_device_t 
{
    struct usb_dev_handle *handle;
    int32_t interface;
    atmel_device_class_t type;
};

#endif

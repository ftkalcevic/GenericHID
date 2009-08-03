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

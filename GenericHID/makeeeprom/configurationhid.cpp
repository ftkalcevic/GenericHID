#include "stdafx.h"
#include "configurationhid.h"

ConfigurationHID::ConfigurationHID(int nDescriptorReportLength)
: m_nDescriptorReportLength(nDescriptorReportLength)
{
}

ConfigurationHID::~ConfigurationHID(void)
{
}


ByteArray ConfigurationHID::GetReportDescriptor(StringTable & /*table*/) const
{
    ByteBuffer HIDDescriptor;

    HIDDescriptor.AddByte(0);	    // Length
    HIDDescriptor.AddByte(0x21);    // HID Type
    HIDDescriptor.AddShort(0x1001); // HID Class specification release
    HIDDescriptor.AddByte(0);	    // Country code
    HIDDescriptor.AddByte(1);	    // Number of descriptors (HID report)
    HIDDescriptor.AddByte(0x22);    // HID report type
    HIDDescriptor.AddShort((uint16_t)m_nDescriptorReportLength);      // HID report length
    HIDDescriptor[0] = (byte)HIDDescriptor.count();

    return HIDDescriptor;
}
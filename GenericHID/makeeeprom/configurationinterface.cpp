#include "stdafx.h"
#include "configurationinterface.h"

ConfigurationInterface::ConfigurationInterface(void)
{
}

ConfigurationInterface::~ConfigurationInterface(void)
{
}



ByteArray ConfigurationInterface::GetReportDescriptor(StringTable & /*table*/) const
{
    ByteBuffer InterfaceDescriptor;

    InterfaceDescriptor.AddByte(0);      // Length
    InterfaceDescriptor.AddByte(4);      // INTERFACE Type
    InterfaceDescriptor.AddByte(0);      // Interface number
    InterfaceDescriptor.AddByte(0);      // Alternate Setting
    InterfaceDescriptor.AddByte(2);      // Total endpoints - always 2 = 1 input + 1 output
    InterfaceDescriptor.AddByte(3);      // Class
    InterfaceDescriptor.AddByte(0);      // subclass
    InterfaceDescriptor.AddByte(0);      // protocol
    InterfaceDescriptor.AddByte(0);      // Interface String 
    InterfaceDescriptor[0] = (byte)InterfaceDescriptor.count();

    return InterfaceDescriptor;
}


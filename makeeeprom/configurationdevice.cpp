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
#include "configurationdevice.h"

ConfigurationDevice::ConfigurationDevice(void)
{
}

ConfigurationDevice::~ConfigurationDevice(void)
{
}

bool ConfigurationDevice::Load( const QDomElement &elem, QString *sError )
{
    if ( !XMLUtility::getAttributeUShort( elem, "VID", m_nVID, 0, 0xFFFF, sError ) )
        return false;
    if ( !XMLUtility::getAttributeUShort( elem, "PID", m_nPID, 0, 0xFFFF, sError ) )
        return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Release", m_nRelease, 0, 0xFFFF, sError ) )
        return false;
    if ( !XMLUtility::getAttributeString( elem, "Manufacturer", m_sManufacturer, sError ) )
        return false;
    if ( !XMLUtility::getAttributeString( elem, "Product", m_sProduct, sError ) )
        return false;
    if ( !XMLUtility::getAttributeString( elem, "SerialNo", m_sSerialNo, sError ) )
        return false;
    return true;
}

ByteArray ConfigurationDevice::GetReportDescriptor(StringTable &table) const
{
    ByteBuffer DeviceDescriptor;
    
    DeviceDescriptor.AddByte( 0 );		// Length
    DeviceDescriptor.AddByte(1);		// DEVICE Type
    DeviceDescriptor.AddShort( 0x0110 );	// USB Specification version
    DeviceDescriptor.AddByte( 0 );		// Class (defined in interface for HID devices)
    DeviceDescriptor.AddByte(0);		// SubClass (defined in interface for HID devices)
    DeviceDescriptor.AddByte(0);		// Protocol (defined in interface for HID devices)
    DeviceDescriptor.AddByte(0x40);		// Max packet size for endpoint 0
    DeviceDescriptor.AddUShort( m_nVID );	// Vendor ID
    DeviceDescriptor.AddUShort( m_nPID );	// Product ID
    DeviceDescriptor.AddUShort( m_nRelease );   // Release Number
    DeviceDescriptor.AddByte( table[m_sManufacturer] );   // Manufacturer string index
    DeviceDescriptor.AddByte( table[m_sProduct] );        // Product string index
    DeviceDescriptor.AddByte( table[m_sSerialNo] );       // Serial Number string index
    DeviceDescriptor.AddByte(1);		// The number of configurations.
    DeviceDescriptor[0] = (byte)DeviceDescriptor.count();
    
    return DeviceDescriptor;
}

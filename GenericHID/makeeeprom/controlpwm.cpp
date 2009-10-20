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
#include "controlpwm.h"

ControlPWM::ControlPWM(void)
{
}

ControlPWM::~ControlPWM(void)
{
}

bool ControlPWM::Load( const QDomElement &elem, QString *sError )
{
    if ( !GetPort( elem, "Port", m_nPort, sError ) )
	return false;
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Resolution", m_nResolution, 1, 0xFFFF, sError ) )
	return false;

    m_nBits = 0;
    int nTemp = m_nResolution;
    while ( nTemp != 0 )
    {
	m_nBits++;
	nTemp >>= 1;
    }

    return true;
}

ByteArray ControlPWM::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(m_nResolution);
    Desc.ReportSize(m_nBits);
    Desc.ReportCount(1);
    nBits += m_nBits;
    if (m_sName.length() > 0)
        Desc.StringIndex(table[m_sName]);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlPWM::GetControlConfig( byte nReportId ) const
{
    struct SPWMControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = PWMOutput;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.Port = (byte)m_nPort;			// Port = Portn * 8 + Pin
    config.Bits = m_nBits;				// number of bits the value occupies in the report
    config.Resolution = m_nResolution;			// top value (used to validate).

    return ByteBuffer((byte *)&config, sizeof(config) );
}


/*

    PB5	    OC.1A
    PB6	    OC.1B
    PB7	    OC.1C
    PB4	    OC.2A
    PD1	    OC.2B
    PC4	    OC.3C
    PC5	    OC.3B
    PC6	    OC.3A
*/

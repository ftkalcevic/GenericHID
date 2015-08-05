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
#include "controldigitalencoder.h"

ControlDigitalEncoder::ControlDigitalEncoder(void)
{
}

ControlDigitalEncoder::~ControlDigitalEncoder(void)
{
}

bool ControlDigitalEncoder::Load( const QDomElement &elem, QString *sError )
{
    if ( !GetPort( elem, "PortA", m_nPortA, sError ) )
	return false;
    if ( !GetPort( elem, "PortB", m_nPortB, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUInt( elem, "Bits", m_nBits, 1, 16, sError ) )
	return false;
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeBool( elem, "Pullup", m_bPullup, sError ) )
	return false;
    QString sOutput;
    if ( !XMLUtility::getAttributeString( elem, "Output", sOutput, sError ) )
	return false;
    if ( sOutput == "Absolute" )
	m_bAbsoluteOutput = true;
    else
	m_bAbsoluteOutput = false;
    return true;
}


ByteArray ControlDigitalEncoder::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    if ( m_bAbsoluteOutput )
    {
        Desc.LogicalMinimum(0);
        Desc.LogicalMaximum((1<<m_nBits)-1);
    }
    else
    {
        Desc.LogicalMinimum(-((1<<(m_nBits-1))-1));
        Desc.LogicalMaximum((1<<(m_nBits-1)));
    }
    Desc.ReportSize(m_nBits);
    Desc.ReportCount(1);
    nBits += m_nBits;
    if (!m_sName.isEmpty())
        Desc.StringIndex(table[m_sName]);
    if ( m_bAbsoluteOutput )
        Desc.Input(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::Wrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EBufferType::BitField);
    else
        Desc.Input(EDataType::Data, EVarType::Variable, ERelType::Relative, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EBufferType::BitField);

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlDigitalEncoder::GetControlConfig( byte nReportId ) const
{
    struct SDigitalEncoderControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = DigitalEncoder;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.PortA = (byte)m_nPortA;
    config.PortB = (byte)m_nPortB;
    config.Bits = (byte)m_nBits;
    config.Options = (m_bAbsoluteOutput ? (1<<DE_ABSOLUTE) : 0) |
                     (m_bPullup ? (1<<DE_PULLUP) : 0 );

    return ByteBuffer((byte *)&config, sizeof(config) );
}

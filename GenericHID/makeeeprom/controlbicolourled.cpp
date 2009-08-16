#include "stdafx.h"
#include "controlbicolourled.h"

ControlBiColourLED::ControlBiColourLED(void)
{
}

ControlBiColourLED::~ControlBiColourLED(void)
{
}

bool ControlBiColourLED::Load( const QDomElement &elem, QString *sError )
{
    if ( !GetPort( elem, "PortA", m_nPortA, sError ) )
	return false;
    if ( !GetPort( elem, "PortB", m_nPortB, sError ) )
	return false;
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    return true;
}

ByteArray ControlBiColourLED::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(2);
    Desc.ReportSize(2);
    Desc.ReportCount(1);
    nBits += 2;
    if (m_sName.length() > 0)
        Desc.StringIndex(table[m_sName]);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlBiColourLED::GetControlConfig( byte nReportId ) const
{
    struct SBicolourLEDControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = BicolourLED;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.PortA = (byte)m_nPortA;
    config.PortB = (byte)m_nPortB;

    return ByteBuffer((byte *)&config, sizeof(config) );
}

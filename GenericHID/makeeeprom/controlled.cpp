#include "stdafx.h"
#include "controlled.h"

ControlLED::ControlLED(void)
{
}

ControlLED::~ControlLED(void)
{
}

bool ControlLED::Load( const QDomElement &elem, QString *sError )
{
    if ( !GetPort( elem, "Port", m_nPort, sError ) )
	return false;
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeBool( elem, "Source", m_bSink, sError ) )
	return false;
    m_bSink = !m_bSink;
    return true;
}

ByteArray ControlLED::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(1);
    Desc.ReportSize(1);
    Desc.ReportCount(1);
    nBits += 1;
    if ( !m_sName.isEmpty() )
        Desc.StringIndex(table[m_sName]);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlLED::GetControlConfig( byte nReportId ) const
{
    struct SLEDControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = LED;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.Port = (byte)m_nPort;
    config.Options = (m_bSink ? (1<<LED_SINK) : 0 );

    return ByteBuffer((byte *)&config, sizeof(config) );
}

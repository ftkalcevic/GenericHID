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
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    return true;
}


ByteArray ControlDigitalEncoder::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(255);
    Desc.ReportSize(8);
    Desc.ReportCount(1);
    nBits += 8;
    if (!m_sName.isEmpty())
        Desc.StringIndex(table[m_sName]);
    Desc.Input(EDataType::Data, EVarType::Variable, ERelType::Relative, EWrapType::Wrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EBufferType::BitField);

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

    return ByteBuffer((byte *)&config, sizeof(config) );
}

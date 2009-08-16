#include "stdafx.h"
#include "controlcounter.h"

ControlCounter::ControlCounter(void)
{
}

ControlCounter::~ControlCounter(void)
{
}

bool ControlCounter::Load( const QDomElement &elem, QString *sError )
{
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Period", m_nPeriod, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeByte( elem, "Bits", m_nBits, 1, 16, sError ) )
	return false;

    return true;
}

ByteArray ControlCounter::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum( (1<<m_nBits) - 1 );
    Desc.ReportSize(m_nBits);
    nBits += m_nBits;
    Desc.ReportCount(1);
    if ( !m_sName.isEmpty() )
        Desc.StringIndex(table[m_sName]);
    Desc.Input(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::Wrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EBufferType::BitField);

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlCounter::GetControlConfig( byte nReportId ) const
{
    struct SCounterControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = Counter;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.Period = m_nPeriod;
    config.Bits = m_nBits;

    return ByteBuffer((byte *)&config, sizeof(config) );
}

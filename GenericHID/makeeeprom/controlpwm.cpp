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
    if ( !XMLUtility::getAttributeUInt( elem, "Period", m_nPeriod, 0, 0xFFFFFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeByte( elem, "Resolution", m_nResolution, 1, 16, sError ) )
	return false;
    return true;
}

ByteArray ControlPWM::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(1<<m_nResolution);
    Desc.ReportSize(m_nResolution);
    Desc.ReportCount(1);
    nBits += m_nResolution;
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

    config.hdr.Type = BicolourLED;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.Port = (byte)m_nPort;
    config.Period = m_nPeriod;
    config.Resolution = m_nResolution;

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

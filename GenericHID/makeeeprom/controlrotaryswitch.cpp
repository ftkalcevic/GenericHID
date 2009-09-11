#include "stdafx.h"
#include "controlrotaryswitch.h"

ControlRotarySwitch::ControlRotarySwitch(void)
{
}

ControlRotarySwitch::~ControlRotarySwitch(void)
{
}



bool ControlRotarySwitch::Load( const QDomElement &elem, QString *sError )
{
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeBool( elem, "Pullup", m_bPullup, sError ) )
	return false;
    if ( !XMLUtility::getAttributeBool( elem, "Debounce", m_bDebounce, sError ) )
	return false;
    if ( !XMLUtility::getAttributeBool( elem, "Encoded", m_bEncoded, sError ) )
	return false;
    if ( m_bEncoded )
	if ( !XMLUtility::getAttributeUShort( elem, "Outputs", m_nOutputs, 0, 16, sError ) )
	    return false;

    QDomNodeList pins = XMLUtility::elementsByTagName( elem, "Input" );
    for ( int i = 0; i < pins.count(); i++ )
    {
	QDomElement pin = pins.item(i).toElement();
	byte nPort;
	if ( !GetPort( pin, "Port", nPort, sError ) )
	    return false;
	byte nBit;
	if ( !XMLUtility::getAttributeByte( pin, "Bit", nBit, 0, 255, sError ) )
	    return false;
	m_Pins.push_back( RotarySwitchPin(nPort,nBit) );
    }

    m_nLogicalMax = 0;
    if ( m_bEncoded )
        m_nLogicalMax = m_nOutputs-1;       // binary encoded
    else
        m_nLogicalMax = m_Pins.count() - 1;        // 0..n-1

    // count max bits
    m_nReportBits = 0;
    int nTemp = m_nLogicalMax;
    while (nTemp != 0)
    {
        m_nReportBits++;
        nTemp >>= 1;
    }

    return true;
}


ByteArray ControlRotarySwitch::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(m_nLogicalMax);
    Desc.ReportSize(m_nReportBits);
    nBits += (int)m_nReportBits;
    Desc.ReportCount(1);
    if ( !m_sName.isEmpty() )
        Desc.StringIndex(table[m_sName]);

    Desc.Input(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EBufferType::BitField);

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlRotarySwitch::GetControlConfig( byte nReportId ) const
{
    int nBufSize = sizeof(struct SRotarySwitchControl) + sizeof(struct SRotarySwitchControl::_SRotarySwitchPin) * m_Pins.count();
    byte *pBuf = new byte [ nBufSize ];
    memset( pBuf, 0, nBufSize );
    struct SRotarySwitchControl &config = *reinterpret_cast<struct SRotarySwitchControl *>( pBuf );

    config.hdr.Type = RotarySwitch;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = (byte)nBufSize;
    config.PinCount = (byte)m_Pins.count();
    config.ReportSize = (byte)m_nReportBits;
    config.Options = (m_bPullup ? (1<<RSW_PULLUP) : 0 ) |
		     (m_bDebounce ? (1<<RSW_DEBOUNCE) : 0 ) |
		     (m_bEncoded ? (1<<RSW_ENCODED) : 0 );
    config.LastValue = 0;

    for ( int i = 0; i < m_Pins.count(); i++ )
    {
	config.Pins[i].Port = m_Pins[i].m_nPort;
	if ( m_bEncoded )
	    config.Pins[i].Bit = m_Pins[i].m_nBit;	    // Data Bit mask
	else
	    config.Pins[i].Bit = m_Pins[i].m_nBit-1;
	config.Pins[i].Debounce = 0;
    }

    return ByteBuffer((byte *)&config, nBufSize );
}

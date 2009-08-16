#include "stdafx.h"
#include "controllcd.h"
#include "usages.h"

ControlLCD::ControlLCD(void)
{
}

ControlLCD::~ControlLCD(void)
{
}

bool ControlLCD::Load( const QDomElement &elem, QString *sError )
{
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeByte( elem, "Rows", m_nRows, 1, 4, sError ) )
	return false;
    if ( !XMLUtility::getAttributeByte( elem, "Columns", m_nCols, 1, 40, sError ) )
	return false;
    if ( !XMLUtility::getAttributeBool( elem, "Bus8Bit", m_b8Bit, sError ) )
	return false;
    if ( !GetPort( elem, "PortRS", m_nPortRS, sError ) )
	return false;
    if ( !GetPort( elem, "PortRW", m_nPortRW, sError ) )
	return false;
    if ( !GetPort( elem, "PortE", m_nPortE, sError ) )
	return false;
    if ( m_b8Bit )
    {
	if ( !GetPort( elem, "PortD0", m_nPortD0, sError ) )
	    return false;
	if ( !GetPort( elem, "PortD1", m_nPortD1, sError ) )
	    return false;
	if ( !GetPort( elem, "PortD2", m_nPortD2, sError ) )
	    return false;
	if ( !GetPort( elem, "PortD3", m_nPortD3, sError ) )
	    return false;
    }
    if ( !GetPort( elem, "PortD4", m_nPortD4, sError ) )
	return false;
    if ( !GetPort( elem, "PortD5", m_nPortD5, sError ) )
	return false;
    if ( !GetPort( elem, "PortD6", m_nPortD6, sError ) )
	return false;
    if ( !GetPort( elem, "PortD7", m_nPortD7, sError ) )
	return false;

    switch ( m_nRows )
    {
	case 4: if ( !XMLUtility::getAttributeByte( elem, "Row3", m_nAddrRow3, 0, 0xFF, sError ) ) return false;
	case 3: if ( !XMLUtility::getAttributeByte( elem, "Row2", m_nAddrRow2, 0, 0xFF, sError ) ) return false;
	case 2: if ( !XMLUtility::getAttributeByte( elem, "Row1", m_nAddrRow1, 0, 0xFF, sError ) ) return false;
	case 1: if ( !XMLUtility::getAttributeByte( elem, "Row0", m_nAddrRow0, 0, 0xFF, sError ) ) return false;
    }

    return true;
}


ByteArray ControlLCD::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    // The report to display text - row, col, data
    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    if (!m_sName.isEmpty())
	Desc.StringIndex(table[m_sName]);
    Desc.Collection(CollectionType::Logical);

    Desc.Usage(USAGE_COLUMN);                       // column
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(m_nCols-1);
    Desc.ReportSize(8);
    Desc.ReportCount(1);
    nBits += 8;
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

    Desc.Usage(USAGE_ROW);                       // row
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(m_nRows - 1);
    Desc.ReportSize(8);
    Desc.ReportCount(1);
    nBits += 8;
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

    Desc.Usage(USAGE_DISPLAY_DATA);                       // row data
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(255);
    Desc.ReportSize(8);
    Desc.ReportCount(m_nCols);
    nBits += 8 * m_nCols;
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::Buffered);


    // row, col, attributes
    Desc.UsagePage(USAGEPAGE_ALPHANUMERIC_DISPLAY);	// alpha numeric display
    Desc.Usage(USAGE_DISPLAY_ATTRIBUTES_REPORT);        // display attributes report
    Desc.Collection(CollectionType::Logical);
    Desc.Usage(USAGE_ROWS);				// Rows
    Desc.Usage(USAGE_COLUMNS);				// Columns
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(255);
    Desc.ReportSize(8);
    Desc.ReportCount(2);
    Desc.Feature(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
    Desc.EndCollection();

    Desc.EndCollection();

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlLCD::GetControlConfig( byte nReportId ) const
{
    struct SLCDControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = LCD;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.nRows = m_nRows;
    config.nColumns = m_nCols;
    config.b8Bit = m_b8Bit;
    config.nPortRS = m_nPortRS;
    config.nPortRW = m_nPortRW;
    config.nPortE = m_nPortE;
    config.nPortD0 = m_nPortD0;
    config.nPortD1 = m_nPortD1;
    config.nPortD2 = m_nPortD2;
    config.nPortD3 = m_nPortD3;
    config.nPortD4 = m_nPortD4;
    config.nPortD5 = m_nPortD5;
    config.nPortD6 = m_nPortD6;
    config.nPortD7 = m_nPortD7;
    config.RowAddr[0] = m_nAddrRow0;
    config.RowAddr[1] = m_nAddrRow1;
    config.RowAddr[2] = m_nAddrRow2;
    config.RowAddr[3] = m_nAddrRow3;

    return ByteBuffer((byte *)&config, sizeof(config) );
}

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
    if ( !XMLUtility::getAttributeByte( elem, "FunctionSet", m_nFunctionSet, 0, 7, sError ) )
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
    else
    {
	m_nPortD0 = 0;
	m_nPortD1 = 0;
	m_nPortD2 = 0;
	m_nPortD3 = 0;
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


ByteArray ControlLCD::GetHIDReportDescriptor( StringTable &table, byte &nReportId, byte OutputReportLength[MAX_REPORTS], byte &nMaxOutReportLen ) const
{
    int nBits = 0;

    HIDReportDescriptorBuilder Desc;

    Desc.ReportID(nReportId+LCD_DISPLAY_REPORT_ID);
    Desc.UsagePage(USAGEPAGE_ALPHANUMERIC_DISPLAY);
    Desc.Usage(USAGE_ALPHANUMERIC_DISPLAY);
    if (!m_sName.isEmpty())
	Desc.StringIndex(table[m_sName]);
    Desc.Collection(CollectionType::Logical);

    // The report to display text - row, col, data
    Desc.Usage(USAGE_CHARACTER_REPORT);
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
    Desc.EndCollection();


    // row, col, attributes
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

    // Font change report
    Desc.ReportID(nReportId+LCD_FONT_REPORT_ID);
    Desc.Usage(USAGE_FONT_REPORT);
    Desc.Collection(CollectionType::Logical);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(7);
    Desc.ReportSize(8);
    Desc.ReportCount(1);
    Desc.Usage(USAGE_DISPLAY_DATA);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField );
    Desc.ReportCount(5);	    //	Assumes a 5x7 font, 35 bits
    Desc.Usage(USAGE_FONT_DATA);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::Buffered );	//Font data
    Desc.EndCollection();

    // Cursor position report
    Desc.ReportID(nReportId+LCD_CURSOR_POSITION_REPORT_ID);
    Desc.Usage(USAGE_CURSOR_POSITION_REPORT);
    Desc.Collection(CollectionType::Logical);
    Desc.Usage(USAGE_COLUMN);                       // column
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(m_nCols-1);
    Desc.ReportSize(8);
    Desc.ReportCount(1);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
    Desc.Usage(USAGE_ROW);                       // row
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(m_nRows - 1);
    Desc.ReportSize(8);
    Desc.ReportCount(1);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(1);
    Desc.ReportSize(1);
    Desc.ReportCount(1);
    Desc.Usage(USAGE_CURSOR_ENABLE);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField );
    Desc.Usage(USAGE_CURSOR_BLINK);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField );
    Desc.ReportSize(6);
    Desc.ReportCount(1);
    Desc.Output(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
    Desc.EndCollection();

    // Function set
    Desc.ReportID(nReportId+LCD_FUNCTION_SET_REPORT_ID);
    Desc.Usage(USAGE_FUNCTION_SET_REPORT);
    Desc.Collection(CollectionType::Logical);
    Desc.Usage(USAGE_FUNCTION_SET);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(7);
    Desc.ReportSize(3);
    Desc.ReportCount(1);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
    Desc.Usage(0);          // Padding
    Desc.ReportSize(5);
    Desc.ReportCount(1);
    Desc.Output(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
    Desc.EndCollection();

    Desc.EndCollection();

    byte nLength = (byte)((nBits + 7) / 8);
    OutputReportLength[nReportId+LCD_DISPLAY_REPORT_ID-1] = nLength;
    OutputReportLength[nReportId+LCD_FONT_REPORT_ID-1] = 6;
    OutputReportLength[nReportId+LCD_CURSOR_POSITION_REPORT_ID-1] = 3;
    OutputReportLength[nReportId+LCD_FUNCTION_SET_REPORT_ID-1] = 1;
    nMaxOutReportLen = MAX( nMaxOutReportLen, nLength );
    nMaxOutReportLen = MAX( nMaxOutReportLen, 6 );
    nReportId += 4;

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlLCD::GetControlConfig( byte nReportId ) const
{
    struct SLCDControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = LCD;
    config.hdr.ReportId = nReportId;
    config.hdr.ReportIdMax = nReportId + LCD_FUNCTION_SET_REPORT_ID;
    config.hdr.Length = sizeof(config);
    config.nRows = m_nRows;
    config.nColumns = m_nCols;
    config.b8Bit = m_b8Bit;
    config.nFunctionSet = m_nFunctionSet;
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

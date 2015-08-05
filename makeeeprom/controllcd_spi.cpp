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



/*
    Configuration for the DIY graphics/alphanumeric display.

    All the data here is constant.  The device 
    uses the a90usb1287 SPI port, so the pins are fixed.
    It only works with the DIY device, which has fixed 
    resolution, fixed fonts, fixed commands.
*/

#include "stdafx.h"
#include "controllcd_spi.h"
#include "usages.h"

ControlLCDSPI::ControlLCDSPI(void)
{
}

ControlLCDSPI::~ControlLCDSPI(void)
{
}

bool ControlLCDSPI::Load( const QDomElement &elem, QString *sError )
{
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !GetPort( elem, "PortSS", m_nPortSS, sError ) )
	return false;
    if ( !GetPort( elem, "PortSCK", m_nPortSCK, sError ) )
	return false;
    if ( !GetPort( elem, "PortMOSI", m_nPortMOSI, sError ) )
	return false;
    if ( !GetPort( elem, "PortMISO", m_nPortMISO, sError ) )
	return false;

    m_nCols = 45;
    m_nRows = 17;

    return true;
}


ByteArray ControlLCDSPI::GetHIDReportDescriptor( StringTable &table, byte &nReportId, byte OutputReportLength[MAX_REPORTS], byte &nMaxOutReportLen ) const
{
    int nBits;
    byte nLength;

    int nPixelWidth = 320;
    int nPixelWidthBits = 9;
    int nPixelHeight = 240;
    int nPixelHeightBits = 8;

    byte nDefaultReportId = (byte)nReportId;
    byte nCharacterOutputReportId = nDefaultReportId;
    //byte nAttributeFeatureReportId = nDefaultReportId;
    byte nDisplayControlOutputReportId = nDefaultReportId + LCDSPI_DISPLAY_CONTROL_REPORT_ID;
    byte nFillRectOutputReportId = nDefaultReportId + LCDSPI_DRAW_RECT_REPORT_ID;

    HIDReportDescriptorBuilder Desc;

    Desc.ReportID(nDefaultReportId);
    Desc.UsagePage(USAGEPAGE_ALPHANUMERIC_DISPLAY);
    Desc.Usage(USAGE_ALPHANUMERIC_DISPLAY);
    if (!m_sName.isEmpty())
	Desc.StringIndex(table[m_sName]);
    Desc.Collection(CollectionType::Logical);

	// The report to set the cursor position and display text
	Desc.Usage(USAGE_CHARACTER_REPORT);
	Desc.Collection(CollectionType::Logical);

	    nBits = 0;

	    Desc.Usage(USAGE_COLUMN);                       // column
	    Desc.LogicalMinimum(0);
	    Desc.LogicalMaximum(nPixelWidth);
	    Desc.ReportSize(nPixelWidthBits);
	    Desc.ReportCount(1);
	    nBits += nPixelWidthBits;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_ROW);                       // row
	    Desc.LogicalMinimum(0);
	    Desc.LogicalMaximum(nPixelHeight - 1);
	    Desc.ReportSize(nPixelHeightBits);
	    Desc.ReportCount(1);
	    nBits += nPixelHeightBits;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_CURSOR_PIXEL_POSITIONING); // use pixel positioning
	    Desc.LogicalMaximum(1);
	    Desc.ReportSize(1);
	    Desc.ReportCount(1);
	    nBits += 1;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    // padding
	    uint nPad = (uint)(8 - (nBits % 8));
	    Desc.ReportCount(1);
	    Desc.ReportSize(nPad);
	    Desc.Output(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
	    nBits += nPad;

	    Desc.Usage(USAGE_DISPLAY_DATA);                       // row data
	    Desc.LogicalMinimum(0);
	    Desc.LogicalMaximum(255);
	    Desc.ReportSize(8);
	    Desc.ReportCount(m_nCols);
	    nBits += 8 * m_nCols;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::Buffered);

	    nLength = (byte)((nBits + 7) / 8);
	    OutputReportLength[nCharacterOutputReportId-1] = nLength;
	    nMaxOutReportLen = MAX( nMaxOutReportLen, nLength );

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

	// 
	Desc.Usage(USAGE_DISPLAY_CONTROL_REPORT);        // display control report
	Desc.Collection(CollectionType::Logical);

	    nBits = 0;
    
	    Desc.ReportID(nDisplayControlOutputReportId);

	    Desc.Usage(USAGE_CLEAR_DISPLAY); 
	    Desc.LogicalMaximum(1);
	    Desc.ReportSize(1);
	    Desc.ReportCount(1);
	    nBits += 1;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
	    
	    Desc.Usage(USAGE_DISPLAY_BRIGHTNESS); 
	    Desc.LogicalMaximum(100);
	    Desc.ReportSize(7);
	    Desc.ReportCount(1);
	    nBits += 7;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    // Custom items
	    Desc.Usage(USAGE_FOREGROUND_COLOUR); 
	    Desc.Usage(USAGE_BACKGROUND_COLOUR); 
	    Desc.ReportSize(16);
	    Desc.ReportCount(2);
	    nBits += 16*2;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_SELECT_FONT); 
	    Desc.ReportSize(1);			// only two fonts at the moment
	    Desc.ReportCount(1);
	    nBits += 1;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_CHAR_ATTR_UNDERLINE); 
	    Desc.ReportSize(1);			// only two fonts at the moment
	    Desc.ReportCount(1);
	    nBits += 1;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    // padding
	    nPad = (uint)(8 - (nBits % 8));
	    Desc.ReportCount(1);
	    Desc.ReportSize(nPad);
	    nBits += nPad;
	    Desc.Output(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    nLength = (byte)((nBits + 7) / 8);
	    OutputReportLength[nDisplayControlOutputReportId-1] = nLength;
	    nMaxOutReportLen = MAX( nMaxOutReportLen, nLength );

	Desc.EndCollection();

	// The report to draw a rectangle
	Desc.Usage(USAGE_DRAW_RECT_REPORT);
	Desc.Collection(CollectionType::Logical);

	    nBits = 0;
	    Desc.ReportID(nFillRectOutputReportId);

	    Desc.Usage(USAGE_RECT_FILLED);
	    Desc.ReportSize(1);
	    Desc.ReportCount(1);
	    nBits += 1;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_RECT_X);                       // x
	    Desc.LogicalMinimum(0);
	    Desc.LogicalMaximum(nPixelWidth - 1);
	    Desc.ReportSize(nPixelWidthBits);
	    Desc.ReportCount(1);
	    nBits += nPixelWidthBits;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_RECT_Y);                       // y
	    Desc.LogicalMinimum(0);
	    Desc.LogicalMaximum(nPixelHeight - 1);
	    Desc.ReportSize(nPixelHeightBits);
	    Desc.ReportCount(1);
	    nBits += nPixelHeightBits;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_RECT_WIDTH);                       // width
	    Desc.LogicalMinimum(0);
	    Desc.LogicalMaximum(nPixelWidth - 1);
	    Desc.ReportSize(nPixelWidthBits);
	    Desc.ReportCount(1);
	    nBits += nPixelWidthBits;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    Desc.Usage(USAGE_RECT_HEIGHT);                       // height
	    Desc.LogicalMinimum(0);
	    Desc.LogicalMaximum(nPixelHeight - 1);
	    Desc.ReportSize(nPixelHeightBits);
	    Desc.ReportCount(1);
	    nBits += nPixelHeightBits;
	    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    nPad = (uint)(8 - (nBits % 8));
	    Desc.ReportCount(1);
	    Desc.ReportSize(nPad);
	    nBits += nPad;
	    Desc.Output(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

	    nLength = (byte)((nBits + 7) / 8);
	    OutputReportLength[nFillRectOutputReportId-1] = nLength;
	    nMaxOutReportLen = MAX( nMaxOutReportLen, nLength );

	Desc.EndCollection();

    Desc.EndCollection();

    nReportId += 3;

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlLCDSPI::GetControlConfig( byte nReportId ) const
{
    struct SLCDSPIControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = LCD_SPI;
    config.hdr.ReportId = nReportId;
    config.hdr.ReportIdMax = nReportId + LCDSPI_DRAW_RECT_REPORT_ID;
    config.hdr.Length = sizeof(config);
    config.nRows = m_nRows;
    config.nColumns = m_nCols;
    config.nPortSS = m_nPortSS;	// this is pointless as there is only one SPI interface
    config.nPortSCK = m_nPortSCK;
    config.nPortMOSI = m_nPortMOSI;
    config.nPortMISO = m_nPortMISO;

    ByteBuffer retBuf((byte *)&config, sizeof(config) );

    return retBuf;
}


/*
// Standard display functions
Output Reports 
    - Set cursor report
	#define CMD_SET_TEXT_POSITION		0x84	    // Set position 0x04, x, y
	#define CMD_SET_GRAPHICS_POSITION	0x85	    // set position 0x05, xmsb, xlsb, y
    - Display data report
	#define CMD_WRITE_TEXT			0x83	    // Write Text - 0x03, len(?), data..., or
    - Display Control Report
	#define CMD_CLEAR_SCREEN		0x82	    // Clear Screen 0x02,
	- custom - #define CMD_SET_BACKGROUND_COLOUR	0x80	    // Set Background Colour 0x00, MSB, LSB
	- custom - #define CMD_SET_FOREGROUND_COLOUR	0x81	    // Set Foreground Colour 0x01, MSB, LSB
	- custom - #define CMD_SET_FONT			0x86	    // Set font? 0x05, 0/1
	#define CMD_SET_BACKLIGHT		0x87	    // set backlight 0x06, 0-100
	//#define CMD_SET_TEXT_ATTRIBUTE		0x88	    // set attribute 0x07, [underline][Reverse?bg/fg?][italic][bold][strikethrough]
	//#define     TEXT_ATTRIBUTE_UNDERLINE	    0x01
	//#define     TEXT_ATTRIBUTE_STRIKEOUT	    0x02
    - Draw Rectangle Report
	#define CMD_FILL_RECTANGLE		0x89	    // fill rect xlsb, xmsg, y, widthlsb, widthmsb, height
Feature Reports
    - Display attribute report
*/

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



#include "Common.h"
#include "Serial.h"
#include "Controls.h"
#include "LCD.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything

enum RegisterSelect
{
    Instr = 0,
    Data = 1
       };

enum ReadWrite
{
    Read = 1,
    Write = 0
        };

static void SetDataDirection( struct SLCDControl *pData, byte dir )
{
    if ( pData->b8Bit )
    {
        SetDDR( pData->nPortD0, dir );
        SetDDR( pData->nPortD1, dir );
        SetDDR( pData->nPortD2, dir );
        SetDDR( pData->nPortD3, dir );
    }
    SetDDR( pData->nPortD4, dir );
    SetDDR( pData->nPortD5, dir );
    SetDDR( pData->nPortD6, dir );
    SetDDR( pData->nPortD7, dir );
}

static void WriteLCDData4( struct SLCDControl *pData, enum RegisterSelect eRegSel, byte nData )
{
    SetDataDirection( pData, DDR_OUTPUT );
    SetPORT( pData->nPortRS, eRegSel );
    SetPORT( pData->nPortRW, Write );
    SetPORT( pData->nPortD4, nData & 0x01 );
    SetPORT( pData->nPortD5, nData & 0x02 );
    SetPORT( pData->nPortD6, nData & 0x04 );
    SetPORT( pData->nPortD7, nData & 0x08 );

    SetPORT( pData->nPortE, 1 );
    _delay_us( CPU_CLK, 1 );
    SetPORT( pData->nPortE, 0 );
}

static void WriteLCDData( struct SLCDControl *pData, enum RegisterSelect eRegSel, byte nData )
{
    if ( pData->b8Bit )
    {
        // Wait for the BF to disappear
        SetDataDirection( pData, DDR_INPUT );
        SetPORT( pData->nPortRS, Instr );
        SetPORT( pData->nPortRW, Read );

        byte bf = 1;
        byte nTimeout = 0xFF;
        while ( bf && nTimeout)
        {
            SetPORT( pData->nPortE, 1 );
            _delay_us( CPU_CLK, 1 );
            bf = GetPIN( pData->nPortD7 );
            SetPORT( pData->nPortE, 0 );

            nTimeout--;
        }

        SetDataDirection( pData, DDR_OUTPUT );
        SetPORT( pData->nPortRS, eRegSel );
        SetPORT( pData->nPortRW, Write );
        SetPORT( pData->nPortD0, nData & 0x01 );
        SetPORT( pData->nPortD1, nData & 0x02 );
        SetPORT( pData->nPortD2, nData & 0x04 );
        SetPORT( pData->nPortD3, nData & 0x08 );
        SetPORT( pData->nPortD4, nData & 0x10 );
        SetPORT( pData->nPortD5, nData & 0x20 );
        SetPORT( pData->nPortD6, nData & 0x40 );
        SetPORT( pData->nPortD7, nData & 0x80 );

        SetPORT( pData->nPortE, 1 );
        _delay_us( CPU_CLK, 1 );
        SetPORT( pData->nPortE, 0 );
    }
    else
    {
        // Wait for the BF to disappear
        SetDataDirection( pData, DDR_INPUT );
        SetPORT( pData->nPortRS, Instr );
        SetPORT( pData->nPortRW, Read );

        byte bf = 1;
        byte nTimeout = 0xFF;
        while ( bf && nTimeout)
        {
            SetPORT( pData->nPortE, 1 );
            _delay_us( CPU_CLK, 1 );
            bf = GetPIN( pData->nPortD7 );
            SetPORT( pData->nPortE, 0 );

            _delay_us( CPU_CLK, 1 );

            SetPORT( pData->nPortE, 1 );
            _delay_us( CPU_CLK, 1 );
            GetPIN( pData->nPortD7 );
            SetPORT( pData->nPortE, 0 );

            nTimeout--;
        }

        SetDataDirection( pData, DDR_OUTPUT );
        SetPORT( pData->nPortRS, eRegSel );
        SetPORT( pData->nPortRW, Write );
        SetPORT( pData->nPortD4, nData & 0x10 );
        SetPORT( pData->nPortD5, nData & 0x20 );
        SetPORT( pData->nPortD6, nData & 0x40 );
        SetPORT( pData->nPortD7, nData & 0x80 );

        SetPORT( pData->nPortE, 1 );
        _delay_us( CPU_CLK, 1 );
        SetPORT( pData->nPortE, 0 );
        _delay_us( CPU_CLK, 1 );

        SetPORT( pData->nPortD4, nData & 0x01 );
        SetPORT( pData->nPortD5, nData & 0x02 );
        SetPORT( pData->nPortD6, nData & 0x04 );
        SetPORT( pData->nPortD7, nData & 0x08 );

        SetPORT( pData->nPortE, 1 );
        _delay_us( CPU_CLK, 1 );
        SetPORT( pData->nPortE, 0 );
    }
}
static void Init8Bit(struct SLCDControl *pData)
{
    if ( nSerialDebugLevel > 0 )
        UART1_Send_P(PSTR("Initialising 8bit LCD\r\n"));

    // wait for the clock to hit > 45ms	- assume this is already done
    WriteLCDData( pData, Instr, 0x30 );		// set interface to 8 bits
    _delay_us( CPU_CLK, 5000 );			// wait 4.1ms or more
    WriteLCDData( pData, Instr, 0x30 );		// set interface to 8 bits
    _delay_us( CPU_CLK, 100 );			// wait 100us
    WriteLCDData( pData, Instr, 0x30 );		// set interface to 8 bits

    if ( pData->nRows > 1 )
        WriteLCDData( pData, Instr, 0x38 );	// Function set
    else
        WriteLCDData( pData, Instr, 0x30 );	// Function set

    WriteLCDData( pData, Instr, 0x08 );		// Display Off
    WriteLCDData( pData, Instr, 0x01 );		// Display Clear
    WriteLCDData( pData, Instr, 0x06 );		// Entry Mode Set
    WriteLCDData( pData, Instr, 0x0C );		// Display On
}

static void Init4Bit(struct SLCDControl *pData)
{
    if ( nSerialDebugLevel > 0 )
        UART1_Send_P(PSTR("Initialising 4bit LCD\r\n"));

    // wait for the clock to hit > 45ms	- assume this is already done
    WriteLCDData4( pData, Instr, 0x3 );		// set interface to 4 bits
    _delay_us( CPU_CLK, 5000 );
    WriteLCDData4( pData, Instr, 0x3 );		// set interface to 4 bits
    _delay_us( CPU_CLK, 100 );
    WriteLCDData4( pData, Instr, 0x3 );		// set interface to 4 bits
    WriteLCDData4( pData, Instr, 0x2 );		// set interface to 4 bits

    if ( pData->nRows > 1 )
        WriteLCDData( pData, Instr, 0x28 );		// Function set
    else
        WriteLCDData( pData, Instr, 0x20 );		// Function set

    WriteLCDData( pData, Instr, 0x08 );		// Display Off
    WriteLCDData( pData, Instr, 0x01 );		// Display Clear
    WriteLCDData( pData, Instr, 0x06 );		// Entry Mode Set
    WriteLCDData( pData, Instr, 0x0C );		// Display On
}

void InitLCD( struct SLCDControl *pData )
{
    if ( !bInitialised )
    {
        // wait for the first ms counter to get > 45.  This is to stablise power to the LCD.
        while ( milliseconds_counter < 45 )
            continue;
    }

    SetPORT( pData->nPortRS, 0 );
    SetPORT( pData->nPortRW, 0 );
    SetPORT( pData->nPortE, 0 );

    SetDataDirection( pData, DDR_INPUT );
    SetDDR( pData->nPortRS, DDR_OUTPUT );
    SetDDR( pData->nPortRW, DDR_OUTPUT );
    SetDDR( pData->nPortE, DDR_OUTPUT );

    if ( nSerialDebugLevel > 0 )
    {
        UART1_Send_P(PSTR("RS=")); UART1_SendHex(pData->nPortRS);
        UART1_Send_P(PSTR(" RW=")); UART1_SendHex(pData->nPortRW);
        UART1_Send_P(PSTR(" E=")); UART1_SendHex(pData->nPortE);
        UART1_Send_P(PSTR(" D4=")); UART1_SendHex(pData->nPortD4);
        UART1_Send_P(PSTR(" D5=")); UART1_SendHex(pData->nPortD5);
        UART1_Send_P(PSTR(" D6=")); UART1_SendHex(pData->nPortD6);
        UART1_Send_P(PSTR(" D7=")); UART1_SendHex(pData->nPortD7);
        UART1_SendCRLF();
    }

    // Initialisation 
    if ( pData->b8Bit )
        Init8Bit( pData );
    else
        Init4Bit( pData );

    bInitialised = true;
}


static void LCD_Display( struct SLCDControl *pData, byte **ReportBuffer, byte *nBit )
{
    if ( nSerialDebugLevel > 10 )
    {
        UART1_Send_P(PSTR("LCD Output Report"));
        UART1_SendCRLF();
    }

    byte nColumn = ReadPackData16( ReportBuffer, nBit, 8 );
    byte nRow = ReadPackData16( ReportBuffer, nBit, 8 );
    if ( nRow > pData->nRows-1 )
        nRow = pData->nRows-1;

    byte nAddr = pData->RowAddr[nRow] + nColumn;
    WriteLCDData( pData, Instr, 0x80 | nAddr );			// Set Cursor Addr

    for ( int i = 0; i < pData->nColumns; i++ )
    {
        byte c = ReadPackData16( ReportBuffer, nBit, 8 );
        if ( c == 0 ) break;
        WriteLCDData( pData, Data, c );					// Data
    }
}


static void LCD_Font( struct SLCDControl *pData, byte **ReportBuffer, byte *nBit )
{
    if ( nSerialDebugLevel > 10 )
    {
        UART1_Send_P(PSTR("LCD Font Report"));
        UART1_SendCRLF();
    }
    
    // [id][data 5 bytes of data]
    // the 5 bytes of data are broken into 7 x 5 bits, one for each row of data.
    byte nCharAddr = ReadPackData16( ReportBuffer, nBit, 8 );
    WriteLCDData( pData, Instr, 0x40 | (nCharAddr << 3) );	// Set CG RAM address
    for ( int i = 0; i < 7; i++ )
    {
        byte nData = ReadPackData16( ReportBuffer, nBit, 5 );
        WriteLCDData( pData, Data, nData );			// Data
    }
    WriteLCDData( pData, Data, 0 );  // this is the space for the underline cursor.
    // and the left overs 5*8 - 7*5
    //ReadPackData16( ReportBuffer, nBit, 5 );
}

static void LCD_Cursor( struct SLCDControl *pData, byte **ReportBuffer, byte *nBit )
{
    if ( nSerialDebugLevel > 10 )
    {
        UART1_Send_P(PSTR("LCD Cursor Report"));
        UART1_SendCRLF();
    }
    
    // [column][row][cursor][blink]
    byte nColumn = ReadPackData16( ReportBuffer, nBit, 8 );
    byte nRow = ReadPackData16( ReportBuffer, nBit, 8 );
    byte nCursor = ReadPackData16( ReportBuffer, nBit, 1 );
    byte nBlink = ReadPackData16( ReportBuffer, nBit, 1 );

    if ( nRow > pData->nRows-1 )
        nRow = pData->nRows-1;
    byte nAddr = pData->RowAddr[nRow] + nColumn;
    WriteLCDData( pData, Instr, 0x80 | nAddr );     // Set Cursor Addr

    WriteLCDData( pData, Instr, (1 << 3) |          // Display On/Off cmd
                  (1 << 2) |          // Display On
                  (nCursor << 1) |    // Cursor
                  (nBlink << 0) );    // Blink
}

static void LCD_FunctionSet( struct SLCDControl *pData, byte **ReportBuffer, byte *nBit )
{
    if ( nSerialDebugLevel > 10 )
    {
        UART1_Send_P(PSTR("LCD FunctionSet Report"));
        UART1_SendCRLF();
    }
    
    // [row][column][Enable][blink]
    byte nFunctionSet = ReadPackData16( ReportBuffer, nBit, 3 );

    byte data = (1 << 5) |                      // Function Set Cmd
                ((pData->b8Bit?1:0) << 4) |     // 4/8Bit
                (((pData->nRows>1)?1:0) << 3) | // 1 or 2 rows
                nFunctionSet;                   // Function
    WriteLCDData( pData, Instr, data );
    //{
    //    static char Hex[] = "0123456789ABCDEF";
    //    WriteLCDData( pData, Data, Hex[data>>4] );
    //    WriteLCDData( pData, Data, Hex[data&0xF] );
    //}
}

void WriteLCD( struct SLCDControl *pData, byte nReportId, byte **ReportBuffer, byte *nBit )
{
    if ( nReportId == pData->hdr.ReportId + LCD_DISPLAY_REPORT_ID )
        LCD_Display( pData, ReportBuffer, nBit );
    else if ( nReportId == pData->hdr.ReportId + LCD_FONT_REPORT_ID )
        LCD_Font( pData, ReportBuffer, nBit );
    else if ( nReportId == pData->hdr.ReportId + LCD_CURSOR_POSITION_REPORT_ID )
        LCD_Cursor( pData, ReportBuffer, nBit );
    else if ( nReportId == pData->hdr.ReportId + LCD_FUNCTION_SET_REPORT_ID )
        LCD_FunctionSet( pData, ReportBuffer, nBit );
    else
    {
        if ( nSerialDebugLevel > 0 )
        {
            UART1_Send_P(PSTR("LCD Unknown report id "));
            UART1_SendInt(nReportId);
            UART1_SendChar('?');
            UART1_SendInt(pData->hdr.ReportId);
            UART1_SendCRLF();
        }
    }
}


void SendFeatureAttributeReportLCD( struct SLCDControl *pData )
{
    byte *pReport = &(pData->hdr.ReportId);

    for ( int i = 0; i < 3; i++, pReport++ )
    {
        if ( nSerialDebugLevel > 10 )
        {
            UART1_SendHex( *pReport );
            UART1_SendChar( ' ' );
        }
        Endpoint_Write_Byte( *pReport );
    }
    if ( nSerialDebugLevel > 10 )
        UART1_SendCRLF();
}


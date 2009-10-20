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
#include "LCD_SPI.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>
// todo put this somewhere
#include "D:\\Projects\\CNC\\PendantDisplay\\source\\display\\displaycommands.h"

static bool bInitialised = false;			// first time through we need to initialise everything
static byte nOldBrightness;
static uint16_t nOldForegroundColour;
static uint16_t nOldBackgroundColour;
static byte nOldFont;


#ifndef SPI_TX_BUFFER_SIZE
    #define SPI_TX_BUFFER_SIZE 256		/* 2,4,8,16,32,64,128 or 256 bytes */
#endif
#define SPI_TX_BUFFER_MASK ( SPI_TX_BUFFER_SIZE - 1 )
#if ( SPI_TX_BUFFER_SIZE & SPI_TX_BUFFER_MASK )
    #error TX buffer size is not a power of 2
#endif

static byte SPI_TxBuf[SPI_TX_BUFFER_SIZE];
static volatile byte SPI_TxHead;
static volatile byte SPI_TxTail; 


SIGNAL (SPI_STC_vect)
{ 
    PORTB |= _BV(PB0);	    // SS high

    /* Check if all data is transmitted */
    if ( SPI_TxHead != SPI_TxTail )
    {
	/* Calculate buffer index */
	byte tmptail = ( SPI_TxTail + 1 ) & SPI_TX_BUFFER_MASK;
	SPI_TxTail = tmptail;      /* Store new index */

	PORTB &= ~_BV(PB0);	    // SS low
	SPDR = SPI_TxBuf[tmptail];  /* Start transmition */
    }
    else
    {
	SPCR &= ~_BV(SPIE);	    // Disable SPIE
    }
}
 
static void spi_SendChar( byte data )
{
       /* Calculate buffer index */
    unsigned char tmphead = ( SPI_TxHead + 1 ) & SPI_TX_BUFFER_MASK; /* Wait for free space in buffer */
    if ( tmphead == SPI_TxTail )
    {
	// problem.  Buffer full
	return;
    }

    // if SPI is off, just send the data, don't buffer
    cli();
    if ( !(SPCR & _BV(SPIE)) )	
    {
	sei();
	// interrupt is off.
	SPCR |= _BV(SPIE);		    // Enable SPIE
	PORTB &= ~_BV(PB0);		    // SS low
	SPDR = data;
    }
    else
    {
	SPI_TxBuf[tmphead] = data;          /* Store data in buffer */
	SPI_TxHead = tmphead;               /* Store new index */
	sei();
    }
}

static void spi_SendBuf( byte *pBuf, int nLen )
{
    while ( nLen-- )
	spi_SendChar( *(pBuf++) );
}


static void spi_init(void)
{
    DDRB &= ~_BV(PB3);					// MISO input
    DDRB |= OUTPUT(PB2) | OUTPUT(PB1) | OUTPUT(PB0);	// MOSI output, SCK output, SS output
    SPCR = _BV(MSTR) | _BV(SPE) | _BV(SPR1) | _BV(SPR0);    // master, enable, clk/128
//    SPCR = _BV(MSTR) | _BV(SPE);				// master, enable, clk/4

    PORTB |= _BV(PB0);	    // SS high

    SPI_TxTail = 0; 
    SPI_TxHead = 0;
}



void InitLCD_SPI( struct SLCDSPIControl *pData )
{
    if ( bSerialDebug )
    {
	UART1_Send_P(PSTR("LCD_SPI Initialising"));
	UART1_SendCRLF();
    }

    if ( !bInitialised )
    {
	nOldBrightness = 100;
	nOldForegroundColour = 0xFFFF;
	nOldBackgroundColour = 0;
	nOldFont = 0;

	spi_init();
    }


    bInitialised = true;
}


static void LCD_SPI_Display( struct SLCDSPIControl *pData, byte **ReportBuffer, byte *nBit )
{
    if ( bSerialDebug )
    {
	UART1_Send_P(PSTR("LCD_SPI Character Output Report"));
	UART1_SendCRLF();
    }

    // really should use the report descriptor, or at least create constants for all this.
    uint16_t nColumn = ReadPackData16( ReportBuffer, nBit, 9 );
    byte nRow = ReadPackData16( ReportBuffer, nBit, 8 );
    byte bPixelPositioning = ReadPackData16( ReportBuffer, nBit, 1 );
    ReadPackData16( ReportBuffer, nBit, 6 );	// padding

    if ( bPixelPositioning )
    {
	CmdSetGraphicsPosition cmd;
	cmd.cmd = CMD_SET_GRAPHICS_POSITION;
	cmd.x = nColumn;
	cmd.y = nRow;
	spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
    }
    else
    {
	CmdSetTextPosition cmd;
	cmd.cmd = CMD_SET_TEXT_POSITION;
	cmd.x = nColumn;
	cmd.y = nRow;
	spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
    }

    spi_SendChar( CMD_WRITE_TEXT );
    for ( int i = 0; i < pData->nColumns; i++ )
    {
	byte c = ReadPackData16( ReportBuffer, nBit, 8 );
	if ( c == 0 ) 
	    break;
	spi_SendChar( c );
    }
    spi_SendChar( 0 );
}


static void LCD_SPI_DisplayControl( struct SLCDSPIControl *pData, byte **ReportBuffer, byte *nBit )
{
    if ( bSerialDebug )
    {
	UART1_Send_P(PSTR("LCD_SPI Display control Output Report"));
	UART1_SendCRLF();
    }

    // really should use the report descriptor, or at least create constants for all this.
    byte nDisplayClear = ReadPackData16( ReportBuffer, nBit, 1 );
    byte nBrightness = ReadPackData16( ReportBuffer, nBit, 7 );
    uint16_t nForegroundColour = ReadPackData16( ReportBuffer, nBit, 16 );
    uint16_t nBackgroundColour = ReadPackData16( ReportBuffer, nBit, 16 );
    byte nFont = ReadPackData16( ReportBuffer, nBit, 1 );
    //byte nAttrUnderline = ReadPackData16( ReportBuffer, nBit, 1 );

    if ( nBrightness != nOldBrightness )
    {
	CmdSetBacklight cmd;
	cmd.cmd = CMD_SET_BACKLIGHT;
	cmd.intensity = nBrightness;
	spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
	nOldBrightness = nBrightness;
    }
    if ( nForegroundColour != nOldForegroundColour )
    {
	CmdSetForegroundColour cmd;
	cmd.cmd = CMD_SET_FOREGROUND_COLOUR;
	cmd.color = nForegroundColour;
	spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
	nOldForegroundColour = nForegroundColour;
    }
    if ( nBackgroundColour != nOldBackgroundColour )
    {
	CmdSetBackgroundColour cmd;
	cmd.cmd = CMD_SET_BACKGROUND_COLOUR;
	cmd.color = nBackgroundColour;
	spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
	nOldBackgroundColour = nBackgroundColour;
    }
    if ( nFont != nOldFont )
    {
	CmdSetFont cmd;
	cmd.cmd = CMD_SET_FONT;
	cmd.font = nFont;
	spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
	nOldFont = nFont;
    }
    if ( nDisplayClear )
    {
	CmdClearScreen cmd;
	cmd.cmd = CMD_CLEAR_SCREEN;
	spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
    }
}


static void LCD_SPI_DrawRect( struct SLCDSPIControl *pData, byte **ReportBuffer, byte *nBit )
{
    if ( bSerialDebug )
    {
	UART1_Send_P(PSTR("LCD_SPI DrawRect Output Report"));
	UART1_SendCRLF();
    }

    // really should use the report descriptor, or at least create constants for all this.
    CmdDrawRectangle cmd;
    cmd.cmd = CMD_DRAW_RECTANGLE;
    cmd.fill = ReadPackData16( ReportBuffer, nBit, 1 );
    cmd.x = ReadPackData16( ReportBuffer, nBit, 9 );
    cmd.y = ReadPackData16( ReportBuffer, nBit, 8 );
    cmd.width = ReadPackData16( ReportBuffer, nBit, 9 );
    cmd.height = ReadPackData16( ReportBuffer, nBit, 8 );
    spi_SendBuf( (byte *)&cmd, sizeof(cmd) );
}


void WriteLCD_SPI( struct SLCDSPIControl *pData, byte nReportId, byte **ReportBuffer, byte *nBit )
{
    if ( nReportId == pData->hdr.ReportId + LCDSPI_DISPLAY_REPORT_ID )
	LCD_SPI_Display( pData, ReportBuffer, nBit );
    else if ( nReportId == pData->hdr.ReportId + LCDSPI_DISPLAY_CONTROL_REPORT_ID )
	LCD_SPI_DisplayControl( pData, ReportBuffer, nBit );
    else if ( nReportId == pData->hdr.ReportId + LCDSPI_DRAW_RECT_REPORT_ID )
	LCD_SPI_DrawRect( pData, ReportBuffer, nBit );
    else
    {
	if ( bSerialDebug )
	{
	    UART1_Send_P(PSTR("LCD_SPI Unknown report id "));
	    UART1_SendInt(nReportId);
	    UART1_SendChar('?');
	    UART1_SendInt(pData->hdr.ReportId);
	    UART1_SendCRLF();
	}
    }

}



void SendFeatureAttributeReportLCD_SPI( struct SLCDSPIControl *pData )
{
    byte *pReport = &(pData->hdr.ReportId);

    for ( int i = 0; i < 3; i++, pReport++ )
    {
	if ( bSerialDebug )
	{
	    UART1_SendHex( *pReport );
	    UART1_SendChar( ' ' );
	}
	Endpoint_Write_Byte( *pReport );
    }
    if ( bSerialDebug )
	UART1_SendCRLF();
}



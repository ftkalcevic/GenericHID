// graphicdisplay, firmware for the lcd graphic display
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

#ifndef _DISPLAYCOMMANDS_H_
#define _DISPLAYCOMMANDS_H_


#define RGB(r,g,b) ( ((uint16_t)(b>>3) & 0x1F) | ((((uint16_t)(g>>3)) & 0x1f) << 5) | (((uint16_t)(r>>3) & 0x1f) << 11) )

#define CMD_SET_BACKGROUND_COLOUR	0x80	    // Set Background Colour CMD, LSB, MSB
#define CMD_SET_FOREGROUND_COLOUR	0x81	    // Set Foreground Colour CMD, LSB, MSB
#define CMD_CLEAR_SCREEN		0x82	    // Clear Screen CMD,
#define CMD_WRITE_TEXT			0x83	    // Write Text - CMD, data..., NULL
#define CMD_SET_TEXT_POSITION		0x84	    // Set position CMD, x, y
#define CMD_SET_GRAPHICS_POSITION	0x85	    // set position CMD, xlsb, xmsb, y
#define CMD_SET_FONT			0x86	    // Set font CMD, 0/1
#define CMD_SET_BACKLIGHT		0x87	    // set backlight CMD, 0-100
#define CMD_SET_TEXT_ATTRIBUTE		0x88	    // set attribute CMD, [underline][Reverse?bg/fg?][italic][bold][strikethrough]
#define     TEXT_ATTRIBUTE_UNDERLINE	    0x01
#define     TEXT_ATTRIBUTE_STRIKEOUT	    0x02
#define CMD_DRAW_RECTANGLE		0x89	    // rect xlsb, xmsg, y, widthlsb, widthmsb, height, fill

#define CMD_MIN				0x80
#define CMD_MAX				0x89


typedef struct _CmdSetBackgroundColour
{
    byte cmd;
    uint16_t color;
} CmdSetBackgroundColour;

typedef struct _CmdSetForegroundColour
{
    byte cmd;
    uint16_t color;
} CmdSetForegroundColour;

typedef struct _CmdClearScreen
{
    byte cmd;
} CmdClearScreen;

typedef struct _CmdWriteText
{
    byte cmd;
    byte data[0];   // null terminated string.
} CmdWriteText;

typedef struct _CmdSetTextPosition
{
    byte cmd;
    byte x;
    byte y;
} CmdSetTextPosition;

typedef struct _CmdSetGraphicsPosition
{
    byte cmd;
    uint16_t x;
    byte y;
} CmdSetGraphicsPosition;

typedef struct _CmdSetFont
{
    byte cmd;
    byte font;
} CmdSetFont;

typedef struct _CmdSetBacklight
{
    byte cmd;
    byte intensity;
} CmdSetBacklight;

typedef struct _CmdSetAttribute
{
    byte cmd;
    byte attribute;
} CmdSetAttribute;

typedef struct _CmdDrawRectangle
{
    byte cmd;
    uint16_t x;
    byte y;
    uint16_t width;
    byte height;
    byte fill;
} CmdDrawRectangle;

#define MAX_CMD_LEN			(sizeof(CmdDrawRectangle))

#endif


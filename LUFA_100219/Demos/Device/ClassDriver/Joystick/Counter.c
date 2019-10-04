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
#include "Joystick.h"
#include "Controls.h"
#include "Counter.h"

#include <string.h>


void InitCounter( struct SCounterControl *pData )
{
    g_nCounterTop = pData->Period;
    g_nCounterCount = 0;
    g_nCounterCounter = 0;
    uint32_t nMask = 1;
    nMask <<= pData->Bits;
    nMask -= 1;
    g_nCounterMask = *(uint16_t *)&nMask;

    if ( nSerialDebugLevel > 1 )
    {
	UART1_Send_P(PSTR("InitCounter Period="));
	UART1_SendHex(g_nCounterTop);
	UART1_Send_P(PSTR(", Bits="));
	UART1_SendHex(pData->Bits);
	UART1_Send_P(PSTR(", Mask="));
	UART1_SendHex(g_nCounterMask);
	UART1_SendCRLF();
    }
}

void ReadCounter( struct SCounterControl *pData, byte **ReportBuffer, byte *nBit )
{
    uint16_t n = g_nCounterCounter;
    n = n & g_nCounterMask;
    PackData16( ReportBuffer, nBit, n, pData->Bits );
}



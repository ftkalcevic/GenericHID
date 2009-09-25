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

    if ( bSerialDebug )
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



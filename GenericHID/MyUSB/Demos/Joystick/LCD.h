#ifndef _LCD_H_
#define _LCD_H_


extern void InitLCD( struct SLCDControl *pData );
extern void WriteLCD( struct SLCDControl *pData, byte **ReportBuffer, byte *nBit );
extern void WriteLCDFont( struct SLCDFontControl *pData, byte **ReportBuffer, byte *nBit );
extern void SendFeatureAttributeReportLCD( struct SLCDControl *pData );

#endif

#ifndef _CONTROLS_H_
#define _CONTROLS_H_

// port pins are stored as (port id) * 8 + pin

#define GET_PORT_ID( n )			( (n) >> 3 )
#define GET_PORT_PIN( n )			( (n) & 0x7 )

extern void InitControls( byte *pData );
extern void PackData16( byte **pBuf, byte *nBit, uint16_t nValue, byte nBits );
extern uint16_t ReadPackData16( byte **pBuf, byte *nBit, byte nBits );
extern void ReadControls( byte nReportId, byte *pData, byte *pReportBuffer, byte *nLength );
extern void WriteControls( byte nReportId, byte *pData, byte *pReportBuffer );
extern void WriteFeatureReport( byte nReportId, byte *pData );

//extern void SetPORTBit( byte nPort, byte nPin, byte n );
#define DDR_INPUT 0
#define DDR_OUTPUT 1
extern void SetDDRBit( byte nPort, byte nPin, byte n );
//extern byte GetPINBit( byte nPort, byte nPin );

extern byte GetIOBit( byte nIO );
extern void SetIOBit( byte nIO );
extern void ClearIOBit( byte nIO ); 

#define SetPORTBit( nPort, nPin, n )		( n ? SetIOBit( nPort * 8 + nPin ) : ClearIOBit( nPort * 8 + nPin ) )
#define GetPINBit( nPort, nPin )			GetIOBit( nPort * 8 + nPin )

#define SetDDR( port, dir )			SetDDRBit( GET_PORT_ID(port), GET_PORT_PIN(port), dir )
#define SetPORT( port, n )			( n ? SetIOBit( port ) : ClearIOBit( port ) )
#define GetPIN( port )				GetIOBit( port )

#endif

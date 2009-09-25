#ifndef __AVR_SERIAL_H__
#define __AVR_SERIAL_H__

enum EParity
{
    eNone,
    eOdd,
    eEven
} ;


#ifdef UART0_RS232_TX
    extern void UART0_SendChar( byte data );
    extern void UART0_Send( char *str );
    extern void UART0_Send_P( PGM_P s  );
    extern void UART0_SendInt( int v );
    extern void UART0_SendLong( long v );
    extern void UART0_SendHex( int v );
    extern void UART0_SendCRLF( void ); 
    extern bool UART0_SendBufferEmpty( void ); 
#endif
#ifdef UART0_RS232_RX
    extern byte UART0_ReadByte( void );
    extern bool UART0_ReadDataAvailable( void );
#endif
#if defined( UART0_RS232_RX ) || defined( UART0_RS232_TX )
    
#endif


#ifdef UART1_RS232_TX
    extern void UART1_SendChar( byte data );
    extern void UART1_Send( char *str );
    extern void UART1_Send_P( PGM_P s  );
    extern void UART1_SendInt( int v );
    extern void UART1_SendLong( long v );
    extern void UART1_SendHex( int v );
    extern void UART1_SendCRLF( void ); 
    extern bool UART1_SendBufferEmpty( void ); 
#endif
#ifdef UART1_RS232_RX
    extern byte UART1_ReadByte( void );
    extern bool UART1_ReadDataAvailable( void );
#endif
#if defined(UART1_RS232_RX) || defined(UART1_RS232_TX)
    extern void UART1_Init( unsigned short nBaud );
    extern void UART1_SetCharSize( byte nBits );
    extern void UART1_SetStopBits( byte nBits );
    extern void UART1_SetParity( enum EParity eParity );
#endif

    extern bool bSerialDebug;
#endif

#ifndef H__AVR_COMMON_H__
#define H__AVR_COMMON_H__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

typedef uint8_t bool;
typedef uint8_t byte;

#define true 1
#define false 0

#include "../../../include/deviceconfig.h"


#define countof(x) ((sizeof(x))/sizeof(x[0]))

#define _delay_us( _clk, _us )											\
{																		\
	const double ticks = ((double)_us) * ((double)_clk) / ((double)1000000.0);\
	const double loops1 = ticks/((double)3.0);							\
	const double loops2 = ticks/((double)4.0);							\
	const unsigned int iloops2 = loops2;								\
	if ( loops1 < ((double)256) )										\
	{																	\
		_delay_loop_1( ((unsigned char)loops1) );						\
	}																	\
	else if ( loops2 < ((double)65535.0) )								\
	{																	\
		_delay_loop_2( iloops2 );										\
	}																	\
	else																\
	{																	\
		_delay_loop_2( ((unsigned int)65535) );							\
	}																	\
}

#define INPUT(bit_number)	(0)
#define OUTPUT(bit_number) (_BV(bit_number))
#define SET_OUTPUT_PIN(port, bit_number)	port |= OUTPUT(bit_number)

#define NO_OP __asm__ __volatile__ ("nop")

#endif


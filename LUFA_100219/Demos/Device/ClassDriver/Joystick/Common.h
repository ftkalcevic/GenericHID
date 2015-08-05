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

#include "../../../../../include/deviceconfig.h"


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


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



/*
   PortIO.S

	runtime access to IO ports.
	
	byte GetIOBit( byte nIO );
	void SetIOBit( byte nIO );
	void ClearIOBit( byte nIO );
  
*/

#include "macros.inc"  
#include "ctoasm.inc"

	.text
	
/*  ---------------------------------------------------------------------- */
	
	.global _U(GetIOBit)
	.func _U(GetIOBit)

_U(GetIOBit):

	// We don't do error checking.  Just compute the jump offset and do it.
	mov	r30, r24
    eor	r31, r31
    lsl	r30
    lsl	r30
    eor	r24, r24
    subi r30, lo8(pm(-(GetIOsStart)))
    sbci r31, hi8(pm(-(GetIOsStart)))
    ijmp
    
GetIOsStart:
.macro GET_BIT_CODE Port, Bit
    sbic	\Port, \Bit
    ldi		r24, 1
    ret
    nop
.endm

.macro GET_PORT_BIT_CODE Port
GET_BIT_CODE \Port, 0
GET_BIT_CODE \Port, 1 
GET_BIT_CODE \Port, 2 
GET_BIT_CODE \Port, 3 
GET_BIT_CODE \Port, 4 
GET_BIT_CODE \Port, 5 
GET_BIT_CODE \Port, 6 
GET_BIT_CODE \Port, 7 
.endm

GET_PORT_BIT_CODE 0x0		; PinA
GET_PORT_BIT_CODE 0x3		; PinB
GET_PORT_BIT_CODE 0x6		; PinC
GET_PORT_BIT_CODE 0x9		; PinD
GET_PORT_BIT_CODE 0xc		; PinE
GET_PORT_BIT_CODE 0xf		; PinF


	
	.endfunc

/*  ---------------------------------------------------------------------- */
/*  ---------------------------------------------------------------------- */
	
	.global _U(SetIOBit)
	.func _U(SetIOBit)

_U(SetIOBit):

	// We don't do error checking.  Just compute the jump offset and do it.
	mov	r30, r24
    eor	r31, r31
    lsl	r30
    eor	r24, r24
    subi r30, lo8(pm(-(SetIOsStart)))
    sbci r31, hi8(pm(-(SetIOsStart)))
    ijmp
    
SetIOsStart:
.macro SET_BIT_CODE Port, Bit
    sbi		\Port, \Bit
    ret
.endm

.macro SET_PORT_BIT_CODE Port
SET_BIT_CODE \Port, 0
SET_BIT_CODE \Port, 1 
SET_BIT_CODE \Port, 2 
SET_BIT_CODE \Port, 3 
SET_BIT_CODE \Port, 4 
SET_BIT_CODE \Port, 5 
SET_BIT_CODE \Port, 6 
SET_BIT_CODE \Port, 7 
.endm

SET_PORT_BIT_CODE 0x2		; PortA
SET_PORT_BIT_CODE 0x5		; PortB
SET_PORT_BIT_CODE 0x8		; PortC
SET_PORT_BIT_CODE 0xB		; PortD
SET_PORT_BIT_CODE 0xE		; PortE
SET_PORT_BIT_CODE 0x11		; PortF
	
	.endfunc

/*  ---------------------------------------------------------------------- */


/*  ---------------------------------------------------------------------- */
/*  ---------------------------------------------------------------------- */
	
	.global _U(ClearIOBit)
	.func _U(ClearIOBit)

_U(ClearIOBit):

	// We don't do error checking.  Just compute the jump offset and do it.
	mov	r30, r24
    eor	r31, r31
    lsl	r30
    eor	r24, r24
    subi r30, lo8(pm(-(ClearIOsStart)))
    sbci r31, hi8(pm(-(ClearIOsStart)))
    ijmp
    
ClearIOsStart:
.macro CLEAR_BIT_CODE Port, Bit
    cbi		\Port, \Bit
    ret
.endm

.macro CLEAR_PORT_BIT_CODE Port
CLEAR_BIT_CODE \Port, 0
CLEAR_BIT_CODE \Port, 1 
CLEAR_BIT_CODE \Port, 2 
CLEAR_BIT_CODE \Port, 3 
CLEAR_BIT_CODE \Port, 4 
CLEAR_BIT_CODE \Port, 5 
CLEAR_BIT_CODE \Port, 6 
CLEAR_BIT_CODE \Port, 7 
.endm

CLEAR_PORT_BIT_CODE 0x2		; PortA
CLEAR_PORT_BIT_CODE 0x5		; PortB
CLEAR_PORT_BIT_CODE 0x8		; PortC
CLEAR_PORT_BIT_CODE 0xB		; PortD
CLEAR_PORT_BIT_CODE 0xE		; PortE
CLEAR_PORT_BIT_CODE 0x11	; PortF
	
	.endfunc

/*  ---------------------------------------------------------------------- */
/*  ---------------------------------------------------------------------- */

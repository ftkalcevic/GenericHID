/* Copyright (C) 1997,1998,1999,2000,2001,2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/*
 *	ISO C99: 7.18 Integer types <stdint.h>
 */

#ifndef _INT_TYPES_H_
#define _INT_TYPES_H_

#ifdef _WIN32
    typedef signed char		int8_t;
    typedef short int		int16_t;
    typedef int			int32_t;
    typedef __int64			int64_t;

    /* Unsigned.  */
    typedef unsigned char		uint8_t;
    typedef unsigned short int	uint16_t;
    typedef unsigned int		uint32_t;
    typedef unsigned __int64	uint64_t;

    #define UINT8_MAX		(255)
    #define UINT16_MAX		(65535)

#else
    #define __STDC_LIMIT_MACROS
    #include <stdint.h>
#endif

#endif 

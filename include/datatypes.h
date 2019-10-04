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

#ifndef _TYPES_H_
#define _TYPES_H_


#ifdef _WIN32
    typedef signed char		int8_t;
    typedef short int		int16_t;
    typedef int			int32_t;
    typedef __int64		int64_t;

    typedef unsigned char	uint8_t;
    typedef unsigned short int	uint16_t;
    typedef unsigned int	uint32_t;
    typedef unsigned __int64	uint64_t;

    #define UINT8_MAX		(255)
    #define UINT16_MAX		(65535)

#else
    #define __STDC_LIMIT_MACROS
    #include <stdint.h>
#endif

typedef uint8_t	byte;

#endif

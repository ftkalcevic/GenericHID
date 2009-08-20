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

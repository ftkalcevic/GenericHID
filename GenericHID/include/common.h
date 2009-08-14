#ifndef _COMMON_H_
#define _COMMON_H_

#define countof(x)		(sizeof(x)/(sizeof((x)[0])))

#ifndef _WIN32
#define _snprintf snprintf
#endif

#define VERSION_MAJOR	    1
#define VERSION_MINOR	    0

#define FRANKSWORKSHOP_VID	    0x1C40
#define GENERICHID_PID		    0x04D9
#define ORBEE_PID		    0x04DA

#endif

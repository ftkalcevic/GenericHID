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

#ifndef _COMMON_H_
#define _COMMON_H_

#define countof(x)		(sizeof(x)/(sizeof((x)[0])))

#ifndef _WIN32
#define _snprintf snprintf
#endif

#define VERSION_MAJOR	    1
#define VERSION_MINOR	    3

#define FRANKSWORKSHOP_VID	    0x1C40
#define GENERICHID_PID		    0x04D9
#define ORBEE_PID		    0x04DA

#endif

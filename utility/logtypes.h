// hidcomp/hidconfig, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

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

#ifndef _LOGTYPES_H_
#define _LOGTYPES_H_

namespace LogTypes
{
    enum LogTypes
    {
        Error        = 1,
        Warning      = 2,
        Info         = 3,
        Debug        = 4,
        Audit        = 5,
        FuncEnter    = 6,
        FuncExit     = 7,
    };
};


#endif

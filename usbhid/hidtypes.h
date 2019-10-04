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

#ifndef _HIDTYPES_H_
#define _HIDTYPES_H_

#pragma pack(1)

struct HID_DESCRIPTOR
{
    unsigned char bLength;
    unsigned char bDescriptorType;
    unsigned short bcdHID;
    unsigned char bCountryCode;
    unsigned char bNumDescriptors;
    struct
    {
        unsigned char bDescriptorType;
        unsigned short wDescriptorLength;
    } Descriptor[1];
};

#pragma pack()


#endif

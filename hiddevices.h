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


#ifndef _HIDDEVICES_H_
#define _HIDDEVICES_H_

#include "hiddevice.h"

class HIDDevices
{
public:
    HIDDevices();
    ~HIDDevices(void);
    static bool Open(int nDebugLevel=0);

private:
    static bool m_bOpen;

public:
    std::vector<HIDDevice *>  m_Devices;
    bool FindHIDDevices();
    std::vector<HIDDevice *> SearchHIDDevices( bool bPID, unsigned short nPID, bool bVID, unsigned short nVID, bool bManufacturer, const QString &sManfacturer, bool bProduct, const QString &sProduct, bool bSN, const QString &sSerialNumber, bool bSystemId, const QString &sSystemId, bool bInterfaceNumber, const byte nInterfaceNumber );
    Logger m_Logger;
};

#endif

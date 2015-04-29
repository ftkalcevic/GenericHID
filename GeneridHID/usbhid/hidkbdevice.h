// hidcomp/hidconfig/generichid, HID device interface for emc
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

#ifndef _HIDKBDEVICE_H_
#define _HIDKBDEVICE_H_

#include "hidparser.h"
#include "hiddevice.h"
#include "datatypes.h"

#ifdef _WIN32
#pragma warning(push, 1)
#endif

#include <QVector>
#include <QMap>

#ifdef _WIN32
#pragma warning(pop)
#pragma warning(disable:4251)
#endif

class HIDKBDevice
{
public:
    HIDKBDevice(HID_CollectionPath_t *pCol);
    ~HIDKBDevice(void);

    bool Init();

    const QMap<unsigned short, HID_ReportItem_t *> Modifiers() const { return m_modifiers; }
    const QVector<HID_ReportItem_t *> Keys() const { return m_keys; }
    const QMap<unsigned short, bool> ModifiersDown() const { return m_ModifiersDown; }
    const QVector<int> KeysDown() const { return m_KeysDown; }

    bool ProcessKeyboardData();


private:
    Logger m_Logger;
    bool m_bInitialised;
    HID_CollectionPath_t *m_pKBCol;
    QMap<unsigned short, HID_ReportItem_t *> m_modifiers;
    QVector<HID_ReportItem_t *> m_keys;
    QMap<unsigned short, bool> m_ModifiersDown;
    QVector<int> m_KeysDown;

    bool FindKeys();
    bool FindModifiers();
};

#endif

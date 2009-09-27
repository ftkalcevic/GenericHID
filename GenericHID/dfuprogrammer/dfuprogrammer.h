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

#ifndef _DFUPROGRAMMER_H_
#define _DFUPROGRAMMER_H_

#include "dfutypes.h"
#include "inttypes.h"
#include "intelhexbuffer.h"

class DFUProgrammer
{
public:
    DFUProgrammer( enum targets_enum target );
    ~DFUProgrammer();
    bool GetDevice();
    bool ReleaseDevice();
    bool HasDevice();

    bool EraseDevice();
    bool StartProgramming(IntelHexBuffer &memory);
    bool StartVerify(IntelHexBuffer &memory);
    bool EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr = 0);
    IntelHexBuffer LoadHex(MemoryType::MemoryType memtype, const QString &sBuffer);
    IntelHexBuffer LoadHexFile(MemoryType::MemoryType memtype, const QString &sPath);
    //const QString &LastError() { return m_sLastError; }

    void RegisterCallback( void (*callback)( void *user_data, int percent ), void *user_data )
    {
	m_Callback = callback;
	m_user_data = user_data;
    }

private:
    enum targets_enum m_eTargetDevice;
    struct target_details *m_pDetails;
    struct dfu_device_t *m_pDFUDevice;
    struct usb_device *m_pDevice;

    uint32_t m_top_eeprom_memory_address;
    uint32_t m_flash_address_top;
    uint32_t m_flash_address_bottom;
    uint32_t m_bootloader_bottom;
    uint32_t m_bootloader_top;

    void (*m_Callback)( void *user_data, int percent );
    void *m_user_data;
};

#endif



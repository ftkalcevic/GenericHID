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
#include "deviceprogrammer.h"

class DFUProgrammer: public DeviceProgrammer
{
public:
    DFUProgrammer( enum targets_enum target );
    virtual ~DFUProgrammer();

    virtual bool GetDevice();
    virtual bool ReleaseDevice();
    //bool HasDevice();
    virtual bool EraseDevice();
    virtual bool StartProgramming(IntelHexBuffer &memory);
    virtual bool StartVerify(IntelHexBuffer &memory);
    virtual bool EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr = 0);
    //const QString &LastError() { return m_sLastError; }


private:
    enum targets_enum m_eTargetDevice;
    struct target_details *m_pDetails;
    struct dfu_device_t *m_pDFUDevice;
    struct usb_device *m_pDevice;
};

#endif



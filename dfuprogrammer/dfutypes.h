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

#ifndef _DFUTYPES_H_
#define _DFUTYPES_H_


enum targets_enum { tar_at89c51snd1c,
                    tar_at89c51snd2c,
                    tar_at89c5130,
                    tar_at89c5131,
                    tar_at89c5132,
                    tar_at90usb1287,
                    tar_at90usb1286,
                    tar_at90usb1287_4k,
                    tar_at90usb1286_4k,
                    tar_at90usb647,
                    tar_at90usb646,
                    tar_at90usb162,
                    tar_at90usb82,
                    tar_atmega32u6,
                    tar_atmega32u4,
                    tar_atmega16u4,
                    tar_at32uc3b064,
                    tar_at32uc3b164,
                    tar_at32uc3b0128,
                    tar_at32uc3b1128,
                    tar_at32uc3b0256,
                    tar_at32uc3b1256,
                    tar_at32uc3b0256es,
                    tar_at32uc3b1256es,
                    tar_at32uc3a0128,
                    tar_at32uc3a1128,
                    tar_at32uc3a0256,
                    tar_at32uc3a1256,
                    tar_at32uc3a0512,
                    tar_at32uc3a1512,
                    tar_at32uc3a0512es,
                    tar_at32uc3a1512es,
                    tar_at32uc3a364,
                    tar_at32uc3a364s,
                    tar_at32uc3a3128,
                    tar_at32uc3a3128s,
                    tar_at32uc3a3256,
                    tar_at32uc3a3256s,
                    tar_none };


enum setfuse_enum { set_lock, set_epfl, set_bootprot, set_bodlevel,
                    set_bodhyst, set_boden, set_isp_bod_en,
                    set_isp_io_cond_en, set_isp_force };


namespace MemoryType
{
    enum MemoryType
    {
        EEPROM,
        FLASH,
    };
};


namespace ResetMode
{
    enum ResetMode
    {
        Hard,
        Soft
    };
};



#endif

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



#ifndef _BICOLOURLED_H_
#define _BICOLOURLED_H_


extern void InitBicolourLED( struct SBicolourLEDControl *pData );
extern void WriteBicolourLED( struct SBicolourLEDControl *pData, byte **ReportBuffer, byte *nBit );
extern void SetBicolourLED( struct SBicolourLEDControl *pData, byte nOut );

#endif

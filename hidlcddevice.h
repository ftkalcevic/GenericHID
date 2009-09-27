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

#ifndef _HIDLCDDEVICE_H_
#define _HIDLCDDEVICE_H_

#include "hidparser.h"
#include "hiddevice.h"
#include "datatypes.h"

#ifdef _WIN32
#pragma warning(push, 1)
#endif

#include <QVarLengthArray>

#ifdef _WIN32
#pragma warning(pop)
#pragma warning(disable:4251)
#endif

class HIDLCDDevice
{
public:
    HIDLCDDevice(HIDDevice *pDevice, HID_CollectionPath_t *pCol);
    ~HIDLCDDevice(void);

    bool Init();
    bool LCDWrite( int nRow, int nCol, QString sText );
    bool LCDSendUserFont( byte nCharIndex, const QVector<byte> &data );

    int rows() const { return m_nRows; }
    int columns() const { return m_nCols; }
    bool hasUserFonts() const { return m_bUserFonts; }
    int minFontIndex() const { return m_nMinFontIndex; }
    int maxFontIndex() const { return m_nMaxFontIndex; }
    bool initialised() const { return m_bInitialised; }
    int characterWidth() const { return 5; }		    // Fixed 5x7 fonts
    int characterHeight() const { return 7; }

private:
    bool initWriteParameters();
    bool getFontAttributes( bool &bUserFonts, int &nMinIndex, int &nMaxIndex );
    bool getDisplayAttributes( int &nRows, int &nCols );

    Logger m_Logger;
    HIDDevice *m_pDevice;
    HID_CollectionPath_t *m_pLCDReportCol;
    HID_ReportItem_t *m_pRowItem;
    HID_ReportItem_t *m_pColItem;
    HID_CollectionPath_t *m_pCharacterReportCol;
    byte m_nCharacterReportId;
    byte m_nCharacterReportOffset;
    int m_nCharacterDataIndex;
    int m_nRows;
    int m_nCols;
    bool m_bUserFonts;
    int m_nMinFontIndex;
    int m_nMaxFontIndex;
    bool m_bInitialised;
    QVarLengthArray<byte> m_bufLCDWrite;
};

#endif

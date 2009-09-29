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

#ifndef _USAGES_H_
#define _USAGES_H_

#include <QString>
#include <map>
#include <vector>
#include "common.h"
#include "log.h"

#include <QString>

class Usage
{
    friend class Usages;

    QString m_sName;
    unsigned short m_nId;

    Usage(QString sName, unsigned short nId);
public:
    const QString &name() const { return m_sName; }
    unsigned short id() const { return m_nId; }
};

class UsagePage
{
    friend class Usages;

private:
    QString m_sName;
    unsigned short m_nId;
    std::vector<Usage*> m_Usages;
    std::map<unsigned short, Usage*> m_UsageMap;

    UsagePage(QString sName, unsigned short nId);
    void AddUsage( Usage *u );

public:
    const std::vector<Usage*> usages() const { return m_Usages; }
    QString GetUsageString(unsigned short nUsage) const;
    const Usage * GetUsage(unsigned short nUsage) const;

    unsigned short id() const { return m_nId; }
    const QString &name() const { return m_sName; }
};



class Usages
{
public:
    Usages(void);
    ~Usages(void);

    static void GetUsages( unsigned short nPage, unsigned short nUsage, QString &sPage, QString &sUsage );
    static const std::vector<UsagePage*> &GetUsagesPages() { LoadUsages(); return m_UsagePages; }
    static const UsagePage *GetUsagePage(unsigned short nPage);

private:
    static std::vector<UsagePage*> m_UsagePages;
    static std::map<unsigned short, UsagePage*> m_UsagePageMap;
    static bool m_bInitialised;
    static void LoadUsages();
    static Logger *m_Logger;
};

// Some Common Usages
#define USAGE_VENDOR_DEFINED			    0xFF00
#define USAGEPAGE_GENERIC_DESKTOP_CONTROLS          0x01
    #define USAGE_POINTER                           0x01
    #define USAGE_MOUSE                             0x02
    #define USAGE_JOYSTICK                          0x04
    #define USAGE_GAME PAD                          0x05
    #define USAGE_KEYBOARD                          0x06
    #define USAGE_KEYPAD                            0x07
    #define USAGE_X                                 0x30
    #define USAGE_Y                                 0x31
    #define USAGE_Z                                 0x32
    #define USAGE_RX                                0x33
    #define USAGE_RY                                0x34
    #define USAGE_RZ                                0x35
    #define USAGE_SLIDER                            0x36
    #define USAGE_DIAL                              0x37
    #define USAGE_WHEEL                             0x38
    #define USAGE_HATSWITCH                         0x39
    #define USAGE_START                             0x3D
    #define USAGE_SELECT                            0x3E
    #define USAGE_DPAD_UP                           0x90
    #define USAGE_DPAD_DOWN                         0x91
    #define USAGE_DPAD_RIGHT                        0x92
    #define USAGE_DPAD_LEFT                         0x93
#define USAGEPAGE_SIMULATION_CONTROLS               0x02
#define USAGEPAGE_VR_CONTROLS                       0x03
#define USAGEPAGE_SPORT_CONTROLS                    0x04
#define USAGEPAGE_GAME_CONTROLS                     0x05
#define USAGEPAGE_GENERIC_DEVICE_CONTROLS           0x06
#define USAGEPAGE_KEYBOARD_KEYPAD                   0x07
#define USAGEPAGE_LEDS                              0x08
#define USAGEPAGE_BUTTON                            0x09
#define USAGEPAGE_ORDINAL                           0x0A
#define USAGEPAGE_TELEPHONY                         0x0B
#define USAGEPAGE_CONSUMER                          0x0C
#define USAGEPAGE_DIGITIZER                         0x0D
#define USAGEPAGE_PID_PAGE                          0x0F
#define USAGEPAGE_UNICODE                           0x10
#define USAGEPAGE_ALPHANUMERIC_DISPLAY              0x14
    #define USAGE_ALPHANUMERIC_DISPLAY              0x01
    #define USAGE_BITMAPPED_DISPLAY                 0x02
    #define USAGE_DISPLAY_ATTRIBUTES_REPORT         0x20
    #define USAGE_ASCII_CHARACTER_SET               0x21
    #define USAGE_DATA_READ_BACK                    0x22
    #define USAGE_FONT_READ_BACK                    0x23
    #define USAGE_DISPLAY_CONTROL_REPORT            0x24
    #define USAGE_CLEAR_DISPLAY                     0x25
    #define USAGE_DISPLAY_ENABLE                    0x26
    #define USAGE_SCREEN_SAVER_DELAY                0x27
    #define USAGE_SCREEN_SAVER_ENABLE               0x28
    #define USAGE_VERTICAL_SCROLL                   0x29
    #define USAGE_HORIZONTAL_SCROLL                 0x2A
    #define USAGE_CHARACTER_REPORT                  0x2B
    #define USAGE_DISPLAY_DATA                      0x2C
    #define USAGE_DISPLAY_STATUS                    0x2D
    #define USAGE_CURSOR_POSITION_REPORT            0x32
    #define USAGE_ROW                               0x33
    #define USAGE_COLUMN                            0x34
    #define USAGE_ROWS                              0x35
    #define USAGE_COLUMNS                           0x36
    #define USAGE_CURSOR_PIXEL_POSITIONING          0x37
    #define USAGE_CURSOR_MODE                       0x38
    #define USAGE_CURSOR_ENABLE                     0x39
    #define USAGE_CURSOR_BLINK                      0x3A
    #define USAGE_FONT_REPORT                       0x3B
    #define USAGE_FONT_DATA                         0x3C
    #define USAGE_CHARACTER_WIDTH                   0x3D
    #define USAGE_CHARACTER_HEIGHT                  0x3E
    #define USAGE_CHARACTER_SPACING_HORIZONTAl      0x3F
    #define USAGE_CHARACTER_SPACING_VERTICAL        0x40
    #define USAGE_UNICODE_CHARACTER_SET             0x41
    #define USAGE_FONT_7_SEGMENT                    0x42
    #define USAGE_7_SEGMENT_DIRECT_MAP              0x43
    #define USAGE_FONT_14_SEGMENT                   0x44
    #define USAGE_14_SEGMENT_DIRECT_MAP             0x45
    #define USAGE_DISPLAY_BRIGHTNESS                0x46
    #define USAGE_DISPLAY_CONTRAST                  0x47
    #define USAGE_CHARACTER_ATTRIBUTE               0x48
    #define USAGE_ATTRIBUTE_READBACK                0x49
    #define USAGE_ATTRIBUTE_DATA                    0x4A
    #define USAGE_CHAR_ATTR_ENHANCE                 0x4B
    #define USAGE_CHAR_ATTR_UNDERLINE               0x4C
    #define USAGE_CHAR_ATTR_BLINK                   0x4D
    #define USAGE_BITMAP_SIZE_X                     0x80
    #define USAGE_BITMAP_SIZE_Y                     0x81
    #define USAGE_BIT_DEPTH_FORMAT                  0x83
    #define USAGE_DISPLAY_ORIENTATION               0x84
    #define USAGE_BLIT_REPORT			    0x8A 
    #define USAGE_BLIT_RECTANGLE_X1		    0x8B 
    #define USAGE_BLIT_RECTANGLE_Y1		    0x8C 
    #define USAGE_BLIT_RECTANGLE_X2		    0x8D 
    #define USAGE_BLIT_RECTANGLE_Y2		    0x8E 
    #define USAGE_BLIT_DATA			    0x8F 
    #define USAGE_FOREGROUND_COLOUR		    (USAGE_VENDOR_DEFINED+0)
    #define USAGE_BACKGROUND_COLOUR		    (USAGE_VENDOR_DEFINED+1)
    #define USAGE_SELECT_FONT			    (USAGE_VENDOR_DEFINED+2)
    #define USAGE_DRAW_RECT_REPORT		    (USAGE_VENDOR_DEFINED+3)
    #define USAGE_RECT_FILLED			    (USAGE_VENDOR_DEFINED+4)
    #define USAGE_RECT_X			    (USAGE_VENDOR_DEFINED+5)
    #define USAGE_RECT_Y			    (USAGE_VENDOR_DEFINED+6)
    #define USAGE_RECT_WIDTH			    (USAGE_VENDOR_DEFINED+7)
    #define USAGE_RECT_HEIGHT			    (USAGE_VENDOR_DEFINED+8)
#define USAGEPAGE_MEDICAL_INSTRUMENTS               0x40
#define USAGEPAGE_MONITOR_PAGES                     0x83
#define USAGEPAGE_POWER_PAGES                       0x87
#define USAGEPAGE_BAR_CODE_SCANNER_PAGE             0x8C
#define USAGEPAGE_SCALE_PAGE                        0x8D
#define USAGEPAGE_MAGNETIC_STRIPE_READING_DEVICES   0x8E
#define USAGEPAGE_CAMERA_CONTROL_PAGE               0x90
#define USAGEPAGE_ARCADE_PAGE                       0x91
#define USAGEPAGE_VENDOR_DEFINED                    0xFF00

#endif

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
	#define KEYBOARD_ERRORROLLOVER		1
	#define KEYBOARD_POSTFAIL		2
	#define KEYBOARD_ERRORUNDEFINED		3
	#define KEYBOARD_A			4
	#define KEYBOARD_B			5
	#define KEYBOARD_C			6
	#define KEYBOARD_D			7
	#define KEYBOARD_E			8
	#define KEYBOARD_F			9
	#define KEYBOARD_G			10
	#define KEYBOARD_H			11
	#define KEYBOARD_I			12
	#define KEYBOARD_J			13
	#define KEYBOARD_K			14
	#define KEYBOARD_L			15
	#define KEYBOARD_M			16
	#define KEYBOARD_N			17
	#define KEYBOARD_O			18
	#define KEYBOARD_P			19
	#define KEYBOARD_Q			20
	#define KEYBOARD_R			21
	#define KEYBOARD_S			22
	#define KEYBOARD_T			23
	#define KEYBOARD_U			24
	#define KEYBOARD_V			25
	#define KEYBOARD_W			26
	#define KEYBOARD_X			27
	#define KEYBOARD_Y			28
	#define KEYBOARD_Z			29
	#define KEYBOARD_1			30
	#define KEYBOARD_2			31
	#define KEYBOARD_3			32
	#define KEYBOARD_4			33
	#define KEYBOARD_5			34
	#define KEYBOARD_6			35
	#define KEYBOARD_7			36
	#define KEYBOARD_8			37
	#define KEYBOARD_9			38
	#define KEYBOARD_0			39
	#define KEYBOARD_ENTER			40
	#define KEYBOARD_ESCAPE			41
	#define KEYBOARD_DELETE			42
	#define KEYBOARD_TAB			43
	#define KEYBOARD_SPACEBAR		44
	#define KEYBOARD_MINUS			45
	#define KEYBOARD_EQUALS			46
	#define KEYBOARD_LEFTBRAKET		47
	#define KEYBOARD_RIGHTBRAKET		48
	#define KEYBOARD_BACKSLASH		49
	#define KEYBOARD_HASH			50
	#define KEYBOARD_SEMICOLON		51
	#define KEYBOARD_APOSTROPHE		52
	#define KEYBOARD_GRAVE			53
	#define	KEYBOARD_COMMA			54
	#define KEYBOARD_PERIOD			55
	#define KEYBOARD_SLASH			56
	#define KEYBOARD_CAPSLOCK		57
	#define KEYBOARD_F1			58
	#define KEYBOARD_F2			59
	#define KEYBOARD_F3			60
	#define KEYBOARD_F4			61
	#define KEYBOARD_F5			62
	#define KEYBOARD_F6			63
	#define KEYBOARD_F7			64
	#define KEYBOARD_F8			65
	#define KEYBOARD_F9			66
	#define KEYBOARD_F10			67
	#define KEYBOARD_F11			68
	#define KEYBOARD_F12			69
	#define KEYBOARD_PRINTSCREEN		70
	#define KEYBOARD_SCROLL			71
	#define KEYBOARD_PAUSE			72
	#define KEYBOARD_INSERT			73
	#define KEYBOARD_HOME			74
	#define KEYBOARD_PAGEUP			75
	#define KEYBOARD_DELETEFORWARD		76
	#define KEYBOARD_END			77
	#define KEYBOARD_PAGEDOWN		78
	#define KEYBOARD_RIGHTARROW		79
	#define KEYBOARD_LEFTARROW		80
	#define KEYBOARD_DOWNARROW		81
	#define KEYBOARD_UPARROW		82
	#define KEYPAD_NUMLOCK			83
	#define KEYPAD_DIVIDE			84
	#define KEYPAD_MULTIPLY			85
	#define KEYPAD_MINUS			86
	#define KEYPAD_PLUS			87
	#define KEYPAD_ENTER			88
	#define KEYPAD_1			89
	#define KEYPAD_2			90
	#define KEYPAD_3			91
	#define KEYPAD_4			92
	#define KEYPAD_5			93
	#define KEYPAD_6			94
	#define KEYPAD_7			95
	#define KEYPAD_8			96
	#define KEYPAD_9			97
	#define KEYPAD_0			98
	#define KEYPAD_PERIOD			99
	#define KEYBOARD_NONUSBACKSLASH		100
	#define KEYBOARD_APPLICATION		101
	#define KEYBOARD_POWER			102
	#define KEYPAD_EQUALS			103
	#define KEYBOARD_F13			104
	#define KEYBOARD_F14			105
	#define KEYBOARD_F15			106
	#define KEYBOARD_F16			107
	#define KEYBOARD_F17			108
	#define KEYBOARD_F18			109
	#define KEYBOARD_F19			110
	#define KEYBOARD_F20			111
	#define KEYBOARD_F21			112
	#define KEYBOARD_F22			113
	#define KEYBOARD_F23			114
	#define KEYBOARD_F24			115
	#define KEYBOARD_EXECUTE		116
	#define KEYBOARD_HELP			117
	#define KEYBOARD_MENU			118
	#define KEYBOARD_SELECT			119
	#define KEYBOARD_STOP			120
	#define KEYBOARD_AGAIN			121
	#define KEYBOARD_UNDO			122
	#define KEYBOARD_CUT			123
	#define KEYBOARD_COPY			124
	#define KEYBOARD_PASTE			125
	#define KEYBOARD_FIND			126
	#define KEYBOARD_MUTE			127
	#define KEYBOARD_VOLUMEUP		128
	#define KEYBOARD_VOLUMEDOWN		129
	#define KEYBOARD_LOCKINGCAPS		130
	#define KEYBOARD_LOCKINGNUM		131
	#define KEYBOARD_LOCKINGSCROLL		132
	#define KEYPAD_COMMA			133
	#define KEYPAD_EQUAL			134
	#define KEYBOARD_INTERNATIONAL1		135
	#define KEYBOARD_INTERNATIONAL2		136
	#define KEYBOARD_INTERNATIONAL3		137
	#define KEYBOARD_INTERNATIONAL4		138
	#define KEYBOARD_INTERNATIONAL5		139
	#define KEYBOARD_INTERNATIONAL6		140
	#define KEYBOARD_INTERNATIONAL7		141
	#define KEYBOARD_INTERNATIONAL8		142
	#define KEYBOARD_INTERNATIONAL9		143
	#define KEYBOARD_LANG1			144
	#define KEYBOARD_LANG2			145
	#define KEYBOARD_LANG3			146
	#define KEYBOARD_LANG4			147
	#define KEYBOARD_LANG5			148
	#define KEYBOARD_LANG6			149
	#define KEYBOARD_LANG7			150
	#define KEYBOARD_LANG8			151
	#define KEYBOARD_LANG9			152
	#define KEYBOARD_ALTERNATE		153
	#define KEYBOARD_SYSREQ			154
	#define KEYBOARD_CANCEL			155
	#define KEYBOARD_CLEAR			156
	#define KEYBOARD_PRIOR			157
	#define KEYBOARD_RETURN			158
	#define KEYBOARD_SEPARATOR		159
	#define KEYBOARD_OUT			160
	#define KEYBOARD_OPER			161
	#define KEYBOARD_CLEARAGAIN		162
	#define KEYBOARD_CRSEL			163
	#define KEYBOARD_EXSEL			164
	#define KEYPAD_00			176
	#define KEYPAD_000			177
	#define KEYPAD_THOUSANDSSEPARATOR	178
	#define KEYPAD_DECIMALSEPARATOR		179
	#define KEYPAD_CURRENCYUNIT		180
	#define KEYPAD_CURRENCYSUBUNIT		181
	#define KEYPAD_LEFTPARENTHESIS		182
	#define KEYPAD_RIGHTPARENTHESIS		183
	#define KEYPAD_LEFTBRACE		184
	#define KEYPAD_RIGHTBRACE		185
	#define KEYPAD_TAB			186
	#define KEYPAD_BACKSPACE		187
	#define KEYPAD_A			188
	#define KEYPAD_B			189
	#define KEYPAD_C			190
	#define KEYPAD_D			191
	#define KEYPAD_E			192
	#define KEYPAD_F			193
	#define KEYPAD_XOR			194
	#define KEYPAD_CARET			195
	#define KEYPAD_PERCENT			196
	#define KEYPAD_LESSTHAN			197
	#define KEYPAD_GREATERTHAN		198
	#define KEYPAD_AMPERSAND		199
	#define KEYPAD_AMPERSANDAMPERSAND	200
	#define KEYPAD_BAR			201
	#define KEYPAD_BARBAR			202
	#define KEYPAD_COLON			203
	#define KEYPAD_HASH			204
	#define KEYPAD_SPACE			205
	#define KEYPAD_AT			206
	#define KEYPAD_EXCLAMATION		207
	#define KEYPAD_MEMORYSTORE		208
	#define KEYPAD_MEMORYRECALL		209
	#define KEYPAD_MEMORYCLEAR		210
	#define KEYPAD_MEMORYADD		211
	#define KEYPAD_MEMORYSUBTRACT		212
	#define KEYPAD_MEMORYMULTIPLY		213
	#define KEYPAD_MEMORYDIVIDE		214
	#define KEYPAD_PLUSMINUS		215
	#define KEYPAD_CLEAR			216
	#define KEYPAD_CLEARENTRY		217
	#define KEYPAD_BINARY			218
	#define KEYPAD_OCTAL			219
	#define KEYPAD_DECIMAL			220
	#define KEYPAD_HEXADECIMAL		221
	#define KEYBOARD_LEFTCONTROL		224
	#define KEYBOARD_LEFTSHIFT		225
	#define KEYBOARD_LEFTALT		226
	#define KEYBOARD_LEFT			227
	#define KEYBOARD_RIGHTCONTROL		228
	#define KEYBOARD_RIGHTSHIFT		229
	#define KEYBOARD_RIGHTALT		230
	#define KEYBOARD_RIGHT			231

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
    #define USAGE_FUNCTION_SET_REPORT		    (USAGE_VENDOR_DEFINED+9)
    #define USAGE_FUNCTION_SET			    (USAGE_VENDOR_DEFINED+10)
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

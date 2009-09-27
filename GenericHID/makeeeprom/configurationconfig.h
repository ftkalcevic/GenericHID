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

#ifndef _CONFIGURATIONCONFIG_H_
#define _CONFIGURATIONCONFIG_H_

#include "configuration.h"
#include "deviceconfig.h"

class ConfigurationConfig : public Configuration
{
    enum { DESCRIPTOR_SIZE = 9 };
public:
    enum USBKeyLED
    {
        LED1,
        LED2,
        Both,
        None
    };

    ConfigurationConfig(void);
    virtual ~ConfigurationConfig(void);
    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetReportDescriptor(StringTable &table) const;
    virtual ByteArray GetControlConfig( byte /*nReportId*/ ) const { return ByteArray(); }	    // todo: should this be in Configuration?

    void setDescriptorsLength( unsigned short n ) { m_nDescriptorsLength = (unsigned short)(n + DESCRIPTOR_SIZE); } 
    bool busPowered() const { return m_bBusPowered; }
    unsigned short powerConsumption() const { return m_nPowerConsumption; }
    unsigned short descriptorsLength() const { return m_nDescriptorsLength; }
    USBKeyLED useStatusLEDs() const { return m_eUseStatusLEDs; }
    bool serialDebug() const { return m_bSerialDebug; }
    bool HIDDebug() const { return m_bHIDDebug; }
    unsigned short usagePage() const { return m_nUsagePage; }
    unsigned short usage() const { return m_nUsage; }
    bool is5Volts() const { return m_b5Volts; }
    byte powerPort() const { return m_nPowerPort; }
    byte interval() const { return m_nInterval; }
    const struct TimerConfig *timers() const { return m_Timers; }

private:
    bool m_bBusPowered;
    unsigned short m_nPowerConsumption;
    unsigned short m_nDescriptorsLength;
    USBKeyLED m_eUseStatusLEDs;
    bool m_bSerialDebug;
    bool m_bHIDDebug;
    unsigned short m_nUsagePage;
    unsigned short m_nUsage;
    bool m_b5Volts;
    byte m_nPowerPort;
    byte m_nInterval;
    struct TimerConfig m_Timers[MAX_TIMERS];
};

#endif

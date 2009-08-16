#ifndef _CONFIGURATIONCONFIG_H_
#define _CONFIGURATIONCONFIG_H_

#include "configuration.h"

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
    virtual ByteArray GetControlConfig( byte nReportId ) const { return ByteArray(); }	    // todo: should this be in Configuration?

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
    int powerPort() const { return m_nPowerPort; }

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
    int m_nPowerPort;
};

#endif

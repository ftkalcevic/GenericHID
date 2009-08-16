#ifndef _CONFIGURATIONENDPOINT_H_
#define _CONFIGURATIONENDPOINT_H_

#include "configuration.h"

namespace TransferType
{
    enum TransferType
    {
	Control = 0,
	Isochronous = 1,
	Bulk = 2,
	Interrupt = 3
    };
};

namespace SynchronisationType
{
    enum SynchronisationType
    {
	NoSync = 0,
	Async = 1,
	Adaptive = 2,
	Synch = 3
    };
};

namespace UsageType
{
    enum UsageType
    {
	Data = 0,
	Feedback = 1,
	ImplicitFeedback = 2
    };
};


class ConfigurationEndpoint :public Configuration
{
public:
    ConfigurationEndpoint(bool bInput, byte nEndpointNumber, TransferType::TransferType transType, SynchronisationType::SynchronisationType syncType, UsageType::UsageType useType, ushort nPacketSize, byte nPollInterval);
    virtual ~ConfigurationEndpoint(void);

    virtual bool Load( const QDomElement &elem, QString *sError ) { return true; }
    virtual ByteArray GetReportDescriptor(StringTable &table) const;		
    virtual ByteArray GetControlConfig( byte nReportId ) const { return ByteArray(); }
private:
    bool m_bInput;
    byte m_nEndpointNumber;
    TransferType::TransferType m_transType;
    SynchronisationType::SynchronisationType m_syncType;
    UsageType::UsageType m_useType;
    ushort m_nPacketSize;
    byte m_nPollInterval;
};

#endif

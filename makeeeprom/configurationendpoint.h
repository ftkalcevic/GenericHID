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

    virtual bool Load( const QDomElement & /*elem*/, QString * /*sError*/ ) { return true; }
    virtual ByteArray GetReportDescriptor(StringTable &table) const;		
    virtual ByteArray GetControlConfig( byte /*nReportId*/ ) const { return ByteArray(); }
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

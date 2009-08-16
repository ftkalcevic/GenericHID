#include "stdafx.h"
#include "configurationendpoint.h"


ConfigurationEndpoint::ConfigurationEndpoint(bool bInput, byte nEndpointNumber, TransferType::TransferType transType, SynchronisationType::SynchronisationType syncType, UsageType::UsageType useType, ushort nPacketSize, byte nPollInterval)
: m_bInput(bInput)
, m_nEndpointNumber(nEndpointNumber)
, m_transType(transType)
, m_syncType(syncType)
, m_useType(useType)
, m_nPacketSize(nPacketSize)
, m_nPollInterval(nPollInterval)
{
}

ConfigurationEndpoint::~ConfigurationEndpoint(void)
{
}



ByteArray ConfigurationEndpoint::GetReportDescriptor(StringTable &table) const
{
    ByteBuffer EndPointDescriptor;

    EndPointDescriptor.AddByte(0);      // Length
    EndPointDescriptor.AddByte(5);      // ENDPOINT Type
    EndPointDescriptor.AddByte((byte)((m_bInput ? 0x80 : 0) | m_nEndpointNumber));      // Endpoint Address
    EndPointDescriptor.AddByte((byte)((int)m_transType | ((int)m_syncType << 2) | ((int)m_useType << 4)));      // Attributes
    EndPointDescriptor.AddShort(m_nPacketSize);      // Endpoint size
    EndPointDescriptor.AddByte(m_nPollInterval);      // polling interval
    EndPointDescriptor[0] = (byte)EndPointDescriptor.count();

    return EndPointDescriptor;
}

#include "dfucommon.h"
#include "inttypes.h"
#include "deviceprogrammer.h"

DeviceProgrammer::DeviceProgrammer()
{
}

DeviceProgrammer::~DeviceProgrammer(void)
{
}

IntelHexBuffer DeviceProgrammer::LoadHex(MemoryType::MemoryType memtype, const QString &sBuffer)
{
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    bool bEeprom = memtype == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
    }

    IntelHexBuffer buffer;
    buffer.load( memtype, sBuffer, top_memory_address );

    return buffer;
}


void DeviceProgrammer::AppendHex(IntelHexBuffer &memory, MemoryType::MemoryType memtype, const QString &sBuffer)
{
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    bool bEeprom = memtype == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
    }

    memory.append( memtype, sBuffer, top_memory_address );
}






IntelHexBuffer DeviceProgrammer::LoadHexFile(MemoryType::MemoryType memtype, const QString &sPath)
{
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    bool bEeprom = memtype == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
    }

    IntelHexBuffer buffer;
    buffer.loadFile( memtype, sPath.toAscii().constData(), top_memory_address );

    return buffer;
}


#include "dfucommon.h"
#include "inttypes.h"
#include "deviceprogrammer.h"

DeviceProgrammer::DeviceProgrammer(void)
{
}

DeviceProgrammer::~DeviceProgrammer(void)
{
}

IntelHexBuffer DeviceProgrammer::LoadHex(MemoryType::MemoryType memtype, const QString &sBuffer)
{
    uint32_t memory_size;
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    uint32_t page_size = m_flash_page_size;
    bool bEeprom = memtype == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
        page_size = m_eeprom_page_size;
    }

    memory_size = top_memory_address - bottom_memory_address;

    IntelHexBuffer buffer;
    buffer.load( memtype, sBuffer, top_memory_address );

    return buffer;
}


void DeviceProgrammer::AppendHex(IntelHexBuffer &memory, MemoryType::MemoryType memtype, const QString &sBuffer)
{
    uint32_t memory_size;
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    uint32_t page_size = m_flash_page_size;
    bool bEeprom = memtype == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
        page_size = m_eeprom_page_size;
    }

    memory_size = top_memory_address - bottom_memory_address;

    memory.append( memtype, sBuffer, top_memory_address );
}






IntelHexBuffer DeviceProgrammer::LoadHexFile(MemoryType::MemoryType memtype, const QString &sPath)
{
    uint32_t memory_size;
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    uint32_t page_size = m_flash_page_size;
    bool bEeprom = memtype == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
        page_size = m_eeprom_page_size;
    }

    memory_size = top_memory_address - bottom_memory_address;

    IntelHexBuffer buffer;
    buffer.loadFile( memtype, sPath.toAscii().constData(), top_memory_address );

    return buffer;
}


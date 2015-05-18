#ifndef _DEVICEPROGRAMMER_H_
#define _DEVICEPROGRAMMER_H_

#include "intelhexbuffer.h"

class DeviceProgrammer
{
protected:
    DeviceProgrammer(void);
public:
    virtual ~DeviceProgrammer(void);

public:
    void RegisterCallback( void (*callback)( void *user_data, int percent ), void *user_data )
    {
	m_Callback = callback;
	m_user_data = user_data;
    }
    virtual bool GetDevice() = 0;
    virtual bool ReleaseDevice() = 0;
    virtual bool EraseDevice() = 0;
    virtual bool StartProgramming(IntelHexBuffer &memory) = 0;
    virtual bool StartVerify(IntelHexBuffer &memory) = 0;
    virtual bool EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr = 0) = 0;

    IntelHexBuffer LoadHex(MemoryType::MemoryType memtype, const QString &sBuffer);
    IntelHexBuffer LoadHexFile(MemoryType::MemoryType memtype, const QString &sPath);
    void AppendHex(IntelHexBuffer &memory, MemoryType::MemoryType memtype, const QString &sBuffer);

protected:
    void (*m_Callback)( void *user_data, int percent );
    void *m_user_data;

    uint32_t m_top_eeprom_memory_address;
    uint32_t m_flash_address_top;
    uint32_t m_flash_address_bottom;
    uint32_t m_bootloader_bottom;
    uint32_t m_bootloader_top;
    uint32_t m_eeprom_page_size;
    uint32_t m_flash_page_size;
};

#endif

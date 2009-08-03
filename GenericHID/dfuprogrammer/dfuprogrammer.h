#ifndef _DFUPROGRAMMER_H_
#define _DFUPROGRAMMER_H_

#include "dfutypes.h"
#include "inttypes.h"
#include <QString>

namespace MemoryType
{
    enum MemoryType
    {
        EEPROM,
        FLASH,
    };
};


namespace ResetMode
{
    enum ResetMode
    {
        Hard,
        Soft
    };
};


class DFUProgrammer
{
public:
    DFUProgrammer( enum targets_enum target );
    ~DFUProgrammer();
    bool GetDevice();
    bool HasDevice();

    bool EraseDevice();
    bool StartProgramming(MemoryType::MemoryType memory, const QString &HexPath);
    //bool StartVerify();
    bool EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr);
    //const QString &LastError() { return m_sLastError; }
    //bool Program( const QString &sEepromPath, const QString &sFirmwarePath )
    //bool RunFirmware()

protected:
//    virtual void UpdateStatus( ProgramState::ProgramState status ) = 0;

private:
    enum targets_enum m_eTargetDevice;
    struct target_details *m_pDetails;
    struct dfu_device_t *m_pDFUDevice;
    struct usb_device *m_pDevice;

    uint32_t m_top_eeprom_memory_address;
    uint32_t m_flash_address_top;
    uint32_t m_flash_address_bottom;
    uint32_t m_bootloader_bottom;
    uint32_t m_bootloader_top;
};

#endif



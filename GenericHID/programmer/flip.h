#ifndef _FLIP_H_
#define _FLIP_H_

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

class Flip
{
public:
    Flip(void);
    ~Flip(void);

    bool Initialise();
    bool EraseDevice();
    bool StartProgramming(MemoryType::MemoryType memory, const QString &HexPath);
    bool StartVerify();
    bool EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr);
    const QString &LastError() { return m_sLastError; }

private:
    bool loadLibFuncs();

    Logger m_Logger;
    QString m_sDevice;
    int m_nHardware;
    QString m_sLastError;
};

#endif


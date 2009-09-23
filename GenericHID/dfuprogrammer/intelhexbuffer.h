#ifndef _INTELHEXBUFFER_H_
#define _INTELHEXBUFFER_H_

#include "dfutypes.h"
#include "inttypes.h"

class IntelHexBuffer
{
public:
    IntelHexBuffer();
    ~IntelHexBuffer(void);

    bool isEmpty() { return m_HexBuffer.isEmpty(); }
    MemoryType::MemoryType memoryType() { return m_eMemoryType; }
    QVector<int16_t> &data() { return m_HexBuffer; }
    unsigned int usage() { return m_nUsage; }

    bool load(MemoryType::MemoryType memory, const QString &sBuffer, unsigned int max_size);
    bool loadFile(MemoryType::MemoryType memory, const QString &sHexPath, unsigned int max_size);

private:
    QVector<int16_t> m_HexBuffer;
    unsigned int m_nMaxSize;
    MemoryType::MemoryType m_eMemoryType;
    unsigned int m_nUsage;
};


#endif


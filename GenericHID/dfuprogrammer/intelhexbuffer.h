#ifndef _INTELHEXBUFFER_H_
#define _INTELHEXBUFFER_H_

class IntelHexBuffer
{
public:
    IntelHexBuffer(MemoryType::MemoryType memory, const QString &sHexPath, int max_size)
    {
	m_HexBuffer = intel_hex_to_buffer( sHexPath.toAscii().constData(), max_size, &m_nUsage );
	if( m_HexBuffer.isEmpty() ) 
	{
	    DEBUG( "Something went wrong with creating the memory image.\n" );
	    fprintf( stderr, "Something went wrong with creating the memory image.\n" );
	    goto error;
	}
    }

    ~IntelHexBuffer(void) {}

    QVector<int16_t> m_HexBuffer;
    int m_nUsage;
};


#endif


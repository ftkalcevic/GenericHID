#include "dfuprogrammer.h"
#include "atmel.h"
#include "dfu.h"
#include "intel_hex.h"

#define DEBUG(...)

struct target_details
{
    const char *name;
    enum targets_enum value;
    atmel_device_class_t device_type;
    uint16_t chip_id;
    uint16_t vendor_id;
    size_t memory_size;
    size_t bootloader_size;
    bool bootloader_at_highmem;
    size_t flash_page_size;
    bool initial_abort;
    bool honor_interfaceclass;
    size_t eeprom_page_size;
    size_t eeprom_memory_size;
};

/* NOTE FOR: at90usb1287, at90usb1286, at90usb647, at90usb646, at90usb162, at90usb82
 *
 * The actual size of the user-programmable section is limited by the
 * space needed by the bootloader.  The size of the bootloader is set
 * by BOOTSZ0/BOOTSZ1 fuse bits; here we assume the bootloader is 4kb or 8kb.
 * The window used for the bootloader is at the top of the of memory.
 */

/* ----- target specific structures ----------------------------------------- */
static struct target_details target_map[] = {
    { "at89c51snd1c",   tar_at89c51snd1c,   adc_8051,  0x2FFF, 0x03eb, 0x10000, 0x1000, true,  128, false, true,  0,   0      },
    { "at89c51snd2c",   tar_at89c51snd2c,   adc_8051,  0x2FFF, 0x03eb, 0x10000, 0x1000, true,  128, false, true,  0,   0      },
    { "at89c5130",      tar_at89c5130,      adc_8051,  0x2FFD, 0x03eb, 0x04000, 0x0000, true,  128, false, true,  128, 0x03FF },    /* The bootloader is out of the normal flash. */
    { "at89c5131",      tar_at89c5131,      adc_8051,  0x2FFD, 0x03eb, 0x08000, 0x0000, true,  128, false, true,  128, 0x03FF },    /* The bootloader is out of the normal flash. */
    { "at89c5132",      tar_at89c5132,      adc_8051,  0x2FFF, 0x03eb, 0x10000, 0x0C00, true,  128, false, true,  0,   0      },
    { "at90usb1287",    tar_at90usb1287,    adc_AVR,   0x2FFB, 0x03eb, 0x20000, 0x2000, true,  128, true,  false, 128, 0x0FFF },
    { "at90usb1286",    tar_at90usb1286,    adc_AVR,   0x2FFB, 0x03eb, 0x20000, 0x2000, true,  128, true,  false, 128, 0x0FFF },
    { "at90usb1287-4k", tar_at90usb1287_4k, adc_AVR,   0x2FFB, 0x03eb, 0x20000, 0x1000, true,  128, true,  false, 128, 0x0FFF },
    { "at90usb1286-4k", tar_at90usb1286_4k, adc_AVR,   0x2FFB, 0x03eb, 0x20000, 0x1000, true,  128, true,  false, 128, 0x0FFF },
    { "at90usb647",     tar_at90usb647,     adc_AVR,   0x2FF9, 0x03eb, 0x10000, 0x2000, true,  128, true,  false, 128, 0x07FF },
    { "at90usb646",     tar_at90usb646,     adc_AVR,   0x2FF9, 0x03eb, 0x10000, 0x2000, true,  128, true,  false, 128, 0x07FF },
    { "at90usb162",     tar_at90usb162,     adc_AVR,   0x2FFA, 0x03eb, 0x04000, 0x1000, true,  128, true,  false, 128, 0x01FF },
    { "at90usb82",      tar_at90usb82,      adc_AVR,   0x2FF7, 0x03eb, 0x02000, 0x1000, true,  128, true,  false, 128, 0x01FF },
    { "atmega32u6",     tar_atmega32u6,     adc_AVR,   0x2FF2, 0x03eb, 0x08000, 0x1000, true,  128, true,  false, 128, 0x03FF },
    { "atmega32u4",     tar_atmega32u4,     adc_AVR,   0x2FF4, 0x03eb, 0x08000, 0x1000, true,  128, true,  false, 128, 0x03FF },
    { "atmega16u4",     tar_atmega16u4,     adc_AVR,   0x2FF3, 0x03eb, 0x04000, 0x1000, true,  128, true,  false, 128, 0x01FF },
    { "at32uc3a0128",   tar_at32uc3a0128,   adc_AVR32, 0x2FF8, 0x03eb, 0x20000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a1128",   tar_at32uc3a1128,   adc_AVR32, 0x2FF8, 0x03eb, 0x20000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a0256",   tar_at32uc3a0256,   adc_AVR32, 0x2FF8, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a1256",   tar_at32uc3a1256,   adc_AVR32, 0x2FF8, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a0512",   tar_at32uc3a0512,   adc_AVR32, 0x2FF8, 0x03eb, 0x80000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a1512",   tar_at32uc3a1512,   adc_AVR32, 0x2FF8, 0x03eb, 0x80000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a0512es", tar_at32uc3a0512es, adc_AVR32, 0x2FF8, 0x03eb, 0x80000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a1512es", tar_at32uc3a1512es, adc_AVR32, 0x2FF8, 0x03eb, 0x80000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a364",    tar_at32uc3a364,    adc_AVR32, 0x2FF1, 0x03eb, 0x10000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a364s",   tar_at32uc3a364s,   adc_AVR32, 0x2FF1, 0x03eb, 0x10000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a3128",   tar_at32uc3a3128,   adc_AVR32, 0x2FF1, 0x03eb, 0x20000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a3128s",  tar_at32uc3a3128s,  adc_AVR32, 0x2FF1, 0x03eb, 0x20000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a3256",   tar_at32uc3a3256,   adc_AVR32, 0x2FF1, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3a3256s",  tar_at32uc3a3256s,  adc_AVR32, 0x2FF1, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b064",    tar_at32uc3b064,    adc_AVR32, 0x2FF6, 0x03eb, 0x10000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b164",    tar_at32uc3b164,    adc_AVR32, 0x2FF6, 0x03eb, 0x10000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b0128",   tar_at32uc3b0128,   adc_AVR32, 0x2FF6, 0x03eb, 0x20000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b1128",   tar_at32uc3b1128,   adc_AVR32, 0x2FF6, 0x03eb, 0x20000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b0256",   tar_at32uc3b0256,   adc_AVR32, 0x2FF6, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b1256",   tar_at32uc3b1256,   adc_AVR32, 0x2FF6, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b0256es", tar_at32uc3b0256es, adc_AVR32, 0x2FF6, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { "at32uc3b1256es", tar_at32uc3b1256es, adc_AVR32, 0x2FF6, 0x03eb, 0x40000, 0x2000, false, 512, false, true,  0,   0      },
    { NULL }
};

struct target_details *FindDetails( enum targets_enum target )
{
    for ( int i = 0; i < countof(target_map); i++ )
	if ( target_map[i].value == target )
	    return target_map+i;
    return NULL;
}


DFUProgrammer::DFUProgrammer( enum targets_enum target )
: m_eTargetDevice( target )
, m_pDevice( NULL )
{
    usb_init();
    m_pDFUDevice = new dfu_device_t;
    memset( m_pDFUDevice, 0, sizeof(dfu_device_t) );

    m_pDetails = FindDetails( target );

    m_flash_address_top = m_pDetails->memory_size;
    m_flash_address_bottom = 0;
    m_bootloader_bottom = 0;
    m_bootloader_top = 0;
    if( m_pDetails->bootloader_at_highmem ) 
    {
	m_bootloader_bottom = m_flash_address_top - m_pDetails->bootloader_size;
        m_bootloader_top = m_flash_address_top;
        m_flash_address_top -= (m_pDetails->bootloader_size + 1);
    } 
    else 
    {
	m_bootloader_bottom = m_flash_address_bottom;
        m_bootloader_top += m_pDetails->bootloader_size;
        m_flash_address_bottom += m_pDetails->bootloader_size;
    }

    if( m_pDetails->eeprom_memory_size > 0 ) 
    {
        m_top_eeprom_memory_address = m_pDetails->eeprom_memory_size - 1;
    } 
    else 
    {
	m_top_eeprom_memory_address = 0;
    }
}

DFUProgrammer::~DFUProgrammer()
{
}

bool DFUProgrammer::GetDevice()
{
    m_pDevice = dfu_device_init( m_pDetails->vendor_id, m_pDetails->chip_id, m_pDFUDevice,
				 m_pDetails->initial_abort, m_pDetails->honor_interfaceclass );

    // Can we get boot device sizes when we connect to correct flash size for boot loader.
    if( m_pDevice == NULL )
	return false;
    else
	return true;
}

bool DFUProgrammer::HasDevice()
{
    if ( m_pDevice == NULL )
	return false;

    // todo: test if we still have the device
    return true;
}

bool DFUProgrammer::EraseDevice()
{
    if ( m_pDevice == NULL )
	return false;

    int result = atmel_erase_flash( m_pDFUDevice, ATMEL_ERASE_ALL );
    if( result != 0 ) 
    {
	return false;
    }

    result = atmel_blank_check( m_pDFUDevice, m_flash_address_bottom, m_flash_address_top, m_Callback, m_user_data );

    if ( result != 0 )
    {
	return false;
    }

    return true;
}

bool DFUProgrammer::StartProgramming(IntelHexBuffer &memory)
{
    int32_t  usage = 0;
    int32_t  retval = -1;
    int32_t  result = 0;
    uint32_t memory_size;
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    uint32_t page_size = m_pDetails->flash_page_size;
    bool bEeprom = memory.memoryType() == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
        page_size = m_pDetails->eeprom_page_size;
    }

    memory_size = top_memory_address - bottom_memory_address;
    QVector<byte> buffer( memory_size, 0 );

    for( unsigned int i = 0; i < bottom_memory_address; i++ ) 
    {
        if( memory.data()[i] != -1 ) 
	{
            fprintf( stderr, "Attempted to write to illegal memory address.\n" );
            goto error;
        }
    }
    if( !bEeprom ) 
    {	
        for( unsigned int i = m_bootloader_bottom; i < m_bootloader_top && i < top_memory_address; i++) 
	{
            if( -1 != memory.data()[i] ) 
	    {
                //if( m_pDetails->suppressbootloader ) 
		//{
  //                  //If we're ignoring the bootloader, don't bother writing to it
  //                  memory.data()[i] = -1;
  //              } 
		//else 
		{
                    fprintf( stderr, "Bootloader and code overlap.\n" );
                    fprintf( stderr, "Use --suppress-bootloader-mem to ignore\n" );
                    goto error;
                }
            }
        }
    }

    DEBUG( "write %d/%d bytes\n", usage, memory_size );

    result = atmel_flash( m_pDFUDevice, memory.data().data(), bottom_memory_address, top_memory_address, page_size, bEeprom, m_Callback, m_user_data );

    if( result < 0 ) 
    {
        DEBUG( "Error while flashing. (%d)\n", result );
        fprintf( stderr, "Error while flashing.\n" );
        goto error;
    }

    fprintf( stderr, "%d bytes used (%.02f%%)\n", usage,
                     ((float)(usage*100)/(float)(top_memory_address)) );

    retval = 0;

error:

    if ( retval == 0 )
	return true;
    else
	return false;
}

bool DFUProgrammer::StartVerify(IntelHexBuffer &memory)
{
    int32_t  retval = -1;
    int32_t  result = 0;
    uint32_t memory_size;
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    uint32_t page_size = m_pDetails->flash_page_size;
    bool bEeprom = memory.memoryType() == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
        page_size = m_pDetails->eeprom_page_size;
    }

    memory_size = top_memory_address - bottom_memory_address;
    QVector<byte> buffer( memory_size, 0 );

    fprintf( stderr, "Validating...\n" );

    result = atmel_read_flash( m_pDFUDevice, bottom_memory_address,
                               top_memory_address, buffer.data(),
                               memory_size, bEeprom, false, m_Callback, m_user_data );

    if( memory_size != result ) 
    {
        DEBUG( "Error while validating.\n" );
        fprintf( stderr, "Error while validating.\n" );
        goto error;
    }

    for( int i = 0, j = bottom_memory_address; i < result; i++, j++ ) 
    {
        if( (0 <= memory.data()[j]) && (memory.data()[j] < UINT8_MAX) ) 
	{
            /* Memory should have been programmed in this location. */
            if( ((uint8_t) memory.data()[j]) != buffer[i] ) 
	    {
                DEBUG( "Image did not validate at location: %d (%02x != %02x)\n", i,
                       (0xff & memory.data()[j]), (0xff & buffer[i]) );
                fprintf( stderr, "Image did not validate.\n" );
                goto error;
            }
        }
    }

    fprintf( stderr, "%d bytes used (%.02f%%)\n", memory.usage(),
                     ((float)(memory.usage()*100)/(float)(top_memory_address)) );

    retval = 0;

error:
    if ( retval == 0 )
	return true;
    else
	return false;
}


bool DFUProgrammer::EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr)
{
    int result;
    if ( mode == ResetMode::Hard )
	result = atmel_reset( m_pDFUDevice ); 
    else
	result = atmel_start_app( m_pDFUDevice, addr );

    if ( result != 0 )
	return false;
    else
	return true;
}

    //const QString &LastError() { return m_sLastError; }




IntelHexBuffer DFUProgrammer::LoadHex(MemoryType::MemoryType memtype, const QString &sPath)
{
    uint32_t memory_size;
    uint32_t top_memory_address = m_flash_address_top;
    uint32_t bottom_memory_address = m_flash_address_bottom;
    uint32_t page_size = m_pDetails->flash_page_size;
    bool bEeprom = memtype == MemoryType::EEPROM;

    if( bEeprom ) 
    {
        top_memory_address = m_top_eeprom_memory_address;
        bottom_memory_address = 0;
        page_size = m_pDetails->eeprom_page_size;
    }

    memory_size = top_memory_address - bottom_memory_address;

    IntelHexBuffer buffer( memtype, sPath.toAscii().constData(), top_memory_address );

    return buffer;
}

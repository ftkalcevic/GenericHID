#include "stdafx.h"
#include "flip.h"

#include "AtJniIsp.h"

enum { CLOSED, OPEN };

enum { FAIL, PASS };


#define DO_LOG_MSG( logger, type, str )	    { m_sLastError = str; LOG_MSG( logger, type, m_sLastError ); }

#define DYNCALL(FUNC) lp##FUNC
#define DECLARE_FUNCTION_POINTER(FUNC)  static PF_##FUNC lp##FUNC=NULL;  
#define EXTERN_FUNCTION_POINTER(FUNC)  extern PF_##FUNC lp##FUNC;
#define ADDR_CHECK(FUNC) if (lp##FUNC == NULL) {DO_LOG_MSG(m_Logger, LogTypes::Error, QString("Missing function in DLL '%1'").arg(#FUNC));}

#ifdef WIN32
    #define LOAD_FUNCTION_POINTER(DLL,FUNC) lp##FUNC = (PF_##FUNC)GetProcAddress(DLL, #FUNC);
    const char * const FLIP_LIBRARY = "AtJniIsp.dll";
#else
    #include <dlfcn.h>
    #define LOAD_FUNCTION_POINTER(DLL,FUNC) lp##FUNC = (PF_##FUNC)dlsym(DLL, #FUNC);
    const char * const FLIP_LIBRARY = "libatjniisp.so";
#endif


static bool bDllLoaded = false;

// Function pointers.
DECLARE_FUNCTION_POINTER(ispInit);
DECLARE_FUNCTION_POINTER(ispSetLogEnabled);
DECLARE_FUNCTION_POINTER(ispSelectDevice);
DECLARE_FUNCTION_POINTER(ispSelectCommHardware);
DECLARE_FUNCTION_POINTER(ispOpenRs232Port);
DECLARE_FUNCTION_POINTER(ispSyncTarget);
DECLARE_FUNCTION_POINTER(ispParseHexFile);
DECLARE_FUNCTION_POINTER(ispBlankCheckMemory);
DECLARE_FUNCTION_POINTER(ispProgramDevice);
DECLARE_FUNCTION_POINTER(ispProgramAddrRange);
DECLARE_FUNCTION_POINTER(ispReadDevice);
DECLARE_FUNCTION_POINTER(ispReadAddrRange);
DECLARE_FUNCTION_POINTER(ispVerifyDevice);
DECLARE_FUNCTION_POINTER(ispGetLastError);
DECLARE_FUNCTION_POINTER(ispExit);
DECLARE_FUNCTION_POINTER(ispUpdateCmdsIds);
DECLARE_FUNCTION_POINTER(ispOpenCanPort);
DECLARE_FUNCTION_POINTER(ispOpenUsbPort);
DECLARE_FUNCTION_POINTER(ispSelectCanNode);
DECLARE_FUNCTION_POINTER(ispSelectMemory);
DECLARE_FUNCTION_POINTER(ispEraseDevice);
DECLARE_FUNCTION_POINTER(ispGetDeviceIndex);
DECLARE_FUNCTION_POINTER(ispDataLog);
DECLARE_FUNCTION_POINTER(ispSetDebugMode);
DECLARE_FUNCTION_POINTER(ispOpenLogFile);
DECLARE_FUNCTION_POINTER(ispExecCmd);
DECLARE_FUNCTION_POINTER(ispGetLastResult);
DECLARE_FUNCTION_POINTER(ispGetMemorySize);
DECLARE_FUNCTION_POINTER(ispReadSignatureBytes);
DECLARE_FUNCTION_POINTER(ispReadSpecialByte);
DECLARE_FUNCTION_POINTER(ispBlankCheckAddrRange);
DECLARE_FUNCTION_POINTER(ispFillBufferAddrRange);
DECLARE_FUNCTION_POINTER(ispSetSecurityLevel);
DECLARE_FUNCTION_POINTER(ispGetErrorVecPtr);
DECLARE_FUNCTION_POINTER(ispGetBufferAddrHi);
DECLARE_FUNCTION_POINTER(ispGetBufferAddrLo);
DECLARE_FUNCTION_POINTER(ispSerialize);
DECLARE_FUNCTION_POINTER(ispWriteSpecialByte);
DECLARE_FUNCTION_POINTER(ispGetLastErrorMssg);
DECLARE_FUNCTION_POINTER(ispGetErrorMssgFromIdx);
DECLARE_FUNCTION_POINTER(ispSetTimeout);
DECLARE_FUNCTION_POINTER(ispSetAutoIsp);
DECLARE_FUNCTION_POINTER(ispWriteHexFile);
DECLARE_FUNCTION_POINTER(ispSetDtrLow);
DECLARE_FUNCTION_POINTER(ispSetDtrHigh);
DECLARE_FUNCTION_POINTER(ispSetRtsLow);
DECLARE_FUNCTION_POINTER(ispSetRtsHigh);
DECLARE_FUNCTION_POINTER(ispEnterAppliMode);
DECLARE_FUNCTION_POINTER(ispEnterIspMode);



static const char *FlipError(int status)
{
    switch ( status )
    {	
	case STATUS_PASS:				    return "PASS"; 
	case STATUS_MEMORY_ALLOC_ERROR:			    return "MEMORY_ALLOC_ERROR"; 
	case STATUS_ADDRESS_OUT_OF_RANGE_ERROR:		    return "ADDRESS_OUT_OF_RANGE_ERROR"; 
	case STATUS_FUNCTION_NOT_IMPLEMENTED_ERROR:	    return "FUNCTION_NOT_IMPLEMENTED_ERROR"; 
	case STATUS_TIMEOUT_ERROR:			    return "TIMEOUT_ERROR"; 
	case STATUS_READING_ERROR:			    return "READING_ERROR"; 
	case STATUS_CHECKSUM_ERROR:			    return "CHECKSUM_ERROR"; 
	case STATUS_DEVICE_DOES_NOT_EXIST:		    return "DEVICE_DOES_NOT_EXIST";  
	case STATUS_PROTOCOL_DOES_NOT_EXIST:		    return "PROTOCOL_DOES_NOT_EXIST"; 
	case STATUS_CANNOT_EXECUTE_COMMAND:		    return "CANNOT_EXECUTE_COMMAND"; 
	case STATUS_INVALID_DEVICE_HARDWARE_ASSOCIATION:    return "INVALID_DEVICE_HARDWARE_ASSOCIATION"; 	
	case STATUS_CANNOT_CREATE_DEVICE_OBJECT:	    return "CANNOT_CREATE_DEVICE_OBJECT"; 
	case STATUS_CANNOT_CREATE_PROTOCOL_OBJECT:	    return "CANNOT_CREATE_PROTOCOL_OBJECT"; 
	case STATUS_HARDWARE_DOES_NOT_EXIST:		    return "HARDWARE_DOES_NOT_EXIST"; 
	case STATUS_COMMAND_DOES_NOT_EXIST:		    return "COMMAND_DOES_NOT_EXIST"; 
	case STATUS_OPEN_PORT_ERROR:			    return "OPEN_PORT_ERROR"; 
	case STATUS_CLOSE_PORT_ERROR:			    return "CLOSE_PORT_ERROR"; 
	case STATUS_CANNOT_WRITE_TO_PORT:		    return "CANNOT_WRITE_TO_PORT"; 
	case STATUS_POSSIBLE_FRAMING_ERROR:		    return "POSSIBLE_FRAMING_ERROR"; 
	case STATUS_WRITE_PROTECTION_ERROR:		    return "WRITE_PROTECTION_ERROR"; 
	case STATUS_READ_WRITE_PROTECTION_ERROR:	    return "READ_WRITE_PROTECTION_ERROR"; 
	case STATUS_SOFTWARE_SECURITY_ERROR:		    return "SOFTWARE_SECURITY_ERROR"; 
	case STATUS_CANNOT_OPEN_FILE:			    return "CANNOT_OPEN_FILE"; 
	case STATUS_INVALID_HEX_FILE_SYNTAX:		    return "INVALID_HEX_FILE_SYNTAX"; 
	case STATUS_INVALID_HEX_FILE_CHECKSUM:		    return "INVALID_HEX_FILE_CHECKSUM"; 
	case STATUS_INVALID_HEXFILE_PATHNAME:		    return "INVALID_HEXFILE_PATHNAME"; 
	case STATUS_VERIFY_DEVICE_ERROR:		    return "VERIFY_DEVICE_ERROR"; 
	case STATUS_TARGET_REPLY_ERROR:			    return "TARGET_REPLY_ERROR"; 
	case STATUS_BLANK_CHECK_ERROR:			    return "BLANK_CHECK_ERROR"; 
	case STATUS_INVALID_CAN_MESSAGE_LENGTH_ERROR:	    return "INVALID_CAN_MESSAGE_LENGTH_ERROR"; 
	case STATUS_INVALID_CAN_MESSAGE_ID_ERROR:	    return "INVALID_CAN_MESSAGE_ID_ERROR"; 
	case STATUS_CAN_NODE_IS_CLOSED_ERROR:		    return "CAN_NODE_IS_CLOSED_ERROR"; 
	case STATUS_COMMAND_EXECUTION_ERROR:		    return "COMMAND_EXECUTION_ERROR"; 
	case STATUS_INVALID_MEMORY_CONFIG_ERROR:	    return "INVALID_MEMORY_CONFIG_ERROR"; 
	case STATUS_INVALID_BOOTLOADER_REPLY_ERROR:	    return "INVALID_BOOTLOADER_REPLY_ERROR"; 
	case STATUS_INVALID_CRIS_VALUE_ERROR:		    return "INVALID_CRIS_VALUE_ERROR"; 
	case STATUS_NO_ARGUMENT_ERROR:			    return "NO_ARGUMENT_ERROR"; 
	case STATUS_MISSING_PARAMETER:			    return "MISSING_PARAMETER"; 
	case STATUS_MISSING_DEVICE_SWITCH:		    return "MISSING_DEVICE_SWITCH"; 
	case STATUS_MISSING_OPERATION_SWITCH:		    return "MISSING_OPERATION_SWITCH"; 
	case STATUS_INVALID_NUMBER_OF_DEVICE_ARGUMENT:	    return "INVALID_NUMBER_OF_DEVICE_ARGUMENT"; 
	case STATUS_INVALID_PORT_SELECTION:		    return "INVALID_PORT_SELECTION"; 
	case STATUS_INVALID_BAUDRATE:			    return "INVALID_BAUDRATE"; 
	case STATUS_INVALID_CANBITRATE:			    return "INVALID_CANBITRATE"; 
	case STATUS_NO_OPERATION_SPECIFIED:		    return "NO_OPERATION_SPECIFIED"; 
	case STATUS_INVALID_OPERATION:			    return "INVALID_OPERATION"; 
	case STATUS_INVALID_ARGUMENT:			    return "INVALID_ARGUMENT"; 
	case STATUS_TOO_MANY_OPERATIONS:		    return "TOO_MANY_OPERATIONS"; 
	case STATUS_INVALID_CMDFILE_ARGUMENT:		    return "INVALID_CMDFILE_ARGUMENT"; 
	case STATUS_NO_WINDRIVER_LICENSE_ERROR:		    return "NO_WINDRIVER_LICENSE_ERROR"; 
	case STATUS_NO_USB_DEVICE_FOUND_ERROR:		    return "NO_USB_DEVICE_FOUND_ERROR"; 
	case STATUS_COULD_NOT_OPEN_USB_DEVICE_ERROR:	    return "COULD_NOT_OPEN_USB_DEVICE_ERROR"; 
	case STATUS_COULD_NOT_WRITE_USB_DEVICE_ERROR:	    return "COULD_NOT_WRITE_USB_DEVICE_ERROR"; 
	case STATUS_COULD_NOT_READ_USB_DEVICE_ERROR:	    return "COULD_NOT_READ_USB_DEVICE_ERROR"; 
	case STATUS_CANNOT_LOAD_DLL:			    return "CANNOT_LOAD_DLL"; 
	case STATUS_SN_OUT_OF_RANGE_ERROR:		    return "SN_OUT_OF_RANGE_ERROR"; 
	case STATUS_INVALID_SERIALIZE_ARGUMENT:		    return "INVALID_SERIALIZE_ARGUMENT"; 
	case STATUS_SN_IS_64_BITS_ERROR:		    return "SN_IS_64_BITS_ERROR"; 
	case STATUS_JVM_ERROR:				    return "JVM_ERROR"; 
	case STATUS_OCD_ERROR:				    return "OCD_ERROR"; 
	default:					    return "UKNOWN_ERROR";
    }
}



Flip::Flip(void)
: m_Logger( QCoreApplication::applicationName(), "Flip" )
{
    m_sDevice = "AT90USB1287";
    m_nHardware = USB_PORT;
}

Flip::~Flip(void)
{
}

bool Flip::Initialise()
{
    if ( !loadLibFuncs() )
	return false;

    if (DYNCALL(ispInit()) != PASS) 
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to load initialise flip library: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }

    if (DYNCALL(ispSelectDevice( m_sDevice.toAscii().constData())) != PASS) 
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to Select USB Device: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }

    if (DYNCALL(ispSelectCommHardware(m_nHardware)) != PASS) 
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to select Flip communication hardware %1: '%2' (%3)").arg(m_nHardware).arg(FlipError(nError)).arg(nError) );
	return false;
    }

    if (m_nHardware == USB_PORT)
    {
        if (DYNCALL(ispOpenUsbPort()) != PASS)
	{
	    int nError = DYNCALL(ispGetLastError());
	    DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to open USB port: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	    return false;
	}
    }
    else
    {
	DO_LOG_MSG( m_Logger, LogTypes::Error, "Only USB is supported" );
	return false;
    }

    return true;
}

bool Flip::EraseDevice()
{
    if (DYNCALL(ispEraseDevice()) != PASS)
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to erase device: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }
    return true;
}


bool Flip::StartProgramming(MemoryType::MemoryType memory, const QString &sHexPath)
{
    int nType = memory == MemoryType::EEPROM ? EEPROM : FLASH;

    if (DYNCALL(ispSelectMemory(nType)) != PASS )
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed select memory type '%1': '%2' (%3)").arg(nType).arg(FlipError(nError)).arg(nError) );
	return false;
    }

    if (DYNCALL(ispParseHexFile(sHexPath.toAscii().data())) != PASS )
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to parse hex file '%1': '%2' (%3)").arg(sHexPath).arg(FlipError(nError)).arg(nError) );
	return false;
    }

    if (DYNCALL(ispProgramDevice()) != PASS )
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to program device: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }
    return true;
}

bool Flip::StartVerify()
{
    QString sAddrLo;
    QString sAddrHi;

    // The address programming range is defined by the last HEX file parsing operation.
    // We can get the address min/max values by calling the ispGetBufferAddrLo/Hi functions.
    // For some bizarre reason, the addresses come back as strings.
    if ( DYNCALL(ispGetBufferAddrLo()) != PASS )
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to retreive buffer address low: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }

    sAddrLo = DYNCALL(ispGetLastResult());

    if ( DYNCALL(ispGetBufferAddrHi()) != PASS )
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to retreive buffer address high: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }
    sAddrHi = DYNCALL(ispGetLastResult());

    unsigned int nAddrLo = sAddrLo.toUInt(NULL,0);
    unsigned int nAddrHi = sAddrHi.toUInt(NULL,0);

    if ( DYNCALL(ispVerifyDevice(nAddrLo, nAddrHi)) != PASS )
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to verify the device: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }

    return true;
}


bool Flip::EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr)
{
    bool bHardReset;
    if (mode == ResetMode::Hard)
        bHardReset = true;
    else
        bHardReset = false;

    if ( DYNCALL(ispEnterAppliMode(bHardReset, addr)) != PASS )
    {
	int nError = DYNCALL(ispGetLastError());
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to enter application mode: '%1' (%2)").arg(FlipError(nError)).arg(nError) );
	return false;
    }

    return true;
}



bool Flip::loadLibFuncs() 
{
    if ( bDllLoaded )
	return true;

#ifdef WIN32
    HINSTANCE h;
    if ((h = LoadLibraryA(FLIP_LIBRARY)) == NULL) 
    {
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to load flip library '%1': x%2").arg(FLIP_LIBRARY).arg(::GetLastError(),8,16,QChar('0')) );
	return false;
    }
#else
    void* h = NULL;
    if ((h = dlopen(FLIP_LIBRARY, RTLD_NOW)) == NULL) 
    {
	DO_LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to load flip library '%1': %2").arg(FLIP_LIBRARY).arg(dlerror()) );
	return false;
    }
#endif

    LOAD_FUNCTION_POINTER(h, ispInit); 
    ADDR_CHECK(ispInit);
    LOAD_FUNCTION_POINTER(h, ispSetLogEnabled); 
    ADDR_CHECK(ispSetLogEnabled);
    LOAD_FUNCTION_POINTER(h, ispSelectDevice);
    ADDR_CHECK(ispSelectDevice);
    LOAD_FUNCTION_POINTER(h, ispSelectCommHardware);
    ADDR_CHECK(ispSelectCommHardware);
    LOAD_FUNCTION_POINTER(h, ispOpenRs232Port);
    ADDR_CHECK(ispOpenRs232Port);
    LOAD_FUNCTION_POINTER(h, ispSyncTarget);
    ADDR_CHECK(ispSyncTarget);
    LOAD_FUNCTION_POINTER(h, ispParseHexFile);
    ADDR_CHECK(ispParseHexFile);
    LOAD_FUNCTION_POINTER(h, ispBlankCheckMemory);
    ADDR_CHECK(ispBlankCheckMemory);
    LOAD_FUNCTION_POINTER(h, ispProgramDevice);
    ADDR_CHECK(ispProgramDevice);
    LOAD_FUNCTION_POINTER(h, ispProgramAddrRange);
    ADDR_CHECK(ispProgramAddrRange);
    LOAD_FUNCTION_POINTER(h, ispReadDevice);
    ADDR_CHECK(ispReadDevice);
    LOAD_FUNCTION_POINTER(h, ispReadAddrRange);
    ADDR_CHECK(ispReadAddrRange);
    LOAD_FUNCTION_POINTER(h, ispVerifyDevice);
    ADDR_CHECK(ispVerifyDevice);
    LOAD_FUNCTION_POINTER(h, ispGetLastError);
    ADDR_CHECK(ispGetLastError);
    LOAD_FUNCTION_POINTER(h, ispExit);
    ADDR_CHECK(ispExit);
    LOAD_FUNCTION_POINTER(h, ispUpdateCmdsIds);
    ADDR_CHECK(ispUpdateCmdsIds);
    LOAD_FUNCTION_POINTER(h, ispOpenCanPort);
    ADDR_CHECK(ispOpenCanPort);
    LOAD_FUNCTION_POINTER(h, ispOpenUsbPort);
    ADDR_CHECK(ispOpenUsbPort);
    LOAD_FUNCTION_POINTER(h, ispSelectCanNode);
    ADDR_CHECK(ispSelectCanNode);
    LOAD_FUNCTION_POINTER(h, ispSelectMemory);
    ADDR_CHECK(ispSelectMemory);
    LOAD_FUNCTION_POINTER(h, ispEraseDevice);
    ADDR_CHECK(ispEraseDevice);
    LOAD_FUNCTION_POINTER(h, ispGetDeviceIndex);
    ADDR_CHECK(ispGetDeviceIndex);
    LOAD_FUNCTION_POINTER(h, ispDataLog);
    ADDR_CHECK(ispDataLog);    
    LOAD_FUNCTION_POINTER(h, ispSetDebugMode);
    ADDR_CHECK(ispSetDebugMode);
    LOAD_FUNCTION_POINTER(h, ispOpenLogFile);
    ADDR_CHECK(ispOpenLogFile);
    LOAD_FUNCTION_POINTER(h, ispExecCmd);
    ADDR_CHECK(ispExecCmd);
    LOAD_FUNCTION_POINTER(h, ispGetLastResult);
    ADDR_CHECK(ispGetLastResult);
    LOAD_FUNCTION_POINTER(h, ispGetMemorySize);
    ADDR_CHECK(ispGetMemorySize);
    LOAD_FUNCTION_POINTER(h, ispReadSignatureBytes);
    ADDR_CHECK(ispReadSignatureBytes);
    LOAD_FUNCTION_POINTER(h, ispReadSpecialByte);
    ADDR_CHECK(ispReadSpecialByte);
    LOAD_FUNCTION_POINTER(h, ispBlankCheckAddrRange);
    ADDR_CHECK(ispBlankCheckAddrRange);
    LOAD_FUNCTION_POINTER(h, ispFillBufferAddrRange);
    ADDR_CHECK(ispFillBufferAddrRange);
    LOAD_FUNCTION_POINTER(h, ispSetSecurityLevel);
    ADDR_CHECK(ispSetSecurityLevel);
    LOAD_FUNCTION_POINTER(h, ispGetErrorVecPtr);
    ADDR_CHECK(ispGetErrorVecPtr);
    LOAD_FUNCTION_POINTER(h, ispGetBufferAddrHi);
    ADDR_CHECK(ispGetBufferAddrHi);
    LOAD_FUNCTION_POINTER(h, ispGetBufferAddrLo);
    ADDR_CHECK(ispGetBufferAddrLo);
    LOAD_FUNCTION_POINTER(h, ispSerialize);
    ADDR_CHECK(ispSerialize);
    LOAD_FUNCTION_POINTER(h, ispWriteSpecialByte);
    ADDR_CHECK(ispWriteSpecialByte);
    LOAD_FUNCTION_POINTER(h, ispGetLastErrorMssg);
    ADDR_CHECK(ispGetLastErrorMssg);
    LOAD_FUNCTION_POINTER(h, ispGetErrorMssgFromIdx);
    ADDR_CHECK(ispGetErrorMssgFromIdx);
    LOAD_FUNCTION_POINTER(h, ispSetTimeout);
    ADDR_CHECK(ispSetTimeout);
    LOAD_FUNCTION_POINTER(h, ispSetAutoIsp);
    ADDR_CHECK(ispSetAutoIsp);
    LOAD_FUNCTION_POINTER(h, ispWriteHexFile);
    ADDR_CHECK(ispWriteHexFile);
    LOAD_FUNCTION_POINTER(h, ispSetDtrLow);
    ADDR_CHECK(ispSetDtrLow);
    LOAD_FUNCTION_POINTER(h, ispSetDtrHigh);
    ADDR_CHECK(ispSetDtrHigh);
    LOAD_FUNCTION_POINTER(h, ispSetRtsLow);
    ADDR_CHECK(ispSetRtsLow);
    LOAD_FUNCTION_POINTER(h, ispSetRtsHigh);
    ADDR_CHECK(ispSetRtsHigh);
    LOAD_FUNCTION_POINTER(h, ispEnterAppliMode);
    ADDR_CHECK(ispEnterAppliMode);
    LOAD_FUNCTION_POINTER(h, ispEnterIspMode);
    ADDR_CHECK(ispEnterIspMode);

    bDllLoaded = true;

    return true;
}


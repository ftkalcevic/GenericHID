// This is the header file for the FLIP 3.3.2 library

#ifndef _ATJNIISP_H_
#define _ATJNIISP_H_
/* ________________________________ INCLUDES _____________________________________
*/
#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
//#include "compiler.h"
#endif

#include <vector>
#include <string>

//#include "jni.h"

using namespace std;


/* ________________________________ MACROS _____________________________________
*/

#ifdef WIN32

    // Implement the DLL export/import mechanism and allow a C-written program
    // to use our DLL.
    #ifdef ATJNIISP_EXPORTS
        #define ATJNIISP_API extern "C" __declspec(dllexport)
    #else
        #define ATJNIISP_API extern "C" __declspec(dllimport)
    #endif

    // This macro function calls the C runtime's _beginthreadex function. 
    // The C runtime library doesn't want to have any reliance on Windows' data 
    // types such as HANDLE. This means that a Windows programmer needs to cast
    // values when using _beginthreadex. Since this is terribly inconvenient, 
    // this macro has been created to perform the casting.
    typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadId)                 \
      ((HANDLE)_beginthreadex(                      \
         (void *)        (psa),                     \
         (unsigned)      (cbStack),                 \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *)        (pvParam),                 \
         (unsigned)      (fdwCreate),               \
         (unsigned *)    (pdwThreadId)))

    // Allow applications not built with Microsoft Visual C++ to link with our DLL.

    #define STDCALL __stdcall
    #define PATH_SEPARATOR ";"
#else
    // Linux specific
    #define ATJNIISP_API extern "C"
    #define STDCALL
    #define PATH_SEPARATOR ":"
    #define WINAPI
    typedef unsigned char UCHAR;
    typedef unsigned int DWORD;
    typedef unsigned long ULONG;
    typedef unsigned char BYTE;
    typedef void * HANDLE;
    typedef long long LONG64;
#endif//WIN32

#define READ_ERROR "error"
#define TIMEOUT_ERROR ""           
#define REGISTRY_BUFSIZE 1024
#define HANDLE_JVM_ERROR     if (pEnv->ExceptionCheck()) {pEnv->ExceptionDescribe(); gpErrorMngr->setLastError(STATUS_JVM_ERROR); throw STATUS_JVM_ERROR; }
#define ATMEL_USB_VID 0x03EB

/* ________________________________ CONSTANTS _____________________________________
*/

// ISP library status report.
enum STATUS
{
    STATUS_PASS,
    STATUS_MEMORY_ALLOC_ERROR,
    STATUS_ADDRESS_OUT_OF_RANGE_ERROR,
    STATUS_FUNCTION_NOT_IMPLEMENTED_ERROR,
    STATUS_TIMEOUT_ERROR,
    STATUS_READING_ERROR,
    STATUS_CHECKSUM_ERROR,
    STATUS_DEVICE_DOES_NOT_EXIST, 
    STATUS_PROTOCOL_DOES_NOT_EXIST,
    STATUS_CANNOT_EXECUTE_COMMAND,
    STATUS_INVALID_DEVICE_HARDWARE_ASSOCIATION,
    STATUS_CANNOT_CREATE_DEVICE_OBJECT,
    STATUS_CANNOT_CREATE_PROTOCOL_OBJECT,
    STATUS_HARDWARE_DOES_NOT_EXIST,
    STATUS_COMMAND_DOES_NOT_EXIST,
    STATUS_OPEN_PORT_ERROR,
    STATUS_CLOSE_PORT_ERROR,
    STATUS_CANNOT_WRITE_TO_PORT,
    STATUS_POSSIBLE_FRAMING_ERROR,
    STATUS_WRITE_PROTECTION_ERROR,
    STATUS_READ_WRITE_PROTECTION_ERROR,
    STATUS_SOFTWARE_SECURITY_ERROR,
    STATUS_CANNOT_OPEN_FILE,
    STATUS_INVALID_HEX_FILE_SYNTAX,
    STATUS_INVALID_HEX_FILE_CHECKSUM,
    STATUS_INVALID_HEXFILE_PATHNAME,
    STATUS_VERIFY_DEVICE_ERROR,
    STATUS_TARGET_REPLY_ERROR,
    STATUS_BLANK_CHECK_ERROR,
    STATUS_INVALID_CAN_MESSAGE_LENGTH_ERROR,
    STATUS_INVALID_CAN_MESSAGE_ID_ERROR,
    STATUS_CAN_NODE_IS_CLOSED_ERROR,
    STATUS_COMMAND_EXECUTION_ERROR,
    STATUS_INVALID_MEMORY_CONFIG_ERROR,
    STATUS_INVALID_BOOTLOADER_REPLY_ERROR,
    STATUS_INVALID_CRIS_VALUE_ERROR,
    STATUS_NO_ARGUMENT_ERROR,
    STATUS_MISSING_PARAMETER,
    STATUS_MISSING_DEVICE_SWITCH,
    STATUS_MISSING_OPERATION_SWITCH,
    STATUS_INVALID_NUMBER_OF_DEVICE_ARGUMENT,
    STATUS_INVALID_PORT_SELECTION,
    STATUS_INVALID_BAUDRATE,
    STATUS_INVALID_CANBITRATE,
    STATUS_NO_OPERATION_SPECIFIED,
    STATUS_INVALID_OPERATION,
    STATUS_INVALID_ARGUMENT,
    STATUS_TOO_MANY_OPERATIONS,
    STATUS_INVALID_CMDFILE_ARGUMENT,
    STATUS_NO_WINDRIVER_LICENSE_ERROR,
    STATUS_NO_USB_DEVICE_FOUND_ERROR,
    STATUS_COULD_NOT_OPEN_USB_DEVICE_ERROR,
    STATUS_COULD_NOT_WRITE_USB_DEVICE_ERROR,
    STATUS_COULD_NOT_READ_USB_DEVICE_ERROR,
    STATUS_CANNOT_LOAD_DLL,
    STATUS_SN_OUT_OF_RANGE_ERROR,
    STATUS_INVALID_SERIALIZE_ARGUMENT,
    STATUS_SN_IS_64_BITS_ERROR,
    STATUS_JVM_ERROR,
    STATUS_OCD_ERROR
};


enum PORT_TYPE
{
    RS232_HW = 1,
    USB_HW,
    CAN_HW
};

enum COMMHW
{
    NO_HW_SELECTED,
    RS232,
    USB_PORT,
    ATMEL_RS232_CAN,
    JANZ,
    VECTOR,
    IXXAT,
    PEAK,
    SYSTEC_USB_CAN,
    USB_LPC,
    USB_UART,
    USB_SPI
};

enum MEMORY_TYPE
{
    FLASH,
    EEPROM,
    CUSTOM,
    BOOT,
    XAF,
    HSB,
    SIGNATURE,
    SECURITY,
    CONFIGURATION,
    LPC_BOOT,
    MULTICAST,
	USER,

	// OCD
	MEM_OCDSFR = 12,
	MEM_CODE_R51,
	MEM_CODE_C51,
	MEM_FLASH,
	MEM_TESTSDI,

	// AVR32 memories.
	INT_RAM = 20,
	EXT_MEM_CS0,
	EXT_MEM_CS1,
	EXT_MEM_CS2,
	EXT_MEM_CS3,
	EXT_MEM_CS4,
	EXT_MEM_CS5,
	EXT_MEM_CS6,
	EXT_MEM_CS7
};



#define MEM_USER                0
#define MEM_CODE                0
#define MEM_CRAM                0
#define MEM_EEPROM           1
#define MEM_CUSTOM           2
#define MEM_BOOT               3
#define MEM_XAF                 4
#define MEM_HW                  5
#define MEM_SIGNATURES   6

#define MEM_FWUPDATE      7
// Add Memory for OCD :
#define MEM_IDATA               8
#define MEM_ERAM               9
#define MEM_XDATA              10
#define MEM_SFR                 11
#define MEM_OCDSFR              12
#define MEM_CODE_R51            13
#define MEM_CODE_C51            14
#define MEM_FLASH               14
#define MEM_TESTSDI             15



// Fuse Special Byte
enum  sbyte {
    BOOTLOADER_VERSION,
    SOFTWARE_BOOT_VECTOR,
    SOFTWARE_SECURITY_BYTE,
    BOOT_STATUS_BYTE,
    HARDWARE_BYTE,
    EXTRA_BYTE,
    DEVICE_BOOT_ID1,
    DEVICE_BOOT_ID2,
    DEVICE_MANUF_ID,
    DEVICE_ID1,
    DEVICE_ID2,
    DEVICE_ID3,
    BTC_1,
    BTC_2,
    BTC_3,
    NNB,
    CRIS,
    P1_CONF,
    P3_CONF,
    P4_CONF
};

// Fuse bits.

enum {
    OSC_FUSE,
    BLJB_FUSE,
    X2_FUSE,
    SELBOOT_FUSE
};
enum READ_OP_TYPE
{
    READ,
    VERIFY
};

enum HEX_FORMAT {
    HEX386, // Extended Linear Address Records
    HEX86   // Extended Segment Address Records
};


//// ________________________________ TYPE DEFS _____________________________________
//
//
//// Dynamic call of JNI functions.
//typedef jint    (JNICALL CreateJavaVM_t)    (JavaVM**, void**, void*);
typedef vector<string> ERROR_VEC;
//
//// ________________________________ STRUCTURES _____________________________________
//
//
//struct _InvocationFunctions
//{
//    CreateJavaVM_t    *CreateJavaVM;
//};
//
//
//
//// ________________________________ FUNCTIONS PROTOTYPES _____________________________________
//
//// Non exported functions.
//int launchProtocolXmlParser(void);
//string checkSum(string str);
//int selectCommunicationHardware(int hw);
//
//int readVerifyDevice(unsigned long addrLo, unsigned long addrHi, int opType);
//void freeNativeBuffer(void);
//int getCurrentSelectedMemory(void);
//void getCommand(string cmdName, string& cmdValue, int* expectedNumChars);
//void readAvr32BootloaderSize(JNIEnv* pEnv);


// Exported functions.
ATJNIISP_API int    STDCALL  ispInit(void);
ATJNIISP_API int    STDCALL  ispInitNoCheck(void);
ATJNIISP_API int   STDCALL ispSetLogEnabled(UCHAR ena);
ATJNIISP_API int    STDCALL ispSelectDevice(const char* deviceName);
ATJNIISP_API int    STDCALL ispSelectCommHardware(const int hw);
ATJNIISP_API int    STDCALL ispOpenRs232Port(char* portName, const unsigned long baudRate);
ATJNIISP_API int    STDCALL ispSyncTarget(void);
ATJNIISP_API int    STDCALL ispSetTimeout(DWORD tsec, int permanent);

ATJNIISP_API int    STDCALL ispParseHexFile(char* filePathName);
ATJNIISP_API int    STDCALL ispParseElfFile(char* filePathName);

ATJNIISP_API int    STDCALL ispWriteHexFile(ULONG addrLo, ULONG addrHi,
                                       char* hexFile, int format);

ATJNIISP_API int    STDCALL ispBlankCheckMemory(void);
ATJNIISP_API int    STDCALL ispBlankCheckAddrRange(ULONG addrLo, ULONG addrHi);
ATJNIISP_API int    STDCALL ispProgramDevice(void);
ATJNIISP_API int    STDCALL ispReadAddrRange(ULONG addrLo, ULONG addrHi, char* hexF, int format);

ATJNIISP_API int    STDCALL ispReadDevice(ULONG addrLo, ULONG addrHi);
ATJNIISP_API int    STDCALL ispVerifyDevice(ULONG addrLo, ULONG addrHi);
ATJNIISP_API int    STDCALL ispGetLastError(void);
ATJNIISP_API void    STDCALL ispExit(void);
ATJNIISP_API int    STDCALL ispGetTaskCompletionPercent(void);
ATJNIISP_API void    STDCALL ispResetTaskCompletionPercent(void);
ATJNIISP_API HANDLE STDCALL ispGetTaskPeriodicEventHandle(void);
ATJNIISP_API int    STDCALL ispUpdateCmdsIds(int cris);
ATJNIISP_API int    STDCALL ispOpenCanPort(int canBitRate, int cris, int rs232BaudRate);
ATJNIISP_API int    STDCALL ispOpenUsbPort(void);
ATJNIISP_API int    STDCALL ispSelectCanNode(BYTE nodeNumber);
ATJNIISP_API int    STDCALL ispSelectMemory(int memory);
ATJNIISP_API int    STDCALL ispEraseDevice(void);
ATJNIISP_API int    STDCALL ispGetDeviceIndex(char* name);
ATJNIISP_API void    STDCALL ispDataLog(const char* data);
ATJNIISP_API void    STDCALL ispSetDebugMode(int dbg);
ATJNIISP_API void    STDCALL ispOpenLogFile(char* logFile = "log.txt");
ATJNIISP_API int    STDCALL ispExecCmd(const char* cmd);
ATJNIISP_API char*    STDCALL ispGetLastResult(void);
ATJNIISP_API int    STDCALL ispGetMemorySize(void);
ATJNIISP_API int    STDCALL ispReadSignatureBytes(void);
ATJNIISP_API int    STDCALL ispReadSpecialByte(int sbyte);
ATJNIISP_API int    STDCALL ispFillBufferAddrRange(ULONG addrLo, ULONG addrHi, char* data);
ATJNIISP_API int    STDCALL ispProgramAddrRange(ULONG addrLo, ULONG addrHi,
                                                int forceAddRange = 0);
ATJNIISP_API int    STDCALL flipReadDevice(ULONG addrLo, ULONG addrHi);
ATJNIISP_API int    STDCALL flipVerifyDevice(ULONG addrLo, ULONG addrHi);
ATJNIISP_API int    STDCALL ispSetSecurityLevel(int lev);
ATJNIISP_API ERROR_VEC* STDCALL ispGetErrorVecPtr(void);
ATJNIISP_API int    STDCALL ispGetBufferAddrHi(void);
ATJNIISP_API int    STDCALL ispGetBufferAddrLo(void);
ATJNIISP_API int    STDCALL ispSerialize(int memory, ULONG addr, LONG64 sn,
                                     UCHAR snBase, string format);

ATJNIISP_API int    STDCALL ispEnableFuseBit(int fuse);
ATJNIISP_API int    STDCALL ispDisableFuseBit(int fuse);

ATJNIISP_API int    STDCALL ispWriteSpecialByte(int sbyte, BYTE value);
ATJNIISP_API const char* STDCALL ispGetLastErrorMssg(void);
ATJNIISP_API const char* STDCALL ispGetErrorMssgFromIdx(int errIdx);
ATJNIISP_API int    STDCALL ispSetRtsLow(void);
ATJNIISP_API int    STDCALL ispSetRtsHigh(void);
ATJNIISP_API int    STDCALL ispSetDtrHigh(void);
ATJNIISP_API int    STDCALL ispSetDtrLow(void);
ATJNIISP_API int    STDCALL ispSetAutoIsp(int rstHi, int psenLo);
ATJNIISP_API int    STDCALL ispEnterAppliMode(int rstPulse, ULONG addr);

ATJNIISP_API int    STDCALL ispInitOcd(const char* ocdType);
ATJNIISP_API void    STDCALL    ispExitOcdProgrammer(void);
ATJNIISP_API void    STDCALL ispStartOcdProgrammer(const char* ocdType);

// Library functions pointers.



/* ________________________________ FUNCTIONS POINTERS _____________________________________
*/

// These function pointers may be used by applications which load our AtJniIsp
// DLL dynamically.

typedef int        (STDCALL *PF_ispSetLogEnabled)(UCHAR ena);
typedef int        (STDCALL *PF_ispSetTimeout)(DWORD tsec, int permanent);
typedef int        (STDCALL *PF_ispGetLastError)(void);
typedef char*    (STDCALL *PF_ispGetLastResult)(void);
typedef int        (STDCALL *PF_ispInit)(void);
typedef int        (STDCALL *PF_ispInitNoCheck)(void);
typedef void    (STDCALL *PF_ispExit)(void);
typedef int        (STDCALL *PF_ispSelectDevice)(const char* deviceName);
typedef int        (STDCALL *PF_ispSelectCommHardware)(const int hw);
typedef int        (STDCALL *PF_ispOpenCanPort)(int canBitRate, int cris, int rs232BaudRate);

typedef int        (STDCALL *PF_ispOpenRs232Port)(char* portName, const unsigned long baudRate);
typedef int        (STDCALL *PF_ispUpdateRs232Baudrate)(DWORD baudrate);
typedef int        (STDCALL *PF_ispOpenUsbPort)(void);
typedef int        (STDCALL *PF_ispSelectCanNode)(BYTE nodeNumber);
typedef int        (STDCALL *PF_ispUpdateCmdsIds)(int cris);
typedef int        (STDCALL *PF_ispClosePort)(void);
typedef int        (STDCALL *PF_ispSyncTarget)(void);
typedef int        (STDCALL *PF_ispWriteSpecialByte)(int sbyte, BYTE value);
typedef int        (STDCALL *PF_ispReadSpecialByte)(int sbyte);
typedef int        (STDCALL *PF_ispExecSpecialCmd)(int cmd);
typedef int        (STDCALL *PF_ispReadSignatureBytes)(void);
typedef int        (STDCALL *PF_ispParseHexFile)(char* filePathName);
typedef int        (STDCALL *PF_ispParseElfFile)(char* filePathName);
typedef int        (STDCALL *PF_ispWriteHexFile)(ULONG addrLo, ULONG addrHi,
                                           char* hexFile, int format);

typedef int        (STDCALL *PF_ispEraseDevice)(void);
typedef int    (STDCALL *PF_ispEraseSbvBsb)(void);
typedef int        (STDCALL *PF_ispSetSecurityLevel)(int lev);
typedef int    (STDCALL *PF_ispGetExpectedSsbValue)(int level);
typedef int        (STDCALL *PF_ispProgramAddrRange)(ULONG addrLo, ULONG addrHi, int forceAddRange);
typedef int        (STDCALL *PF_ispProgramDevice)(void);
typedef int    (STDCALL *PF_ispReadAddrRange)(ULONG addrLo, ULONG addrHi, char* hexF, int format);
typedef int        (STDCALL *PF_ispReadDevice)(ULONG addrLo, ULONG addrHi);
typedef int    (STDCALL *PF_ispVerifyAddrRange)(ULONG addrLo, ULONG addrHi);
typedef int        (STDCALL *PF_ispVerifyDevice)(ULONG addrLo, ULONG addrHi);
typedef int        (STDCALL *PF_ispBlankCheckAddrRange)(ULONG addrLo, ULONG addrHi);
typedef int        (STDCALL *PF_ispBlankCheckMemory)(void);
typedef int    (STDCALL *PF_ispFillBufferAddrRange)(ULONG addrLo, ULONG addrHi, char* data);
typedef int        (STDCALL *PF_ispGetBufferAddrHi)(void);
typedef int        (STDCALL *PF_ispGetBufferAddrLo)(void);
typedef int        (STDCALL *PF_ispSelectMemory)(int memory);
typedef int    (STDCALL *PF_ispEnterIspMode)(BYTE nodeNumber, ULONG addr);
typedef int        (STDCALL *PF_ispEnterAppliMode)(int rstPulse, ULONG addr);
typedef ERROR_VEC* (STDCALL *PF_ispGetErrorVecPtr)(void);
typedef void    (STDCALL *PF_ispSetDemoMode)(void);
typedef void    (STDCALL *PF_ispCancelDemoMode)(void);


typedef void    (STDCALL *PF_ispSetDebugMode)(int dbg);
typedef void    (STDCALL *PF_ispCancelDebugMode)(void);
typedef void    (STDCALL *PF_ispOpenLogFile)(char* logFile);
typedef void    (STDCALL *PF_ispDataLog)(const char* data);

typedef int    (STDCALL *PF_ispSetAutoIsp)(int rstHi, int psenLo);
typedef int        (STDCALL *PF_ispGetMemorySize)(void);
typedef int        (STDCALL *PF_ispSerialize)(int memory, ULONG addr, LONG64 sn,
                                     UCHAR snBase, string format);
typedef int    (STDCALL *PF_ispEnableFuseBit)(int fuse);
typedef int    (STDCALL *PF_ispDisableFuseBit)(int fuse);
typedef int    (STDCALL *PF_ispComputeCrc)(void);
typedef int        (STDCALL *PF_ispGetDeviceIndex)(char* name);
typedef int        (STDCALL *PF_ispGetTaskCompletionPercent)(void);
typedef void    (STDCALL *PF_ispResetTaskCompletionPercent)(void);

#ifdef WIN32
typedef HANDLE    (STDCALL *PF_ispGetTaskPeriodicEventHandle)(void);
#else
typedef pthread_cond_t* (STDCALL *PF_ispGetTaskPeriodicEventHandle)(void);
#endif

typedef int (STDCALL *PF_ispSetDtrLow)(void);
typedef int (STDCALL *PF_ispSetDtrHigh)(void);
typedef int (STDCALL *PF_ispSetRtsLow)(void);
typedef int (STDCALL *PF_ispSetRtsHigh)(void);

typedef void (STDCALL *PF_ispCheckUsbDriver)(int check);

typedef int (STDCALL *PF_ispInitOcd)(const char* ocdType);
typedef void (STDCALL *PF_ispExitOcdProgrammer)(void);
typedef void (STDCALL *PF_ispStartOcdProgrammer)(const char* ocdType);

typedef void (STDCALL *PF_ispSelectCommLib)(const char* libName);
typedef int (STDCALL *PF_ispExecCmd)(const char* cmd);
typedef const char* (STDCALL *PF_ispGetLastErrorMssg)(void);
typedef const char* (STDCALL *PF_ispGetErrorMssgFromIdx)(int errIdx);

// Atmel reserved function. Do not use.
typedef void (STDCALL *PF_ispSelectProtocol)(int hw, int protocol);
typedef int (STDCALL *PF_flipReadDevice)(ULONG addrLo, ULONG addrHi);
typedef int (STDCALL *PF_flipVerifyDevice)(ULONG addrLo, ULONG addrHi);

bool createHexFile(ULONG addrLo, ULONG addrHi, char* hexFile, int format);


#endif // _ATJNIISP_H_

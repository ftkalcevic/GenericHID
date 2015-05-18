/*
 * dfu-programmer
 *
 * $Id: dfu.c 81 2009-01-22 09:45:15Z schmidtw $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */
// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include <stdio.h>
#include <stdarg.h>
#include "inttypes.h"
#include <stddef.h>
#include <libusb-1.0/libusb.h>
#include <errno.h>
#include "dfucommon.h"
#include "dfu.h"

Logger g_Logger( "DFUProgrammer", "DFUProgrammer" );
QString h_sLastError;

/* DFU commands */
#define DFU_DETACH      0
#define DFU_DNLOAD      1
#define DFU_UPLOAD      2
#define DFU_GETSTATUS   3
#define DFU_CLRSTATUS   4
#define DFU_GETSTATE    5
#define DFU_ABORT       6

#define USB_CLASS_APP_SPECIFIC  0xfe
#define DFU_SUBCLASS            0x01

/* Wait for 10 seconds before a timeout since erasing/flashing can take some time. */
#define DFU_TIMEOUT 10000

/* Time (in ms) for the device to wait for the usb reset after being told to detach
 * before the giving up going into dfu mode. */
#define DFU_DETACH_TIMEOUT 1000

#define DFU_DEBUG_THRESHOLD         100
#define DFU_TRACE_THRESHOLD         200
#define DFU_MESSAGE_DEBUG_THRESHOLD 300


static uint16_t transaction = 0;

static int32_t dfu_find_interface( libusb_device *device,
                                   const bool honor_interfaceclass );
static int32_t dfu_make_idle( dfu_device_t *device, const bool initial_abort );
static void dfu_msg_response_output( const char *function, const int32_t result );

#if HAVE_CONFIG_H
# include <config.h>
#endif
#undef malloc

#include <sys/types.h>

//void *malloc();

/* Allocate an N-byte block of memory from the heap.
 *    If N is zero, allocate a 1-byte block.  */
void* rpl_malloc( size_t n )
{
    if( 0 == n ) {
        n = 1;
    }
    
    return malloc( n );
}


/*
 *  DFU_DETACH Request (DFU Spec 1.0, Section 5.1)
 *
 *  device    - the dfu device to commmunicate with
 *  timeout   - the timeout in ms the USB device should wait for a pending
 *              USB reset before giving up and terminating the operation
 *
 *  returns 0 or < 0 on error
 */
int32_t dfu_detach( dfu_device_t *device, const int32_t timeout )
{
    int32_t result;
    
    DEBUG_MSG( QString("%s( %p, %d )\n").arg(__FUNCTION__).arg((int)device).arg(timeout) );
    
    if( (NULL == device) || (NULL == device->handle) || (timeout < 0) ) 
    {
        ERROR_MSG( "Invalid parameter\n" );
        return -1;
    }
    
    result = libusb_control_transfer( device->handle,
                                      /* bmRequestType */ LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                      /* bRequest      */ DFU_DETACH,
                                      /* wValue        */ timeout,
                                      /* wIndex        */ device->interface,
                                      /* Data          */ NULL,
                                      /* wLength       */ 0,
                                      DFU_TIMEOUT );
    
    dfu_msg_response_output( __FUNCTION__, result );
    
    return result;
}


/*
 *  DFU_DNLOAD Request (DFU Spec 1.0, Section 6.1.1)
 *
 *  device    - the dfu device to commmunicate with
 *  length    - the total number of bytes to transfer to the USB
 *              device - must be less than wTransferSize
 *  data      - the data to transfer
 *
 *  returns the number of bytes written or < 0 on error
 */
int32_t dfu_download( dfu_device_t *device, const size_t length, uint8_t* data )
{
    int32_t result;
    
    DEBUG_MSG( QString("%1( %2, %3, %4 )\n").arg(__FUNCTION__).arg((int)device).arg(length).arg((int)data) );
    
    /* Sanity checks */
    if( (NULL == device) || (NULL == device->handle) ) {
        ERROR_MSG( "Invalid parameter\n" );
        return -1;
    }
    
    if( (0 != length) && (NULL == data) ) {
        ERROR_MSG( "data was NULL, but length != 0\n" );
        return -2;
    }
    
    if( (0 == length) && (NULL != data) ) {
        ERROR_MSG( "data was not NULL, but length == 0\n" );
        return -3;
    }
    
    
    {
        size_t i;
        for( i = 0; i < length; i++ ) {
            DEBUG_MSG( QString("Message: m[%1] = 0x%2\n").arg(i).arg(data[i],2,16,QChar('0')) );
        }
    }
    
    result = libusb_control_transfer( device->handle,
                                      /* bmRequestType */ LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                      /* bRequest      */ DFU_DNLOAD,
                                      /* wValue        */ transaction++,
                                      /* wIndex        */ device->interface,
                                      /* Data          */ (unsigned char *) data,
                                      /* wLength       */ length,
                                      DFU_TIMEOUT );
    
    dfu_msg_response_output( __FUNCTION__, result );
    
    return result;
}


/*
 *  DFU_UPLOAD Request (DFU Spec 1.0, Section 6.2)
 *
 *  device    - the dfu device to commmunicate with
 *  length    - the maximum number of bytes to receive from the USB
 *              device - must be less than wTransferSize
 *  data      - the buffer to put the received data in
 *
 *  returns the number of bytes received or < 0 on error
 */
int32_t dfu_upload( dfu_device_t *device, const size_t length, uint8_t* data )
{
    int32_t result;
    
    DEBUG_MSG( QString("%1( %2, %3, %4 )\n").arg(__FUNCTION__).arg((int)device).arg(length).arg((int)data) );
    
    /* Sanity checks */
    if( (NULL == device) || (NULL == device->handle) ) {
        ERROR_MSG( "Invalid parameter\n" );
        return -1;
    }
    
    if( (0 == length) || (NULL == data) ) {
        ERROR_MSG( "data was NULL, or length is 0\n" );
        return -2;
    }
    
    result = libusb_control_transfer( device->handle,
                                      /* bmRequestType */ LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                      /* bRequest      */ DFU_UPLOAD,
                                      /* wValue        */ transaction++,
                                      /* wIndex        */ device->interface,
                                      /* Data          */ (unsigned char *) data,
                                      /* wLength       */ length,
                                      DFU_TIMEOUT );
    
    dfu_msg_response_output( __FUNCTION__, result );
    
    return result;
}


/*
 *  DFU_GETSTATUS Request (DFU Spec 1.0, Section 6.1.2)
 *
 *  device    - the dfu device to commmunicate with
 *  status    - the data structure to be populated with the results
 *
 *  return the 0 if successful or < 0 on an error
 */
int32_t dfu_get_status( dfu_device_t *device, dfu_status_t *status )
{
    unsigned char buffer[6];
    int32_t result;
    
    DEBUG_MSG( QString("%1( %2, %3 )\n").arg(__FUNCTION__).arg((int)device).arg((int)status) );
    
    if( (NULL == device) || (NULL == device->handle) ) {
        ERROR_MSG( "Invalid parameter\n" );
        return -1;
    }
    
    /* Initialize the status data structure */
    status->bStatus       = DFU_STATUS_ERROR_UNKNOWN;
    status->bwPollTimeout = 0;
    status->bState        = STATE_DFU_ERROR;
    status->iString       = 0;
    
    result = libusb_control_transfer( device->handle,
                                      /* bmRequestType */ LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                      /* bRequest      */ DFU_GETSTATUS,
                                      /* wValue        */ 0,
                                      /* wIndex        */ device->interface,
                                      /* Data          */ buffer,
                                      /* wLength       */ 6,
                                      DFU_TIMEOUT );
    
    dfu_msg_response_output( __FUNCTION__, result );
    
    if( 6 == result ) {
        status->bStatus = buffer[0];
        status->bwPollTimeout = ((0xff & buffer[3]) << 16) |
                ((0xff & buffer[2]) << 8)  |
                (0xff & buffer[1]);
        
        status->bState  = buffer[4];
        status->iString = buffer[5];
        
        DEBUG_MSG( "==============================\n" );
        DEBUG_MSG( QString("status->bStatus: %1 (0x%2)\n").arg(dfu_status_to_string(status->bStatus)).arg(status->bStatus,2,16,QChar('0')) );
        DEBUG_MSG( QString("status->bwPollTimeout: 0x%1\n").arg(status->bwPollTimeout,4,16,QChar('0')) );
        DEBUG_MSG( QString("status->bState: %1 (0x%2)\n").arg(dfu_state_to_string(status->bState)).arg(status->bState,8,16,QChar('0')) );
        DEBUG_MSG( QString("status->iString: 0x%1\n").arg(status->iString,2,16,QChar('0')) );
        DEBUG_MSG( "------------------------------\n" );
    } else {
        if( 0 < result ) {
            /* There was an error, we didn't get the entire message. */
            ERROR_MSG( QString("result: %1\n").arg(result) );
            return -2;
        }
    }
    
    return 0;
}


/*
 *  DFU_CLRSTATUS Request (DFU Spec 1.0, Section 6.1.3)
 *
 *  device    - the dfu device to commmunicate with
 *
 *  return 0 or < 0 on an error
 */
int32_t dfu_clear_status( dfu_device_t *device )
{
    int32_t result;
    
    DEBUG_MSG( QString("%1( %2 )\n").arg(__FUNCTION__).arg((int)device) );
    
    if( (NULL == device) || (NULL == device->handle) ) {
        ERROR_MSG( "Invalid parameter\n" );
        return -1;
    }
    
    result = libusb_control_transfer( device->handle,
                                      /* bmRequestType */ LIBUSB_ENDPOINT_OUT| LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                      /* bRequest      */ DFU_CLRSTATUS,
                                      /* wValue        */ 0,
                                      /* wIndex        */ device->interface,
                                      /* Data          */ NULL,
                                      /* wLength       */ 0,
                                      DFU_TIMEOUT );
    
    dfu_msg_response_output( __FUNCTION__, result );
    
    return result;
}


/*
 *  DFU_GETSTATE Request (DFU Spec 1.0, Section 6.1.5)
 *
 *  device    - the dfu device to commmunicate with
 *
 *  returns the state or < 0 on error
 */
int32_t dfu_get_state( dfu_device_t *device )
{
    int32_t result;
    unsigned char buffer[1];
    
    DEBUG_MSG( QString("%1( %2 )\n").arg(__FUNCTION__).arg((int)device) );
    
    if( (NULL == device) || (NULL == device->handle) ) {
        ERROR_MSG( "Invalid parameter\n" );
        return -1;
    }
    
    result = libusb_control_transfer( device->handle,
                                      /* bmRequestType */ LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                      /* bRequest      */ DFU_GETSTATE,
                                      /* wValue        */ 0,
                                      /* wIndex        */ device->interface,
                                      /* Data          */ buffer,
                                      /* wLength       */ 1,
                                      DFU_TIMEOUT );
    
    dfu_msg_response_output( __FUNCTION__, result );
    
    /* Return the error if there is one. */
    if( result < 1 ) {
        return result;
    }
    
    /* Return the state. */
    return buffer[0];
}


/*
 *  DFU_ABORT Request (DFU Spec 1.0, Section 6.1.4)
 *
 *  device    - the dfu device to commmunicate with
 *
 *  returns 0 or < 0 on an error
 */
int32_t dfu_abort( dfu_device_t *device )
{
    int32_t result;
    
    DEBUG_MSG( QString("%1( %2 )\n").arg(__FUNCTION__).arg((int)device) );
    
    if( (NULL == device) || (NULL == device->handle) ) {
        ERROR_MSG( "Invalid parameter\n" );
        return -1;
    }
    
    result = libusb_control_transfer( device->handle,
                                      /* bmRequestType */ LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                      /* bRequest      */ DFU_ABORT,
                                      /* wValue        */ 0,
                                      /* wIndex        */ device->interface,
                                      /* Data          */ NULL,
                                      /* wLength       */ 0,
                                      DFU_TIMEOUT );
    
    dfu_msg_response_output( __FUNCTION__, result );
    
    return result;
}


/*
 *  dfu_device_init is designed to find one of the usb devices which match
 *  the vendor and product parameters passed in.
 *
 *  vendor  - the vender number of the device to look for
 *  product - the product number of the device to look for
 *  [out] device - the dfu device to commmunicate with
 *
 *  return a pointer to the usb_device if found, or NULL otherwise
 */
libusb_device *dfu_device_init( const uint32_t vendor,
                                const uint32_t product,
                                dfu_device_t *dfu_device,
                                const bool initial_abort,
                                const bool honor_interfaceclass )
{
    int32_t retries = 4;
    
    DEBUG_MSG( QString("%1( %2, %3, %4, %5, %6 )\n").arg(__FUNCTION__).arg(vendor).arg(product).arg((int)dfu_device).arg(initial_abort ? "true" : "false").arg(honor_interfaceclass ? "true" : "false") );
    
retry:
    
    if( 0 < retries ) {
        libusb_device **devices = NULL;
        int device_count = libusb_get_device_list( NULL, &devices);
        
        /* Walk the tree and find our device. */
        for ( int i = 0; i < device_count; i++ )
        {
            libusb_device *device = devices[i];
            
            libusb_device_descriptor desc;
            int r = libusb_get_device_descriptor(device, &desc);
            if ( r == 0 && 
                 (vendor  == desc.idVendor) &&
                 (product == desc.idProduct) )
            {
                int32_t tmp;
                /* We found a device that looks like it matches...
                 * let's try to find the DFU interface, open the device
                 * and claim it. */
                tmp = dfu_find_interface( device, honor_interfaceclass );
                if( 0 <= tmp ) {
                    /* The interface is valid. */
                    dfu_device->interface = tmp;
                    dfu_device->handle = NULL;
                    tmp = libusb_open( device, &(dfu_device->handle) );
                    if( NULL != dfu_device->handle ) {
                        if( 0 == libusb_set_configuration(dfu_device->handle, 1) ) {
                            if( 0 == libusb_claim_interface(dfu_device->handle, dfu_device->interface) ) {
                                switch( dfu_make_idle(dfu_device, initial_abort) )
                                {
                                    case 0:
                                        return device;
                                    case 1:
                                        retries--;
                                        goto retry;
                                }
                                
                                ERROR_MSG( "Failed to put the device in dfuIDLE mode.\n" );
                                libusb_release_interface( dfu_device->handle, dfu_device->interface );
                                libusb_close( dfu_device->handle );
                                retries = 4;
                            } else {
                                ERROR_MSG( "Failed to claim the DFU interface.\n" );
                                libusb_close( dfu_device->handle );
                            }
                        } else {
                            ERROR_MSG( "Failed to set configuration.\n");
                            
                            libusb_close( dfu_device->handle );
                        }
                    } else {
                        ERROR_MSG( "Failed to open device.\n" );
                    }
                } else {
                    ERROR_MSG( "Failed to find the DFU interface.\n" );
                }
            }
        }
    }
    
    dfu_device->handle = NULL;
    dfu_device->interface = 0;
    
    return NULL;
}


/*
 *  Used to convert the DFU state to a string.
 *
 *  state - the state to convert
 *
 *  returns the state name or "unknown state"
 */
const char* dfu_state_to_string( const int32_t state )
{
    const char *message = "unknown state";
    
    switch( state ) {
        case STATE_APP_IDLE:
            message = "appIDLE";
            break;
        case STATE_APP_DETACH:
            message = "appDETACH";
            break;
        case STATE_DFU_IDLE:
            message = "dfuIDLE";
            break;
        case STATE_DFU_DOWNLOAD_SYNC:
            message = "dfuDNLOAD-SYNC";
            break;
        case STATE_DFU_DOWNLOAD_BUSY:
            message = "dfuDNBUSY";
            break;
        case STATE_DFU_DOWNLOAD_IDLE:
            message = "dfuDNLOAD-IDLE";
            break;
        case STATE_DFU_MANIFEST_SYNC:
            message = "dfuMANIFEST-SYNC";
            break;
        case STATE_DFU_MANIFEST:
            message = "dfuMANIFEST";
            break;
        case STATE_DFU_MANIFEST_WAIT_RESET:
            message = "dfuMANIFEST-WAIT-RESET";
            break;
        case STATE_DFU_UPLOAD_IDLE:
            message = "dfuUPLOAD-IDLE";
            break;
        case STATE_DFU_ERROR:
            message = "dfuERROR";
            break;
    }
    
    return message;
}


/*
 *  Used to convert the DFU status to a string.
 *
 *  status - the status to convert
 *
 *  returns the status name or "unknown status"
 */
const char* dfu_status_to_string( const int32_t status )
{
    const char *message = "unknown status";
    
    switch( status ) {
    case DFU_STATUS_OK:
        message = "OK";
        break;
    case DFU_STATUS_ERROR_TARGET:
        message = "errTARGET";
        break;
    case DFU_STATUS_ERROR_FILE:
        message = "errFILE";
        break;
    case DFU_STATUS_ERROR_WRITE:
        message = "errWRITE";
        break;
    case DFU_STATUS_ERROR_ERASE:
        message = "errERASE";
        break;
    case DFU_STATUS_ERROR_CHECK_ERASED:
        message = "errCHECK_ERASED";
        break;
    case DFU_STATUS_ERROR_PROG:
        message = "errPROG";
        break;
    case DFU_STATUS_ERROR_VERIFY:
        message = "errVERIFY";
        break;
    case DFU_STATUS_ERROR_ADDRESS:
        message = "errADDRESS";
        break;
    case DFU_STATUS_ERROR_NOTDONE:
        message = "errNOTDONE";
        break;
    case DFU_STATUS_ERROR_FIRMWARE:
        message = "errFIRMWARE";
        break;
    case DFU_STATUS_ERROR_VENDOR:
        message = "errVENDOR";
        break;
    case DFU_STATUS_ERROR_USBR:
        message = "errUSBR";
        break;
    case DFU_STATUS_ERROR_POR:
        message = "errPOR";
        break;
    case DFU_STATUS_ERROR_UNKNOWN:
        message = "errUNKNOWN";
        break;
    case DFU_STATUS_ERROR_STALLEDPKT:
        message = "errSTALLEDPKT";
        break;
        
    }
    
    return message;
}


/*
 *  Used to find the dfu interface for a device if there is one.
 *
 *  device - the device to search
 *  honor_interfaceclass - if the actual interface class information
 *                         should be checked, or ignored (bug in device DFU code)
 *
 *  returns the interface number if found, < 0 otherwise
 */
static int32_t dfu_find_interface( libusb_device *device,
                                   const bool honor_interfaceclass )
{
    int32_t c, i;
    const libusb_interface_descriptor *interface;
    libusb_device_descriptor desc;
    
    libusb_get_device_descriptor(device, &desc);
    
    /* Loop through all of the configurations */
    for( c = 0; c < desc.bNumConfigurations; c++ ) {
        libusb_config_descriptor *config = NULL;
        if ( libusb_get_config_descriptor( device, c, &config ) == 0 )
        {
            /* Loop through all of the interfaces */
            for( i = 0; i < config->interface->num_altsetting; i++) {
                interface = &(config->interface->altsetting[i]);
                
                if( true == honor_interfaceclass ) {
                    /* Check if the interface is a DFU interface */
                    if(    (USB_CLASS_APP_SPECIFIC == interface->bInterfaceClass)
                           && (DFU_SUBCLASS == interface->bInterfaceSubClass) )
                    {
                        DEBUG_MSG( QString("Found DFU Inteface: %1\n").arg(interface->bInterfaceNumber) );
                        return interface->bInterfaceNumber;
                    }
                } else {
                    /* If there is a bug in the DFU firmware, return the first
                     * found interface. */
                    DEBUG_MSG( QString("Found DFU Inteface: %1\n").arg(interface->bInterfaceNumber) );
                    return interface->bInterfaceNumber;
                }
            }
        }
    }
    
    return -1;
}


/*
 *  Gets the device into the dfuIDLE state if possible.
 *
 *  device    - the dfu device to commmunicate with
 *
 *  returns 0 on success, 1 if device was reset, error otherwise
 */
static int32_t dfu_make_idle( dfu_device_t *device,
                              const bool initial_abort )
{
    dfu_status_t status;
    int32_t retries = 4;
    
    if( true == initial_abort ) {
        dfu_abort( device );
    }
    
    while( 0 < retries ) {
        if( 0 != dfu_get_status(device, &status) ) {
            dfu_clear_status( device );
            continue;
        }
        
        DEBUG_MSG( QString("State: %1 (%2)\n").arg(dfu_state_to_string(status.bState)).arg(status.bState) );
        
        switch( status.bState ) {
        case STATE_DFU_IDLE:
            if( DFU_STATUS_OK == status.bStatus ) {
                return 0;
            }
            
            /* We need the device to have the DFU_STATUS_OK status. */
            dfu_clear_status( device );
            break;
            
        case STATE_DFU_DOWNLOAD_SYNC:   /* abort -> idle */
        case STATE_DFU_DOWNLOAD_IDLE:   /* abort -> idle */
        case STATE_DFU_MANIFEST_SYNC:   /* abort -> idle */
        case STATE_DFU_UPLOAD_IDLE:     /* abort -> idle */
        case STATE_DFU_DOWNLOAD_BUSY:   /* abort -> error */
        case STATE_DFU_MANIFEST:        /* abort -> error */
            dfu_abort( device );
            break;
            
        case STATE_DFU_ERROR:
            dfu_clear_status( device );
            break;
            
        case STATE_APP_IDLE:
            dfu_detach( device, DFU_DETACH_TIMEOUT );
            break;
            
        case STATE_APP_DETACH:
        case STATE_DFU_MANIFEST_WAIT_RESET:
            DEBUG_MSG( "Resetting the device\n" );
            libusb_reset_device( device->handle );
            return 1;
        }
        
        retries--;
    }
    
    ERROR_MSG( "Not able to transition the device into the dfuIDLE state.\n" );
    return -2;
}


/*
 *  Used to output the response from our USB request in a human reable
 *  form.
 *
 *  function - the calling function to output on behalf of
 *  result   - the result to interpret
 */
static void dfu_msg_response_output( const char * function, const int32_t result )
{
    const char *msg = NULL;
    
    if( 0 <= result ) {
        msg = "No error.";
    } else {
        switch( result ) {
            case -ENOENT:
                msg = "-ENOENT: URB was canceled by unlink_urb";
                break;
#ifdef EINPROGRESS
            case -EINPROGRESS:
                msg = "-EINPROGRESS: URB still pending, no results yet "
                        "(actually no error until now)";
                break;
#endif
#ifdef EPROTO
            case -EPROTO:
                msg = "-EPROTO: a) Bitstuff error or b) Unknown USB error";
                break;
#endif
            case -EILSEQ:
                msg = "-EILSEQ: CRC mismatch";
                break;
            case -EPIPE:
                msg = "-EPIPE: a) Babble detect or b) Endpoint stalled";
                break;
#ifdef ETIMEDOUT
            case -ETIMEDOUT:
                msg = "-ETIMEDOUT: Transfer timed out, NAK";
                break;
#endif
            case -ENODEV:
                msg = "-ENODEV: Device was removed";
                break;
#ifdef EREMOTEIO
            case -EREMOTEIO:
                msg = "-EREMOTEIO: Short packet detected";
                break;
#endif
            case -EXDEV:
                msg = "-EXDEV: ISO transfer only partially completed look at "
                        "individual frame status for details";
                break;
            case -EINVAL:
                msg = "-EINVAL: ISO madness, if this happens: Log off and go home";
                break;
            default:
                msg = "Unknown error";
                break;
        }
        
        ERROR_MSG( QString("%1: %2 0x%3 (%4)\n").arg(function).arg(msg).arg(result,8,16,QChar('0')).arg(result) );
    }
}

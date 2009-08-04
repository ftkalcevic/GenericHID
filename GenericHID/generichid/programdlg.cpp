#include "stdafx.h"
#include "programdlg.h"
#include "usb.h"

const int DEVICE_POLL_PERIOD = 500;	// ms

const unsigned short GenericHID_VID = 0x1c40;
const unsigned short GenericHID_PID = 0x04d9;

const unsigned short At90USB1287_VID = 0x03eb;
const unsigned short At90USB1287_PID = 0x2FFB;


ProgramDlg::ProgramDlg(QWidget *parent)
: QDialog(parent)
, m_Logger( QCoreApplication::applicationName(), "ProgramDlg" )
, m_bMultipleWarning( false )
{
    ui.setupUi(this);

    SetMode( false, false, false );

    // timer
    m_timer.setSingleShot( true );
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateDeviceStatus()));
    m_timer.start( 0 );	// first time expires immediately (that is, after the window pops up)
}

ProgramDlg::~ProgramDlg()
{
}

void ProgramDlg::SetMode( bool bDevice, bool bBootloader, bool bHID)
{
    ui.lightUSBDeviceLocated->setPixmap( bDevice ? QPixmap(":/GenericHID/greenlight") : QPixmap(":/GenericHID/redlight") );
    ui.lightBootloaderMode->setPixmap( bBootloader ? QPixmap(":/GenericHID/greenlight") : QPixmap(":/GenericHID/redlight") );
    ui.lightHIDDeviceMode->setPixmap( bHID ? QPixmap(":/GenericHID/greenlight") : QPixmap(":/GenericHID/redlight") );

    ui.btnStartBootloader->setEnabled( bHID );
    ui.btnProgram->setEnabled( bBootloader );
    ui.btnRestartDevice->setEnabled( bBootloader );
}

bool ProgramDlg::FindDevices( int &nGenericHIDs, int &nAt90DFUs )
{
    nGenericHIDs = 0;
    nAt90DFUs = 0;

    usb_find_busses();
    usb_find_devices();

    /* Walk the tree and find our devices. */
    for( struct usb_bus *usb_bus = usb_get_busses(); NULL != usb_bus; usb_bus = usb_bus->next ) 
    {
        for( struct usb_device *device = usb_bus->devices; NULL != device; device = device->next) 
	{
	    LOG_MSG( m_Logger, LogTypes::Debug, QString("VID=%1 PID=%2").arg(device->descriptor.idVendor,4,16,QChar('0')).arg(device->descriptor.idProduct,4,16,QChar('0')) );
            if( device->descriptor.idVendor == GenericHID_VID && device->descriptor.idProduct == GenericHID_PID )
		nGenericHIDs++;
            else if( device->descriptor.idVendor == At90USB1287_VID && device->descriptor.idProduct == At90USB1287_PID )
		nAt90DFUs++;
        }
    }
    return true;
}

USBDevice ProgramDlg::GetGenericHIDDevice()
{
    USBDevice retDevice;
    memset( &retDevice, 0, sizeof(USBDevice) );

    usb_find_busses();
    usb_find_devices();

    /* Walk the tree and find our device. */
    for( struct usb_bus *usb_bus = usb_get_busses(); NULL != usb_bus; usb_bus = usb_bus->next ) 
    {
        for( struct usb_device *device = usb_bus->devices; NULL != device; device = device->next) 
	{
            if( device->descriptor.idVendor == GenericHID_VID && device->descriptor.idProduct == GenericHID_PID )
            {
		/* Loop through all of the configurations */
		for( int c = 0; c < device->descriptor.bNumConfigurations; c++ ) 
		{
		    struct usb_config_descriptor *config = &(device->config[c]);

		    /* Loop through all of the interfaces */
		    for( int i = 0; i < config->interface->num_altsetting; i++) 
		    {
			struct usb_interface_descriptor *interface = &(config->interface->altsetting[i]);

			/* Check if the interface is a HID interface */
			if ( interface->bInterfaceClass == USB_CLASS_HID )
			{
			    usb_dev_handle *hDevice = usb_open( device );
			    if ( hDevice != NULL )
			    {
#ifdef _WIN32
				if( usb_set_configuration(hDevice, config->bConfigurationValue) == 0 ) 
				{
				    int nRet = usb_claim_interface(hDevice, interface->bInterfaceNumber);
#else
				int nRet = usb_claim_interface(hDevice, interface->bInterfaceNumber);
				if ( nRet != 0 )
				{
				    usb_detach_kernel_driver_np(hDevice, interface->bInterfaceNumber);
				    nRet = usb_claim_interface(hDevice, interface->bInterfaceNumber);
				}
				if ( nRet == 0 )
				{
				    nRet = usb_set_configuration(hDevice, config->bConfigurationValue);
				    nRet = 0;
#endif

				    if ( nRet == 0 )
				    {
					// Locate the output endpoint

					unsigned char nEndPoint = USB_ENDPOINT_OUT | USB_ENDPOINT_TYPE_INTERRUPT;

					for ( int ep = 0; ep < interface->bNumEndpoints; ep++ )
					{
					    if ( (interface->endpoint[ep].bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_OUT &&
						(interface->endpoint[ep].bmAttributes & USB_ENDPOINT_TYPE_MASK) == USB_ENDPOINT_TYPE_INTERRUPT )
					    {
						nEndPoint = interface->endpoint[ep].bEndpointAddress;
						break;
					    }
					}
					retDevice.hDevice = hDevice;
					retDevice.pDevice = device;
					retDevice.nEndpoint = nEndPoint;
					return retDevice;
				    }
				    else
				    {
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
    return retDevice;
}


void ProgramDlg::updateDeviceStatus()
{
    // Check for USB devices on the BUS.
    int nGenericHIDs = 0;
    int nAt90DFUs = 0;
    if ( FindDevices( nGenericHIDs, nAt90DFUs ) )
    {
	// check for too many devices
	if ( nGenericHIDs + nAt90DFUs > 1 )
	{
	    //
	    SetMode( false, false, false );
	    if ( !m_bMultipleWarning )
	    {
		m_bMultipleWarning = true;
		QMessageBox::warning( this, "Multiple Devices Found", "More than one GenericHID device (GenericHID or AT90USB128 in DFU mode) found in the system.  Only one can be present during programming." );
	    }
	}
	else
	{
	    m_bMultipleWarning = false;
	    if ( nGenericHIDs > 0 )
	    {
		// the device is present, but still in HID mode.
		SetMode( true, false, true );
	    }
	    else if ( nAt90DFUs > 0 )
	    {
		// the device is present, in DFU mode.
		SetMode( true, true, false );
	    }
	    else
	    {
		SetMode( false, false, false );
	    }
	}
    }

    m_timer.start( DEVICE_POLL_PERIOD );
}


#define MAGIC_BOOTLOADER_CODE	0xDF0DF0DF
#define BOOTLOADER_REPORT_ID	3
#define B0(X) ((X) & 0xFF)
#define B1(X) (((X)>>8) & 0xFF)
#define B2(X) (((X)>>16) & 0xFF)
#define B3(X) (((X)>>24) & 0xFF)

void ProgramDlg::onStartBootloader()
{
    // need to connec to the HID device and send the "boot loader mode" report.
    USBDevice Device = GetGenericHIDDevice();

    if ( Device.hDevice != NULL )
    {
	unsigned char BootloaderReport[] = { BOOTLOADER_REPORT_ID, 
					     B0(MAGIC_BOOTLOADER_CODE), 
					     B1(MAGIC_BOOTLOADER_CODE), 
					     B2(MAGIC_BOOTLOADER_CODE), 
					     B3(MAGIC_BOOTLOADER_CODE) };
	int result = usb_interrupt_write( Device.hDevice, 
					  Device.nEndpoint,
					  (char *)BootloaderReport,
					  countof(BootloaderReport),
					  1000 );
	if ( result != sizeof(BootloaderReport) )
	{
	}

	usb_close( Device.hDevice );
    }
}

void ProgramDlg::onProgram()
{

}

void ProgramDlg::onRestartDevice()
{
    if ( Programmer::Init() )
	Programmer::RunFirmware();
}

void ProgramDlg::onClose()
{
}

void ProgramDlg::onRefresh()
{
}

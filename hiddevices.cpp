// hidcomp/hidconfig, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

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

#include "common.h"
#include "hiddevices.h"
#include "hiddevice.h"
#if defined(_WIN32) || defined(LIBUSB01)
#include "usb.h"
#else
#include "libusb.h"
#endif
#include "log.h"

bool HIDDevices::m_bOpen = false;

HIDDevices::HIDDevices()
: m_Logger(QCoreApplication::applicationName(), "HIDDevices" )
{
}

HIDDevices::~HIDDevices(void)
{
}

bool HIDDevices::Open( int nDebugLevel )
{
    if ( !m_bOpen )
    {
	m_bOpen = true;
#if defined(_WIN32) || defined(LIBUSB01)
	usb_init();
	usb_set_debug(nDebugLevel);
#else
	libusb_init(NULL);
	libusb_set_debug(NULL,nDebugLevel);
#endif
    }
    return true;
}


bool HIDDevices::FindHIDDevices()
{
    if ( !m_bOpen )
	if ( !Open() )
	    return false;

    for ( unsigned int i = 0; i < m_Devices.size(); i++ )
	delete m_Devices[i];
    m_Devices.clear();

#if defined(_WIN32) || defined(LIBUSB01)
    usb_find_busses();
    usb_find_devices();
    struct usb_bus *busses = usb_get_busses();

    for (struct usb_bus *bus = busses; bus; bus = bus->next)
    {
	for (struct usb_device *dev = bus->devices; dev; dev = dev->next)
	{
	    /* Check if this device is a HID device */
	    if ( dev->config != NULL &&
		dev->config->interface != NULL &&
		dev->config->interface->num_altsetting > 0 &&
		dev->config->interface->altsetting != NULL &&
		dev->config->interface->altsetting->bInterfaceClass == USB_CLASS_HID )
	    {
		HIDDevice *pDevice = new HIDDevice( dev, 0, 0 );
		m_Devices.push_back( pDevice );
		LOG_MSG( m_Logger, LogTypes::Info, QString("Found USB HID device - PID:%1 VID:%2 Manufacturer:'%3' Product:'%4' Serial No.:'%5' System Id:'%6'\n" ).arg(pDevice->PID()).arg(pDevice->VID()).arg(pDevice->Manufacturer()).arg(pDevice->Product()).arg(pDevice->SerialNumber()).arg(pDevice->SystemId()) );
	    }
	}
    }
#else
    // discover devices
    libusb_device **list;
    size_t cnt = libusb_get_device_list(NULL, &list);

    for (unsigned int i = 0; i < cnt; i++) 
    {
	libusb_device *dev = list[i];
	libusb_device_descriptor device_desc;
	if ( libusb_get_device_descriptor( dev,  &device_desc ) == 0 )
	{
	    bool bMatched = false;
	    for ( int c = 0; c < device_desc.bNumConfigurations && !bMatched; c++ )
	    {
		libusb_config_descriptor *config_desc = NULL;

		if ( libusb_get_config_descriptor( dev, c, &config_desc ) == 0 )
		{
		    for ( int i = 0; i < config_desc->bNumInterfaces && !bMatched; i++ )
			for ( int a = 0; a < config_desc->interface[i].num_altsetting && !bMatched; a++ )
			    if (config_desc->interface[i].altsetting[a].bInterfaceClass == LIBUSB_CLASS_HID ) 
			    {
				HIDDevice *pDevice = new HIDDevice( dev, config_desc->interface[i].altsetting[a].bInterfaceNumber, config_desc->bConfigurationValue );
				m_Devices.push_back( pDevice );
				bMatched = true;
			    }

		    libusb_free_config_descriptor( config_desc );
		}
	    }
	}
    }

    libusb_free_device_list(list, 1);


#endif
    return true;
}


static bool CompareString( const QString &s1, const QString &s2 )
{
    return s1.compare(s2) == 0;
}


std::vector<HIDDevice *> HIDDevices::SearchHIDDevices( bool bPID, unsigned short nPID, bool bVID, unsigned short nVID, bool bManufacturer, const QString &sManfacturer, bool bProduct, const QString &sProduct, bool bSN, const QString &sSerialNumber, bool bSystemId, const QString &sSystemId, bool bInterfaceNumber, const byte nInterfaceNumber )
{
    std::vector<HIDDevice *> ret;

    if ( m_Devices.empty() )
	FindHIDDevices();

    std::vector<HIDDevice *>::iterator it;
    for ( it = m_Devices.begin(); it != m_Devices.end(); it++ )
    {
	HIDDevice *dev = *it;

	if ( ((bPID && dev->PID()==nPID) || !bPID) &&
	    ((bVID && dev->VID()==nVID) || !bVID) &&
	    ((bManufacturer && CompareString(dev->Manufacturer(), sManfacturer)) || !bManufacturer) &&
	    ((bProduct && CompareString(dev->Product(), sProduct)) || !bProduct) &&
	    ((bSN && CompareString(dev->SerialNumber(), sSerialNumber)) || !bSN) &&
            ((bSystemId && CompareString(dev->SystemId(), sSystemId)) || !bSystemId) &&
            ((bInterfaceNumber && dev->InterfaceNumber() == nInterfaceNumber)) )
	{
	    ret.push_back( dev );
	}
    }

    return ret;
}


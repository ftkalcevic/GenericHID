
/* Much of ths code courtesy of.... */

/* Teensy Loader, Command Line Interface
 * Program and Reboot Teensy Board with HalfKay Bootloader
 * http://www.pjrc.com/teensy/loader_cli.html
 * Copyright 2008-2010, PJRC.COM, LLC
 *
 * You may redistribute this program and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 */

/* Want to incorporate this code into a proprietary application??
 * Just email paul@pjrc.com to ask.  Usually it's not a problem,
 * but you do need to ask to use this code in any way other than
 * those permitted by the GNU General Public License, version 3  */

/* For non-root permissions on ubuntu or similar udev-based linux
 * http://www.pjrc.com/teensy/49-teensy.rules
 */



#include "dfucommon.h"
#include "inttypes.h"
#include "teensyprogrammer.h"
#include <usb.h>

TeensyProgrammer::TeensyProgrammer(void)
{
    // We only support Teensy++ 2.0, the at90usb1286 version...
    int bootloader_size = 512;
    m_flash_address_top = 0x20000;
    m_flash_address_bottom = 0;
    m_flash_page_size = 128;
    m_bootloader_bottom = m_flash_address_top - bootloader_size;
    m_bootloader_top = m_flash_address_top;
    m_flash_address_top -= (bootloader_size + 1);

}

TeensyProgrammer::~TeensyProgrammer(void)
{
}

bool TeensyProgrammer::GetDevice()
{
    if ( teensy_open() )
	return true;
    return false;
}

bool TeensyProgrammer::ReleaseDevice()
{
    teensy_close();
    return true;
}

bool TeensyProgrammer::EraseDevice()
{
    return true;    // Writing block zero, has the side effect of erasing the device, so nothing to do here.
}

bool TeensyProgrammer::StartProgramming(IntelHexBuffer &memory)
{
    // program the data
    unsigned char buf[260];
    bool first_block=true;
    int block_size = m_flash_page_size*2;
    int code_size = m_flash_address_top;
    for ( unsigned int addr = 0; addr < m_flash_address_top; addr += block_size ) 
    {
	if ( addr > 0 && !memory.isInUse(addr, addr + block_size - 1) ) 
	{
	    // don't waste time on blocks that are unused,
	    // but always do the first one to erase the chip
	    continue;
	}
	
	if (code_size < 0x10000) 
	{
	    buf[0] = (unsigned char)(addr & 255);
	    buf[1] = (unsigned char)((addr >> 8) & 255);
	} 
	else 
	{
	    buf[0] = (unsigned char)((addr >> 8) & 255);
	    buf[1] = (unsigned char)((addr >> 16) & 255);
	}
	for ( int i = 0; i < block_size; i++ )
	    buf[i+2] = (unsigned char)memory.data().data()[addr+i];
	
	int r = teensy_write(buf, block_size + 2, first_block ? 3.0 : 0.25);

	if (!r) 
	{
	    //die("error writing to Teensy\n");
	    return false;
	}
	first_block = false;
    }
    return true;
}

bool TeensyProgrammer::StartVerify(IntelHexBuffer &)
{
    return true;    // no verify.
}

bool TeensyProgrammer::EnterApplicationMode(ResetMode::ResetMode mode, unsigned int)
{
    unsigned char buf[260];
    int block_size = m_flash_page_size*2;

 //   if ( mode == ResetMode::Hard )
 //   {
	//hard_reboot();
 //   }
 //   else
    {
	buf[0] = 0xFF;
	buf[1] = 0xFF;
	memset(buf + 2, 0, sizeof(buf) - 2);
	teensy_write(buf, block_size + 2, 0.25);
    }
    return true;
}


usb_dev_handle * TeensyProgrammer::open_usb_device(int vid, int pid)
{
    struct usb_bus *bus;
    struct usb_device *dev;
    usb_dev_handle *h;
    int r;

    usb_init();
    usb_find_busses();
    usb_find_devices();
    //printf_verbose("\nSearching for USB device:\n");
    for (bus = usb_get_busses(); bus; bus = bus->next) {
	for (dev = bus->devices; dev; dev = dev->next) {
	    //printf_verbose("bus \"%s\", device \"%s\" vid=%04X, pid=%04X\n",
	    //	bus->dirname, dev->filename,
	    //	dev->descriptor.idVendor,
	    //	dev->descriptor.idProduct
	    //);
	    if (dev->descriptor.idVendor != vid) continue;
	    if (dev->descriptor.idProduct != pid) continue;
	    h = usb_open(dev);
	    if (!h) {
//		printf_verbose("Found device but unable to open");
		continue;
	    }
#ifdef LIBUSB_HAS_GET_DRIVER_NP
	    r = usb_get_driver_np(h, 0, buf, sizeof(buf));
	    if (r >= 0) {
		r = usb_detach_kernel_driver_np(h, 0);
		if (r < 0) {
		    usb_close(h);
		    printf_verbose("Device is in use by \"%s\" driver", buf);
		    continue;
		}
	    }
#endif
            usb_set_configuration(h, 1);
	    // Mac OS-X - removing this call to usb_claim_interface() might allow
	    // this to work, even though it is a clear misuse of the libusb API.
	    // normally Apple's IOKit should be used on Mac OS-X
	    r = usb_claim_interface(h, 0);
	    if (r < 0) {
		usb_close(h);
//		printf_verbose("Unable to claim interface, check USB permissions");
		continue;
	    }
	    return h;
	}
    }
    return NULL;
}

static usb_dev_handle *libusb_teensy_handle = NULL;

int TeensyProgrammer::teensy_open(void)
{
    teensy_close();
    libusb_teensy_handle = open_usb_device(0x16C0, 0x0478);
    if (libusb_teensy_handle) return 1;
    return 0;
}

int TeensyProgrammer::teensy_write(void *buf, int len, double timeout)
{
    int r;

    if (!libusb_teensy_handle) return 0;
    r = usb_control_msg(libusb_teensy_handle, 0x21, 9, 0x0200, 0, (char *)buf,
	len, (int)(timeout * 1000.0));
    if (r < 0) return 0;
    return 1;
}

void TeensyProgrammer::teensy_close(void)
{
    if (!libusb_teensy_handle) return;
    usb_release_interface(libusb_teensy_handle, 0);
    usb_close(libusb_teensy_handle);
    libusb_teensy_handle = NULL;
}

int TeensyProgrammer::hard_reboot(void)
{
    usb_dev_handle *rebootor;
    int r;

    rebootor = open_usb_device(0x16C0, 0x0477);
    if (!rebootor) return 0;
    r = usb_control_msg(rebootor, 0x21, 9, 0x0200, 0, "reboot", 6, 100);
    usb_release_interface(rebootor, 0);
    usb_close(rebootor);
    if (r < 0) return 0;
    return 1;
}


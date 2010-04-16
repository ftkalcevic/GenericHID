#ifndef _TEENSYPROGRAMMER_H_
#define _TEENSYPROGRAMMER_H_

#include "deviceprogrammer.h"

struct usb_dev_handle;

class TeensyProgrammer: public DeviceProgrammer
{
public:
    TeensyProgrammer(void);
    virtual ~TeensyProgrammer(void);

    virtual bool GetDevice();
    virtual bool ReleaseDevice();
    virtual bool EraseDevice();
    virtual bool StartProgramming(IntelHexBuffer &memory);
    virtual bool StartVerify(IntelHexBuffer &memory);
    virtual bool EnterApplicationMode(ResetMode::ResetMode mode, unsigned int addr = 0);

protected:
    usb_dev_handle * open_usb_device(int vid, int pid);
    int teensy_open(void);
    int teensy_write(void *buf, int len, double timeout);
    void teensy_close(void);
    int hard_reboot(void);
};

#endif


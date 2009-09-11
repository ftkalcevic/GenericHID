#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stringtable.h"
#include "configuration.h"
#include "hidreportdescriptorbuilder.h"
#include "deviceconfig.h"

class Control : public Configuration
{
public:
    Control(void);
    virtual ~Control(void);
    static Control *MakeControl( const QDomElement &elem, QString *sError );

    enum Type
    {
	Input,
	Output,
	Display
    };

    virtual Type type() const = 0;
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const = 0;	// returns the HID report descriptor data for this control
    virtual ByteArray GetReportDescriptor(StringTable & /*table*/ ) const { return ByteArray(); }
};


#endif

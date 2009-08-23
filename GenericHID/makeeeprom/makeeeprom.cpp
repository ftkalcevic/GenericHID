#include "stdafx.h"
#include "makeeeprom.h"
#include "configurationconfig.h"
#include "configurationdevice.h"
#include "configurationendpoint.h"
#include "configurationhid.h"
#include "configurationinterface.h"
#include "control.h"
#include "usages.h"

const int MAX_STRINGS = 254;
const char *SELECT_DFU_MODE = "DFU";
const ushort EndpointInSize = 64;
const ushort EndpointOutSize = 64;

#define MAX(a,b)	((a) > (b) ? (a) : (b))


MakeEEPROM::MakeEEPROM()
{
    m_DeviceConfig = NULL;
    m_ConfigConfig = NULL;
}

MakeEEPROM::~MakeEEPROM()
{
    if ( m_DeviceConfig != NULL )
	delete m_DeviceConfig;
    if ( m_ConfigConfig != NULL )
	delete m_ConfigConfig;
    foreach( Control *control, m_Controls )
	delete control;
    m_Controls.clear();
}

bool MakeEEPROM::loadFile( const QString &sFile )
{
    QDomDocument doc("GenericHIDDevice");

    QFile file(sFile);
    if (!file.open(QIODevice::ReadOnly))
    {
	m_sLastError = file.errorString();
	return false;
    }

    if (!doc.setContent(&file, false, &m_sLastError)) 
    {
	file.close();
	return false;
    }
    file.close();

    return loadXML( doc );
}

bool MakeEEPROM::loadXML( const QString &sXML )
{
    QDomDocument doc("GenericHIDDevice");
    int nLine, nCol;
    if (!doc.setContent(sXML, false, &m_sLastError, &nLine, &nCol)) 
    {
	return false;
    }

    return loadXML( doc );
}

bool MakeEEPROM::loadXML( const QDomDocument &doc )
{
    QDomElement rootElement = doc.firstChildElement( "GenericHIDDevice" );
    if ( rootElement.isNull() )
    {
	m_sLastError = "Root node is not 'GenericHIDDevice'";
	return false;
    }

    QDomElement elem = XMLUtility::firstChildElement( rootElement, "Device" );
    if ( elem.isNull() )
    {
	m_sLastError = "Can't find 'Device' node";
	return false;
    }
    m_DeviceConfig = new ConfigurationDevice();
    if ( !m_DeviceConfig->Load(elem, &m_sLastError) )
	return false;

    elem = XMLUtility::firstChildElement( rootElement, "Configuration" );
    if ( elem.isNull() )
    {
	m_sLastError = "Can't find 'Configuration' node";
	return false;
    }
    m_ConfigConfig = new ConfigurationConfig();
    if ( !m_ConfigConfig->Load( elem, &m_sLastError ) )
	return false;

    elem = XMLUtility::firstChildElement( rootElement, "Controls" );
    if ( !elem.isNull() )
	for ( uint i = 0; i < elem.childNodes().length(); i++ )
	{
	    QDomElement control = elem.childNodes().item(i).toElement();
	    Control *pControl = Control::MakeControl( control, &m_sLastError );
	    if ( pControl == NULL )
		return false;
	    m_Controls.push_back( pControl );
	}

    return true;
}

ByteArray MakeEEPROM::makeEEPROM()
{
    StringTable table;

    // Create the report descriptor first - other descriptors require information.

    // Create application data as we go
    struct SApplicationHeader appHeader;
    memset( &appHeader, 0, sizeof(appHeader) );
    appHeader.nOptions = (m_ConfigConfig->busPowered() ? DEVICE_OPTION_BUS_POWERED : 0 ) |
			       (m_ConfigConfig->HIDDebug() ? DEVICE_OPTION_HID_DEBUG : 0 ) |
			       (m_ConfigConfig->serialDebug() ? DEVICE_OPTION_SERIAL_DEBUG : 0 ) |
			       (m_ConfigConfig->useStatusLEDs() == ConfigurationConfig::LED1 ? DEVICE_OPTION_USE_USBKEY_LED1 : 0 ) |
			       (m_ConfigConfig->useStatusLEDs() == ConfigurationConfig::LED2 ? DEVICE_OPTION_USE_USBKEY_LED2 : 0 ) |
			       (m_ConfigConfig->useStatusLEDs() == ConfigurationConfig::Both ? DEVICE_OPTION_USE_USBKEY_LEDS : 0 ) |
			       (m_ConfigConfig->is5Volts() ? DEVICE_OPTION_5V : 0 );
    for (int i = 0; i < countof(appHeader.ReportLength); i++)
	appHeader.ReportLength[i] = 0;
    for (int i = 0; i < countof(appHeader.timers); i++ )
	appHeader.timers[i] = m_ConfigConfig->timers()[i];

    ByteBuffer ApplicationData;

    // HIReport descriptor header
    HIDReportDescriptorBuilder HIDReport;
    HIDReport.UsagePage(m_ConfigConfig->usagePage());
    HIDReport.Usage(m_ConfigConfig->usage());
    HIDReport.Collection(CollectionType::Application);

    // create feature report for moving device into program mode
    HIDReport.UsagePage(USAGEPAGE_VENDOR_DEFINED);
    HIDReport.Usage(1);
    HIDReport.ReportID(BOOTLOADER_REPORT);
    HIDReport.ReportSize(32);	    // 4 bytes
    HIDReport.ReportCount(1);
    HIDReport.StringIndex(table[SELECT_DFU_MODE]);
    HIDReport.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

    byte nMaxInReportLen = 0;
    byte nMaxOutReportLen = 0;

    // group all the inputs
    byte nReportId = 1;             // 1 = input, 2 = output, 3=Feature, 4... = one for each display.
    bool bFirst = true;
    int nBits = 0;
    foreach (Control *c,m_Controls)
	if ( c->type() == Control::Input )
        {
            ByteBuffer descData = c->GetHIDReportDescriptor(table, nBits);
            ByteBuffer appData = c->GetControlConfig(nReportId);
            if (bFirst)
            {
                bFirst = false;
                HIDReport.ReportID(nReportId);   // output reports
            }
            HIDReport.AddBuffer(descData);
            ApplicationData.AddBuffer(appData);
        }
    if (nBits % 8 != 0)
    {
        // pad out the packet
        uint nPad = (uint)(8 - (nBits % 8));
        HIDReport.ReportCount(1);
        HIDReport.ReportSize(nPad);
	HIDReport.Input(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EBufferType::BitField);
    }

    byte nLength = (byte)((nBits + 7) / 8);
    appHeader.ReportLength[1-1] = nLength;   // report length (id=1)
    nMaxInReportLen = MAX(nMaxInReportLen, nLength );


    // all the outputs 
    bFirst = true;
    nBits = 0;
    nReportId = 2;
    foreach (Control *c, m_Controls)
	if ( c->type() == Control::Output )
        {
            ByteBuffer descData = c->GetHIDReportDescriptor(table, nBits);
            ByteBuffer appData = c->GetControlConfig(nReportId);
            if (bFirst)
            {
                bFirst = false;
                HIDReport.ReportID(nReportId);   // output reports
            }
            HIDReport.AddBuffer(descData);
            ApplicationData.AddBuffer(appData);
        }
    if (nBits % 8 != 0)
    {
        // pad out the packet
        uint nPad = (uint)(8 - (nBits % 8));
        HIDReport.ReportCount(1);
        HIDReport.ReportSize(nPad);
	HIDReport.Output(EDataType::Constant, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);
    }
    nLength = (byte)((nBits + 7) / 8);
    appHeader.ReportLength[2-1] = nLength;   // report length (id=2)
    appHeader.ReportLength[3-1] = (byte)4;   // report length (id=3)

    nMaxOutReportLen = MAX(nMaxOutReportLen,nLength);
    nMaxOutReportLen = MAX(nMaxOutReportLen,4);

    // any lcds
    nReportId = 4;
    foreach (Control *c, m_Controls)
	if (c->type() == Control::Display )
        {
            nBits = 0;
            ByteBuffer descData = c->GetHIDReportDescriptor(table, nBits);
            ByteBuffer appData = c->GetControlConfig(nReportId);
            HIDReport.ReportID(nReportId);
            HIDReport.AddBuffer(descData);
            ApplicationData.AddBuffer(appData);

            nLength = (byte)((nBits + 7) / 8);
            appHeader.ReportLength[nReportId-1] = nLength;       // report length (id=4+)

            nMaxOutReportLen = MAX( nMaxOutReportLen, nLength );
            nReportId++;
        }
    HIDReport.EndCollection();

    struct SControlHeader empty;
    empty.Type = None;
    empty.Length = 0;
    empty.ReportId = 0;
    ApplicationData.AddArray((byte *)&empty, sizeof(empty)); // NULL header to terminate the collection of devices

    if ( nMaxInReportLen > EndpointInSize || nMaxOutReportLen > EndpointOutSize )
    {
	if ( nMaxInReportLen > EndpointInSize )
	    m_sLastError = QString("The input report has exceeded the maximum input endpoint size %1 > %2").arg(nMaxInReportLen).arg(EndpointInSize);
	else
	    m_sLastError = QString("The output report has exceeded the maximum output endpoint size %1 > %2").arg(nMaxOutReportLen).arg(EndpointOutSize);
	return ByteArray();
    }

    //ApplicationData.AddByte(nMaxInReportLen);   // Max out report length
    //ApplicationData.AddByte(nMaxOutReportLen);  // Max in report length

    //// Add send/receive buffer space
    //for (int i = 0; i < 2 * nMaxInReportLen + nMaxOutReportLen; i++)
    //    ApplicationData.AddByte(0);


    /* Output is...
     *  pos len     item
     *   0   2     size of all the data
     *   2   2     offset to report descriptor
     *   4   2     report desc len
     *   6   2     offset to device descriptor
     *   8   2     device descriptor len
     *  10   2     offset to config descriptor
     *  12   2     config descriptor len
     *  14   2     offset to HID descriptor (nested inside config descriptor)
     *  16   2     HID descriptor len
     *  18   2     offset to application data
     *  20   2     offset to string item 1...
     *       data
     *      String table
     *      report descriptor
     *      device descriptor
     *      config descriptor
     *      application data
     * */

    struct SDynamicHID hidData;
    ushort nOffset = sizeof(hidData);

    ByteBuffer DeviceConfig = m_DeviceConfig->GetReportDescriptor(table);

    ConfigurationInterface InterfaceConfig;
    ByteBuffer IFaceConfig = InterfaceConfig.GetReportDescriptor(table);

    ConfigurationHID HIDConfigure(HIDReport.count());
    ByteBuffer HIDConfig = HIDConfigure.GetReportDescriptor(table);

    ConfigurationEndpoint Endpoint1(true, 1, TransferType::Interrupt, SynchronisationType::NoSync, UsageType::Data, EndpointInSize, 50);
    ByteBuffer EndpointConfig1 = Endpoint1.GetReportDescriptor(table);

    ConfigurationEndpoint Endpoint2(false, 2, TransferType::Interrupt, SynchronisationType::NoSync, UsageType::Data, EndpointOutSize, 50);
    ByteBuffer EndpointConfig2 = Endpoint2.GetReportDescriptor(table);

    short nConfigDescLength = (short)(IFaceConfig.count() + HIDConfig.count() + EndpointConfig1.count() + EndpointConfig2.count());
    m_ConfigConfig->setDescriptorsLength(nConfigDescLength);
    ByteBuffer ConfigConfig = m_ConfigConfig->GetReportDescriptor(table);

    ByteBuffer strTable = table.GetDescriptor(nOffset);
    nOffset += (ushort)strTable.count();
    hidData.nStringCount = table.count();
    if ( table.count() > MAX_STRINGS )
    {
	m_sLastError = QString("Exceeded the maximum number of strings that USB supports.  Have %1, Max %2").arg(table.count()).arg(MAX_STRINGS);
	return ByteArray();
    }

    hidData.nReportDescriptorOffset = nOffset;
    hidData.nReportDescriptorLength = HIDReport.count();
    nOffset += HIDReport.count();

    hidData.nDeviceDescriptorOffset = nOffset;
    hidData.nDeviceDescriptorLength = DeviceConfig.count();
    nOffset += DeviceConfig.count();

    hidData.nConfigDescriptorOffset = nOffset;
    hidData.nConfigDescriptorLength = ConfigConfig.count() + nConfigDescLength;            // length of config descriptor
    ushort nOffsetTemp = nOffset;
    nOffset += ConfigConfig.count() + IFaceConfig.count();

    hidData.nHIDDescriptorOffset = nOffset;			    // offset to HID descriptor - this is in the middle of the config descriptor
    hidData.nHIDDescriptorLength = HIDConfig.count();             // length of HID descriptor
    nOffset = nOffsetTemp + ConfigConfig.count() + nConfigDescLength;

    hidData.nApplicationDataOffset = nOffset;                      // offset to application data

    hidData.nBlockLength =	sizeof(hidData) + 
				strTable.count() + 
				HIDReport.count() + 
				DeviceConfig.count() + 
				ConfigConfig.count() + 
				IFaceConfig.count() + 
				HIDConfig.count() + 
				EndpointConfig1.count() + 
				EndpointConfig2.count() + 
				sizeof(appHeader) +
				ApplicationData.count();

    ByteBuffer eeprom;
    eeprom.AddArray((byte *)&hidData, sizeof(hidData));
    eeprom.AddBuffer(strTable);
    eeprom.AddBuffer(HIDReport);
    eeprom.AddBuffer(DeviceConfig);
    eeprom.AddBuffer(ConfigConfig);
    eeprom.AddBuffer(IFaceConfig);
    eeprom.AddBuffer(HIDConfig);
    eeprom.AddBuffer(EndpointConfig1);
    eeprom.AddBuffer(EndpointConfig2);
    eeprom.AddArray((byte *)&appHeader,sizeof(appHeader));
    eeprom.AddBuffer(ApplicationData);

    if ( eeprom.count() > MAX_HID_DATA )
    {
	m_sLastError = QString("Exceeded the EEPROM size.  %1 > %2").arg(eeprom.count()).arg(MAX_HID_DATA);
	return ByteArray();
    }

    //ATLTRACE("EEPROM Data\n");
    //for (int i = 0; i < eeprom.count(); i++)
    //{
        //if (i % 16 == 0)
	    //ATLTRACE( QString("\n%1  ").arg(i,4,16,QChar('0')).toAscii().constData() );
        //ATLTRACE(QString("%1 ").arg(eeprom[i],4,16,QChar('0')).toAscii().constData());
    //}
    //ATLTRACE("\n");

    return eeprom;
}



static QString HexStr(byte b)
{
    return QString("%1").arg(b,2,16,QChar('0'));
}
static QString HexStr(int b)
{
    return QString("%1").arg(b,4,16,QChar('0'));
}

QString MakeEEPROM::MakeIntelHexFormat( ByteArray &eeprom )
{
    QString sb;

    for (int i = 0; i < eeprom.count(); i += 16)
    {
        int nLen = eeprom.count() - i;
        if (nLen > 16)
            nLen = 16;

        byte nChecksum = 0;
        QString sOutput = ":";               // record mark
        sOutput += HexStr((byte)nLen);      // load reclen
        nChecksum += (byte)nLen;
        sOutput += HexStr(i);               // offset
        nChecksum += (byte)(i & 0xff);
        nChecksum += (byte)((i>>8) & 0xff);
        sOutput += "00";                    // rectype 00 - data
        for (int j = 0; j < nLen; j++)
        {
            sOutput += HexStr(eeprom[i + j]);   // data
            nChecksum += eeprom[i + j];
        }
        sOutput += HexStr((byte)(1 + ~nChecksum));                        // checksum
        sb += sOutput + "\n";
    }
    sb += ":00000001FF\n";      // EOF
    return sb;
}

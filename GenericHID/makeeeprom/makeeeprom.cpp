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

#include "stdafx.h"
#include "makeeeprom.h"
#include "configurationconfig.h"
#include "configurationdevice.h"
#include "configurationendpoint.h"
#include "configurationhid.h"
#include "configurationinterface.h"
#include "control.h"
#include "usages.h"
#include "controllcd.h"

const unsigned int MAX_STRINGS = 254;
const char *SELECT_DFU_MODE = "DFU";
const ushort EndpointInSize = 64;
const ushort EndpointOutSize = 64;

#define MAX(a,b)	((a) > (b) ? (a) : (b))


MakeEEPROM::MakeEEPROM()
: m_Logger( QCoreApplication::applicationName(), "MakeEEPROM" )
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
	LOG_DEBUG( m_Logger, "Failed to open xml file" );
	m_sLastError = file.errorString();
	return false;
    }

    if (!doc.setContent(&file, false, &m_sLastError)) 
    {
	file.close();
	LOG_DEBUG( m_Logger, "Failed to load xml from file" );
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
	LOG_DEBUG( m_Logger, "Failed to load xml string" );
	return false;
    }

    return loadXML( doc );
}

bool MakeEEPROM::loadXML( const QDomDocument &doc )
{
    LOG_DEBUG( m_Logger, "Reading xml" );

    QDomElement rootElement = doc.firstChildElement( "GenericHIDDevice" );
    if ( rootElement.isNull() )
    {
	m_sLastError = "Root node is not 'GenericHIDDevice'";
        LOG_DEBUG( m_Logger, QString("Failed to process xml - %1").arg(m_sLastError) );
	return false;
    }

    QDomElement elem = XMLUtility::firstChildElement( rootElement, "Device" );
    if ( elem.isNull() )
    {
	m_sLastError = "Can't find 'Device' node";
        LOG_DEBUG( m_Logger, QString("Failed to process xml - %1").arg(m_sLastError) );
	return false;
    }
    m_DeviceConfig = new ConfigurationDevice();
    if ( !m_DeviceConfig->Load(elem, &m_sLastError) )
    {
        LOG_DEBUG( m_Logger, QString("Failed to read device config - %1").arg(m_sLastError) );
	return false;
    }

    elem = XMLUtility::firstChildElement( rootElement, "Configuration" );
    if ( elem.isNull() )
    {
	m_sLastError = "Can't find 'Configuration' node";
        LOG_DEBUG( m_Logger, QString("Failed to process xml - %1").arg(m_sLastError) );
	return false;
    }
    m_ConfigConfig = new ConfigurationConfig();
    if ( !m_ConfigConfig->Load( elem, &m_sLastError ) )
    {
        LOG_DEBUG( m_Logger, QString("Failed to read ConfigConfig- %1").arg(m_sLastError) );
	return false;
    }

    elem = XMLUtility::firstChildElement( rootElement, "Controls" );
    if ( !elem.isNull() )
	for ( uint i = 0; i < elem.childNodes().length(); i++ )
	{
	    QDomElement control = elem.childNodes().item(i).toElement();
	    Control *pControl = Control::MakeControl( control, &m_sLastError );
	    if ( pControl == NULL )
	    {
		LOG_DEBUG( m_Logger, QString("Failed to read data for control <%1> line %2 - %3").arg(elem.nodeName()).arg(elem.lineNumber()).arg(m_sLastError) );
		return false;
	    }
	    m_Controls.push_back( pControl );
	}

    LOG_DEBUG( m_Logger, "XML Read successfully" );
    return true;
}

ByteArray MakeEEPROM::makeEEPROM()
{
    LOG_DEBUG( m_Logger, "Making EEPROM" );

    StringTable table;

    // Create the report descriptor first - other descriptors require information.

    // Create application data as we go
    struct SApplicationHeader appHeader;
    memset( &appHeader, 0, sizeof(appHeader) );
    appHeader.nPowerPort = m_ConfigConfig->powerPort();
    appHeader.nOptions = (m_ConfigConfig->busPowered() ? DEVICE_OPTION_BUS_POWERED : 0 ) |
			       (m_ConfigConfig->HIDDebug() ? DEVICE_OPTION_HID_DEBUG : 0 ) |
			       (m_ConfigConfig->serialDebug() ? DEVICE_OPTION_SERIAL_DEBUG : 0 ) |
			       (m_ConfigConfig->useStatusLEDs() == ConfigurationConfig::LED1 ? DEVICE_OPTION_USE_USBKEY_LED1 : 0 ) |
			       (m_ConfigConfig->useStatusLEDs() == ConfigurationConfig::LED2 ? DEVICE_OPTION_USE_USBKEY_LED2 : 0 ) |
			       (m_ConfigConfig->useStatusLEDs() == ConfigurationConfig::Both ? DEVICE_OPTION_USE_USBKEY_LEDS : 0 ) |
			       (m_ConfigConfig->is5Volts() ? DEVICE_OPTION_5V : 0 );
    for (unsigned int i = 0; i < countof(appHeader.ReportLength); i++)
	appHeader.ReportLength[i] = 0;
    for (unsigned int i = 0; i < countof(appHeader.timers); i++ )
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
	    ControlLCD *lcd = dynamic_cast<ControlLCD *>( c );
            nBits = 0;
            ByteBuffer descData = lcd->GetHIDReportDescriptor(table, nBits, nReportId);
            ByteBuffer appData = c->GetControlConfig(nReportId);
            HIDReport.AddBuffer(descData);
            ApplicationData.AddBuffer(appData);

            nLength = (byte)((nBits + 7) / 8);
            appHeader.ReportLength[nReportId-1] = nLength;  // report length (id=4+)
            appHeader.ReportLength[nReportId] = 6;	    // font report length (id=4+)

            nMaxOutReportLen = MAX( nMaxOutReportLen, nLength );
            nReportId += 2;
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

    ConfigurationEndpoint Endpoint1(true, 1, TransferType::Interrupt, SynchronisationType::NoSync, UsageType::Data, EndpointInSize, m_ConfigConfig->interval());
    ByteBuffer EndpointConfig1 = Endpoint1.GetReportDescriptor(table);

    ConfigurationEndpoint Endpoint2(false, 2, TransferType::Interrupt, SynchronisationType::NoSync, UsageType::Data, EndpointOutSize, m_ConfigConfig->interval());
    ByteBuffer EndpointConfig2 = Endpoint2.GetReportDescriptor(table);

    short nConfigDescLength = (short)(IFaceConfig.count() + HIDConfig.count() + EndpointConfig1.count() + EndpointConfig2.count());
    m_ConfigConfig->setDescriptorsLength(nConfigDescLength);
    ByteBuffer ConfigConfig = m_ConfigConfig->GetReportDescriptor(table);

    ByteBuffer strTable = table.GetDescriptor(nOffset);
    nOffset = (ushort)(nOffset + strTable.count());
    hidData.nStringCount = (uint16_t)table.count();
    if ( table.count() > MAX_STRINGS )
    {
	m_sLastError = QString("Exceeded the maximum number of strings that USB supports.  Have %1, Max %2").arg(table.count()).arg(MAX_STRINGS);
	return ByteArray();
    }

    hidData.nReportDescriptorOffset = nOffset;
    hidData.nReportDescriptorLength = (uint16_t)HIDReport.count();
    nOffset = (ushort)(nOffset + HIDReport.count());

    hidData.nDeviceDescriptorOffset = nOffset;
    hidData.nDeviceDescriptorLength = (uint16_t)DeviceConfig.count();
    nOffset = (ushort)(nOffset + DeviceConfig.count());

    hidData.nConfigDescriptorOffset = nOffset;
    hidData.nConfigDescriptorLength = (uint16_t)(ConfigConfig.count() + nConfigDescLength);            // length of config descriptor
    ushort nOffsetTemp = nOffset;
    nOffset = (ushort)(nOffset + ConfigConfig.count() + IFaceConfig.count());

    hidData.nHIDDescriptorOffset = nOffset;			    // offset to HID descriptor - this is in the middle of the config descriptor
    hidData.nHIDDescriptorLength = (uint16_t)HIDConfig.count();             // length of HID descriptor
    nOffset = (ushort)(nOffsetTemp + ConfigConfig.count() + nConfigDescLength);

    hidData.nApplicationDataOffset = nOffset;                      // offset to application data

    hidData.nBlockLength = (uint16_t)( sizeof(hidData) + 
					strTable.count() + 
					HIDReport.count() + 
					DeviceConfig.count() + 
					ConfigConfig.count() + 
					IFaceConfig.count() + 
					HIDConfig.count() + 
					EndpointConfig1.count() + 
					EndpointConfig2.count() + 
					sizeof(appHeader) +
					ApplicationData.count() );

    ByteBuffer eeprom;
    eeprom.AddArray((byte *)&hidData, sizeof(hidData));
	LOG_DEBUG( m_Logger, QString("Adding SDynamicHID %1").arg(ByteBuffer((byte*)&hidData,sizeof(hidData)).toString()) );
    eeprom.AddBuffer(strTable);
	LOG_DEBUG( m_Logger, QString("Adding String Table %1").arg(strTable.toString()) );
    eeprom.AddBuffer(HIDReport);
	LOG_DEBUG( m_Logger, QString("Adding HID Report Descriptor %1").arg(HIDReport.toString()) );
    eeprom.AddBuffer(DeviceConfig);
	LOG_DEBUG( m_Logger, QString("Adding Device Descriptor %1").arg(DeviceConfig.toString()) );
    eeprom.AddBuffer(ConfigConfig);
	LOG_DEBUG( m_Logger, QString("Adding Config Descriptor %1").arg(ConfigConfig.toString()) );
    eeprom.AddBuffer(IFaceConfig);
	LOG_DEBUG( m_Logger, QString("Adding Interface Descriptor %1").arg(IFaceConfig.toString()) );
    eeprom.AddBuffer(HIDConfig);
	LOG_DEBUG( m_Logger, QString("Adding HID Config Descriptor %1").arg(HIDConfig.toString()) );
    eeprom.AddBuffer(EndpointConfig1);
	LOG_DEBUG( m_Logger, QString("Adding Endpoint Descriptor1 %1").arg(EndpointConfig1.toString()) );
    eeprom.AddBuffer(EndpointConfig2);
	LOG_DEBUG( m_Logger, QString("Adding Endpoint Descriptor2 %1").arg(EndpointConfig2.toString()) );
    eeprom.AddArray((byte *)&appHeader,sizeof(appHeader));
	LOG_DEBUG( m_Logger, QString("Adding Application Header %1").arg(ByteBuffer((byte *)&appHeader,sizeof(appHeader)).toString()) );
    eeprom.AddBuffer(ApplicationData);
	LOG_DEBUG( m_Logger, QString("Adding Application Data %1").arg(ApplicationData.toString()) );

    LOG_DEBUG( m_Logger, QString("EEPROM... %1").arg(eeprom.toString()) );

    assert( eeprom.count() == hidData.nBlockLength );
    if ( eeprom.count() > MAX_HID_DATA )
    {
	m_sLastError = QString("Exceeded the EEPROM size.  %1 > %2").arg(eeprom.count()).arg(MAX_HID_DATA);
	LOG_DEBUG( m_Logger, m_sLastError );
	return ByteArray();
    }

    LOG_DEBUG( m_Logger, "Making EEPROM complete" );
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
        nChecksum = (byte)(nChecksum + nLen);
        sOutput += HexStr(i);               // offset
        nChecksum = (byte)( nChecksum + (i & 0xff));
        nChecksum = (byte)( nChecksum + ((i>>8) & 0xff) );
        sOutput += "00";                    // rectype 00 - data
        for (int j = 0; j < nLen; j++)
        {
            sOutput += HexStr(eeprom[i + j]);   // data
            nChecksum = (byte)(nChecksum + eeprom[i + j]);
        }
        sOutput += HexStr((byte)(1 + ~nChecksum));                        // checksum
        sb += sOutput + "\n";
    }
    sb += ":00000001FF\n";      // EOF
    return sb;
}

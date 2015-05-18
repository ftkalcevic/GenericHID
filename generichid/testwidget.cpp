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
#include "testwidget.h"
#include "hiddevices.h"
#include "testinput.h"
#include "testoutput.h"
#include "testoutputlcd.h"
#include "usages.h"


class AutoSet
{
    bool *m_pFlag;
public:
    AutoSet( bool *pFlag ) 
    {
        m_pFlag = pFlag;
        *m_pFlag = true;
    }
    ~AutoSet() 
    {
        *m_pFlag = false;
    }
};

TestWidget::TestWidget(QWidget *parent)
    : QWidget(parent)
    , m_Logger(QCoreApplication::applicationName(), "TestWidget" )
    , m_pDevices( NULL )
    , m_pActiveDevice( NULL )
    , m_bLoading( false )
    , m_pThread( NULL )
{
    QVBoxLayout *main_layout = new QVBoxLayout();
    
    QHBoxLayout *select_layout = new QHBoxLayout();
    m_cboDevices = new QComboBox( this );
    m_cboDevices->setSizeAdjustPolicy( QComboBox::AdjustToContents );
    m_btnRefresh = new QPushButton( "Refresh", this );
    select_layout->addWidget( m_cboDevices );
    select_layout->addWidget( m_btnRefresh );
    select_layout->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    
    connect( m_cboDevices, SIGNAL(currentIndexChanged( int )), this, SLOT(onDevicesIndexChanged(int)) );
    connect( m_btnRefresh, SIGNAL(pressed()), this, SLOT(onRefreshPressed()) );
    
    m_pDeviceLayout = new ColumnLayout();
    //m_pDeviceLayout->addItem( new QSpacerItem( 0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum ) );
    
    main_layout->addLayout( select_layout );
    main_layout->addLayout( m_pDeviceLayout );
    
    setLayout( main_layout );
    
    //   setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

TestWidget::~TestWidget()
{
}

void TestWidget::Activate()
{
    onRefreshPressed();
}

void TestWidget::Deactivate()
{
    AutoSet set( &m_bLoading );
    
    DisplayDevice( NULL );
    m_cboDevices->clear();
    if ( m_pDevices != NULL )
    {
        delete m_pDevices;
        m_pDevices = NULL;
    }
}

void TestWidget::onDevicesIndexChanged( int index )
{
    if ( m_bLoading )
        return;
    
    if ( index < 0 || index > (int)(m_pDevices->m_Devices.size() -1) )
        DisplayDevice( NULL );
    else
    {
        DisplayDevice( NULL );
        DisplayDevice( m_pDevices->m_Devices[index] );
    }
}

static QString MakeDeviceString( HIDDevice *pDevice )
{
    QString s;
    
    pDevice->Open();
    
    if ( pDevice->hasOpenErrors() )
        s += "* ";
    if ( pDevice->Manufacturer().length() > 0 )
        s += QString( "%1(%2) ").arg(pDevice->Manufacturer()).arg(pDevice->VID(),4,16,QChar('0'));
    else
        s += QString( "VID:%1 ").arg(pDevice->VID(),4,16,QChar('0'));
    
    if ( pDevice->Product().length() > 0 )
        s += QString( "%1(%2) ").arg(pDevice->Product()).arg(pDevice->PID(),4,16,QChar('0'));
    else
        s += QString( "PID:%1 ").arg(pDevice->PID(),4,16,QChar('0'));
    
    if ( pDevice->SerialNumber().length() > 0 )
        s += QString(" S/N:%1").arg(pDevice->SerialNumber());
    
    pDevice->Close();
    
    return s;
}

void TestWidget::onRefreshPressed()
{
    AutoSet set( &m_bLoading );
    
    if ( m_pActiveDevice != NULL )
        DisplayDevice( NULL );
    
    // reload the device combo
    m_cboDevices->clear();
    
    if ( m_pDevices != NULL )
        delete m_pDevices;
    m_pDevices = new HIDDevices();
    m_pActiveDevice = NULL;
    
    if ( !m_pDevices->FindHIDDevices() )
    {
        assert( false ); // todo
        LOG_MSG(m_Logger, LogTypes::Warning, QString("Failed to retreive list of hid devices") );
        return;
    }
    
    int nSelectGenericHID = -1;
    int nSelectLastDevice = -1;
    for ( int i = 0; i < (int)m_pDevices->m_Devices.size(); i++ )
    {
        HIDDevice *pDevice = m_pDevices->m_Devices[i];
        if ( !m_sLastDevice.isEmpty() && m_sLastDevice == pDevice->SystemId() )
            nSelectLastDevice = i;
        if ( pDevice->VID() == FRANKSWORKSHOP_VID && pDevice->PID() == GENERICHID_PID )
            nSelectGenericHID = i;
        m_cboDevices->addItem( MakeDeviceString( pDevice ), QVariant(i) );
    }
    
    int nSelect=-1;
    if ( nSelectGenericHID >= 0 )
        nSelect = nSelectGenericHID;
    if ( nSelectLastDevice >= 0 )
        nSelect = nSelectLastDevice;
    
    m_cboDevices->setCurrentIndex( nSelect );
    if ( nSelect >= 0 )
    {
        DisplayDevice( m_pDevices->m_Devices[nSelect] );
    }
}


void TestWidget::DisplayDevice( HIDDevice *pDevice )
{
    if ( m_pActiveDevice != NULL && pDevice != NULL )
        DisplayDevice( NULL );
    
    if ( pDevice == NULL )
    {
        StopListening();
        m_pDeviceLayout->clear();   // this clear deletes the widgets.
        foreach ( TestItem *pItem, m_pTestItems )
            delete pItem;
        m_pTestItems.clear();
        m_pActiveDevice = NULL;
    }
    else
    {
        if ( !pDevice->PreprocessReportData() )
        {
            return;
        }
        
        HID_ReportInfo_t &info = pDevice->ReportInfo();
        
        // Go through collections and find USAGE_PAGE_ALPHANUMERIC_DISPLAY devices
        for ( int i = 0; i < (int)info.Collections.size(); i++ )
        {
            HID_CollectionPath_t *col = info.Collections[i];
            if ( col->UsagePage == USAGEPAGE_ALPHANUMERIC_DISPLAY && col->Usage == USAGE_ALPHANUMERIC_DISPLAY )
            {
                TestOutputLCD *pLCD = new TestOutputLCD( pDevice, col );
                if ( pLCD != NULL )
                {
                    m_pTestItems.append( pLCD );	
                    m_pDeviceLayout->addWidget( pLCD );	
                    
                    //connect( pOut, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)) );
                }
            }
        }
        
        for ( int i = 0; i < (int)info.ReportItems.size(); i++ )
        {
            HID_ReportItem_t *pReportItem = info.ReportItems[i];
            
            TestItem *pTestItem = TestItem::CreateItem( pDevice, pReportItem );
            if ( pTestItem != NULL )
            {
                m_pTestItems.append( pTestItem );	
                m_pDeviceLayout->addWidget( pTestItem );	
                
                if ( !pTestItem->isInput() )
                {
                    TestOutput *pOut = dynamic_cast<TestOutput *>( pTestItem );
                    if ( pOut != NULL )
                        connect( pOut, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)) );
                }
                
            }
        }
        
        m_pDeviceLayout->invalidate();
        m_pActiveDevice = pDevice;
        m_sLastDevice = pDevice->SystemId();
        StartListening();
    }
}


void TestWidget::StartListening()
{
    m_pThread = new HIDDataThread( m_pActiveDevice );
    
    qRegisterMetaType<QVector<byte> >("QVector<byte>");
    connect(m_pThread, SIGNAL(newData( QVector<byte>  )), this, SLOT(onNewData( QVector<byte>  )));
    
    m_pThread->start();
}


void TestWidget::StopListening()
{
    if ( m_pThread != NULL )
    {
        m_pThread->stop();
        if ( m_pThread->wait( 3*HIDDataThread::LOOP_TIMEOUT ) )
        {
            // This is a bit ugly.  We only delete the thread object if it exited.
            // If it didn't exit after waiting, we just let it die on its
            // own, leaking memory.  If we delete it while it is still running it
            // will probably SEGV.  There is also the risk that the USB device hasn't 
            // been released.
            delete m_pThread;
        }
        m_pThread = NULL;
    }
}


void TestWidget::onNewData( QVector<byte> data )
{
    if ( m_pActiveDevice == NULL )
        return;
    
    // unpack the data
    byte nReportId = 0;
    if ( m_pActiveDevice->ReportInfo().Reports.size() > 1 )
    {
        // multiple reports
        nReportId = data[0];
        data.pop_front();
    }
    
    // Decode the packet
    HIDParser parser;
    parser.DecodeReport( data.data(), (byte)data.size(), m_pActiveDevice->ReportInfo().ReportItems, nReportId, REPORT_ITEM_TYPE_In );
    
    // Update the display
    foreach( TestItem *pItem, m_pTestItems )
        if ( pItem->isInput() && pItem->reportId() == nReportId )
        {
            TestInput *pInputItem = dynamic_cast<TestInput *>( pItem );
            pInputItem->Update();
        }
}





void TestWidget::onValueChanged(int _nReportId)
{
    byte nReportId = (byte)_nReportId;
    // an output item has changed.  Send the report it belongs to.
    // make a buffer
    int nBufferLen = m_pActiveDevice->ReportInfo().Reports.size() > 1 ? 1 : 0;
    nBufferLen += m_pActiveDevice->ReportInfo().Reports[nReportId].OutReportLength;
    
    unsigned char *buffer = new unsigned char [ nBufferLen ];
    unsigned char *ptr = buffer;
    int len = nBufferLen;
    if ( m_pActiveDevice->ReportInfo().Reports.size() > 1 )
    {
        *ptr = nReportId;
        ptr++;
        len--;
    }
    // Make the report
    HIDParser parser;
    parser.MakeOutputReport( ptr, (byte)len, m_pActiveDevice->ReportInfo().ReportItems, nReportId );
    
    // Send the report
    int nRet = m_pActiveDevice->InterruptWrite( buffer, nBufferLen, USB_TIMEOUT );
    if ( nRet != nBufferLen )
    { 
        LOG_MSG(m_Logger, LogTypes::Warning, QString("Failed to write report.  Expected %1 got %2").arg(nBufferLen).arg(nRet) );
    }
    
    delete buffer;
}


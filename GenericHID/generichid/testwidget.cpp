#include "stdafx.h"
#include "testwidget.h"
#include "hiddevices.h"
#include "testinput.h"
#include "testoutput.h"


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
, m_pDevices( NULL )
, m_pActiveDevice( NULL )
, m_bLoading( false )
, m_pThread( NULL )
{
    QVBoxLayout *main_layout = new QVBoxLayout();

    QHBoxLayout *select_layout = new QHBoxLayout();
    m_cboDevices = new QComboBox( this );
    m_cboDevices->setSizeAdjustPolicy( QComboBox::SizeAdjustPolicy::AdjustToContents );
    m_btnRefresh = new QPushButton( this );
    select_layout->addWidget( m_cboDevices );
    select_layout->addWidget( m_btnRefresh );
    select_layout->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum ) );

    connect( m_cboDevices, SIGNAL(currentIndexChanged( int )), this, SLOT(onDevicesIndexChanged(int)) );
    connect( m_btnRefresh, SIGNAL(pressed()), this, SLOT(onRefreshPressed()) );

    m_pDeviceLayout = new ColumnLayout();
    //m_pDeviceLayout->addItem( new QSpacerItem( 0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum ) );

    main_layout->addLayout( select_layout );
    main_layout->addLayout( m_pDeviceLayout );

    setLayout( main_layout );
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
    m_pActiveDevice = NULL;
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

    if ( index < 0 || index > m_pDevices->m_Devices.size() -1 )
	DisplayDevice( NULL );
    else
	DisplayDevice( m_pDevices->m_Devices[index] );
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

    // reload the device combo
    m_cboDevices->clear();

    if ( m_pDevices != NULL )
	delete m_pDevices;
    m_pDevices = new HIDDevices();
    m_pActiveDevice = NULL;

    if ( !m_pDevices->FindHIDDevices() )
    {
	assert( false ); // todo
	return;
    }

    int nSelect = -1;
    for ( int i = 0; i < m_pDevices->m_Devices.size(); i++ )
    {
	HIDDevice *pDevice = m_pDevices->m_Devices[i];
	if ( pDevice->VID() == FRANKSWORKSHOP_VID && pDevice->PID() == GENERICHID_PID )
	    nSelect = i;
	m_cboDevices->addItem( MakeDeviceString( pDevice ), QVariant(i) );
    }
    
    m_cboDevices->setCurrentIndex( nSelect );
    if ( nSelect >= 0 )
	DisplayDevice( m_pDevices->m_Devices[nSelect] );
}


void TestWidget::DisplayDevice( HIDDevice *pDevice )
{
    if ( m_pActiveDevice != NULL && pDevice != NULL )
	DisplayDevice( NULL );

    if ( pDevice == NULL )
    {
	StopListening();
	m_pDeviceLayout->clear();   // this clear deletes the widgets.
	m_pTestItems.clear();
	m_pActiveDevice = NULL;
    }
    else
    {
	if ( !pDevice->PreprocessReportData() )
	{
	    assert( false );
	    return;
	}

	HID_ReportInfo_t &info = pDevice->ReportInfo();
	for ( int i = 0; i < info.ReportItems.size(); i++ )
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
	StartListening();
    }
}


void TestWidget::StartListening()
{
    assert( m_pThread == NULL );
    m_pThread = new HIDDataThread( m_pActiveDevice );

    qRegisterMetaType<QVector<byte> >("QVector<byte>");
    connect(m_pThread, SIGNAL(newData( QVector<byte>  )), this, SLOT(onNewData( QVector<byte>  )));

    m_pThread->start();
}


void TestWidget::StopListening()
{
    assert( m_pThread != NULL );
    m_pThread->stop();
    m_pThread->wait( 250 );
    delete m_pThread;
    m_pThread = NULL;
}


void TestWidget::onNewData( QVector<byte> data )
{
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





void TestWidget::onValueChanged(int nReportId)
{
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

    delete buffer;
}


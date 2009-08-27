#include "stdafx.h"
#include "hidparser.h"
#include "testoutputlcd.h"
#include "lcdwidget.h"
#include "usages.h"

TestOutputLCD::TestOutputLCD( HIDDevice *pDevice, HID_CollectionPath_t *pCollection, QWidget *parent )
: TestOutput(0,parent)
, m_pDevice( pDevice )
, m_pCol( pCollection )
{
    QHBoxLayout *layout2 = new QHBoxLayout();
    m_pLCD = new LCDWidget();

    QLabel *label = new QLabel("LCD");
    QPushButton *button = new QPushButton( "Clear" );
    
    layout2->addWidget( label );
    layout2->addWidget( button );
    layout2->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    connect( button, SIGNAL(pressed()), this, SLOT(onClearPressed()) );

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout( layout2 );
    layout->addWidget( m_pLCD );

    connect( m_pLCD, SIGNAL(clear()), this, SLOT(onLCDClear()) );
    connect( m_pLCD, SIGNAL(write(int,int,const QString &)), this, SLOT(onLCDWrite(int,int,const QString &)) );

    getLCDAttributes( m_nRows, m_nCols );
    m_pLCD->setSize( m_nRows, m_nCols );
    //m_pLCD->setMaximumHeight( m_nRows*15 );

    setLayout( layout );
}

TestOutputLCD::~TestOutputLCD()
{

}


void TestOutputLCD::onClearPressed()
{
    m_pLCD->Clear();
}


void TestOutputLCD::onLCDClear()
{
    QString s( m_nCols, QChar(' ') );
    for ( int i = 0; i < m_nRows; i++ )
	LCDWrite(i,0,s);
}

void TestOutputLCD::onLCDWrite(int nRow,int nCol,const QString &s)
{
    LCDWrite(nRow,nCol,s);
}


static HID_ReportItem_t *FindReportItem( HIDDevice *pDevice, HID_CollectionPath_t *pCol, HID_ReportItemTypes_t type, unsigned short nUsagePage, unsigned short nUsage )
{
    // Check collection
    for ( unsigned int i = 0; i < pCol->ReportItems.size(); i++ )
	if ( pCol->ReportItems[i]->ItemType == type && pCol->ReportItems[i]->Attributes.UsagePage == nUsagePage && pCol->ReportItems[i]->Attributes.Usage == nUsage )
	    return pCol->ReportItems[i];

    // Look for sub collections
    for ( unsigned int i = 0; i < pDevice->ReportInfo().Collections.size(); i++ )
    {
	HID_CollectionPath_t *pSubCol = pDevice->ReportInfo().Collections[i];
	while ( pSubCol != NULL && pSubCol->Parent != pCol )
	    pSubCol = pSubCol->Parent;
	if ( pSubCol != NULL )
	{
	    for ( unsigned int i = 0; i < pSubCol->ReportItems.size(); i++ )
		if ( pSubCol->ReportItems[i]->ItemType == type && pSubCol->ReportItems[i]->Attributes.UsagePage == nUsagePage && pSubCol->ReportItems[i]->Attributes.Usage == nUsage )
		    return pSubCol->ReportItems[i];
	}
    }
    return NULL;
}

static HID_ReportItem_t *FindReportItem( HID_CollectionPath_t *pCol, unsigned short nUsage )
{
    for ( unsigned int i = 0; i < pCol->ReportItems.size(); i++ )
	if ( pCol->ReportItems[i]->Attributes.Usage == nUsage )
	    return pCol->ReportItems[i];
    return NULL;
}

void TestOutputLCD::getLCDAttributes( int &nRows, int &nCols )
{
    nRows = 0;
    nCols = 0;

    // find the feature report that contains the rows and columns count.  That's all we want.
    HID_ReportItem_t *pRowItem = FindReportItem( m_pDevice, m_pCol, REPORT_ITEM_TYPE_Feature, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_ROWS );
    HID_ReportItem_t *pColItem = FindReportItem( m_pDevice, m_pCol, REPORT_ITEM_TYPE_Feature, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_COLUMNS );

    if ( pRowItem == NULL || pColItem == NULL )
    {
	// todo
	return;
    }
    assert( pColItem->ReportID == pRowItem->ReportID );
    if ( pColItem->ReportID != pRowItem->ReportID )	    // assume row and column count in one report
	return;

    HID_ReportDetails_t pReportDetails = m_pDevice->ReportInfo().Reports[pRowItem->ReportID];
    int nBufLen = pReportDetails.FeatureReportLength;
    int nOffset = 0;
    if ( m_pDevice->ReportInfo().Reports.size() > 1 )
	nOffset=1;

    byte *buf = new byte[nBufLen+nOffset];
    bool b = m_pDevice->GetReport( pRowItem->ReportID, REPORT_ITEM_TYPE_Feature, buf, nBufLen + nOffset );
    if ( !b ) 
    {
        //LOG_MSG(m_Logger, LogTypes::Warning, QString("Failed to retreive feature report") );
    }
    else
    {
	HIDParser parser;
	parser.DecodeReport( buf+nOffset, (byte)nBufLen, m_pDevice->ReportInfo().ReportItems, pRowItem->ReportID, REPORT_ITEM_TYPE_Feature );
	nRows = pRowItem->Value;
	nCols = pColItem->Value;
	delete buf;
    }

    // Find the report items for sending data - row, col, data array

    return;
}




void TestOutputLCD::LCDWrite( int nRow, int nCol, QString sText )
{
    HID_ReportItem_t *pRowItem = FindReportItem( m_pDevice, m_pCol, REPORT_ITEM_TYPE_Out, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_ROW );
    HID_ReportItem_t *pColItem = FindReportItem( m_pDevice, m_pCol, REPORT_ITEM_TYPE_Out, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_COLUMN );

    HID_CollectionPath_t *pCollection = pColItem->CollectionPath;

    // Find the index to the first data item
    unsigned int nIndex = 0;
    for ( ; nIndex < pCollection->ReportItems.size(); nIndex++ )
	if ( pCollection->ReportItems[nIndex]->Attributes.Usage == USAGE_DISPLAY_DATA )
	    break;

    pRowItem->Value = nRow;
    pColItem->Value = nCol;
    for ( int i = 0; i < sText.length() && nIndex+i < (int)m_nCols; i++ )
	pCollection->ReportItems[nIndex + i]->Value = sText[i].toAscii();
    if ( sText.length() < m_nCols )
	pCollection->ReportItems[nIndex + sText.length()]->Value = 0;

    HID_ReportDetails_t pReportDetails = m_pDevice->ReportInfo().Reports[pRowItem->ReportID];
    int nBufLen = pReportDetails.OutReportLength;
    int nOffset = 0;
    if ( m_pDevice->ReportInfo().Reports.size() > 1 )
	nOffset=1;

    byte *buf = new byte[nBufLen+nOffset];
    if ( nOffset )
	*buf = pRowItem->ReportID;

    HIDParser parser;
    parser.MakeOutputReport( buf + nOffset, (byte)nBufLen, m_pDevice->ReportInfo().ReportItems, pRowItem->ReportID );

    // Send the report
    int nRet = m_pDevice->InterruptWrite( buf, nBufLen + nOffset, USB_TIMEOUT );
    //LOG_MSG( m_Logger, LogTypes::Debug, QString("interrupt write returned %1\n").arg(nRet).toLatin1().constData() );

    delete buf;
}

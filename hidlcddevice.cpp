#include "hidlcddevice.h"
#include "usages.h"

HIDLCDDevice::HIDLCDDevice(HIDDevice *pDevice, HID_CollectionPath_t *pCol)
: m_Logger(QCoreApplication::applicationName(), "HIDLCDDevice" )
, m_pDevice( pDevice )
, m_pLCDReportCol( pCol )
, m_pRowItem( NULL )
, m_pColItem( NULL )
, m_pCharacterReportCol( NULL )
, m_nCharacterReportId(0)
, m_nCharacterReportOffset(0)
, m_nCharacterDataIndex( 0 )
, m_nRows( 2 )
, m_nCols( 16 )
, m_bUserFonts( false )
, m_nMinFontIndex(0)
, m_nMaxFontIndex(0)
, m_bInitialised( false )

{
}

HIDLCDDevice::~HIDLCDDevice(void)
{
}

bool HIDLCDDevice::Init()
{
    bool bFailed = false;
    bFailed = bFailed || !getDisplayAttributes( m_nRows, m_nCols );
    bFailed = bFailed || !getFontAttributes( m_bUserFonts, m_nMinFontIndex, m_nMaxFontIndex );
    bFailed = bFailed || !initWriteParameters();
    m_bInitialised = !bFailed;
    return !bFailed;
}


bool HIDLCDDevice::getDisplayAttributes( int &nRows, int &nCols )
{
    nRows = 0;
    nCols = 0;

    // find the feature report that contains the rows and columns count.  That's all we want.
    HID_ReportItem_t *pRowItem = m_pDevice->ReportInfo().FindReportItem( m_pLCDReportCol, REPORT_ITEM_TYPE_Feature, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_DISPLAY_ATTRIBUTES_REPORT, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_ROWS );
    HID_ReportItem_t *pColItem = m_pDevice->ReportInfo().FindReportItem( m_pLCDReportCol, REPORT_ITEM_TYPE_Feature, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_DISPLAY_ATTRIBUTES_REPORT, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_COLUMNS );

    if ( pRowItem == NULL || pColItem == NULL )
    {
        LOG_MSG(m_Logger, LogTypes::Warning, QString("Failed to find rows or columns item in display attribute report") );
	return false;
    }

    if ( pColItem->ReportID != pRowItem->ReportID )	    // assume row and column count in one report
    {
        LOG_MSG(m_Logger, LogTypes::Warning, QString("Rows and columns items in display attribute report are in different reports") );
	return false;
    }
    byte nReportID = pColItem->ReportID;

    HID_ReportDetails_t pReportDetails = m_pDevice->ReportInfo().Reports[pRowItem->ReportID];
    int nBufLen = pReportDetails.FeatureReportLength;
    int nOffset = 0;
    if ( m_pDevice->ReportInfo().Reports.size() > 1 )
	nOffset=1;

    QVarLengthArray<byte> buf(nBufLen+nOffset);
    bool b = m_pDevice->GetReport( nReportID, REPORT_ITEM_TYPE_Feature, buf.data(), nBufLen + nOffset );
    if ( !b ) 
    {
        LOG_MSG(m_Logger, LogTypes::Warning, "Failed to retreive display attributes feature report" );
	return false;
    }
    else
    {
	HIDParser parser;
	parser.DecodeReport( buf.data()+nOffset, (byte)nBufLen, m_pDevice->ReportInfo().ReportItems, nReportID, REPORT_ITEM_TYPE_Feature );
	nRows = pRowItem->Value;
	nCols = pColItem->Value;
	return true;
    }
}



bool HIDLCDDevice::getFontAttributes( bool &bUserFonts, int &nMinIndex, int &nMaxIndex )
{
    // We have user fonts, if there is a font report.
    HID_ReportItem_t *pFontDataItem = m_pDevice->ReportInfo().FindReportItem( m_pLCDReportCol, REPORT_ITEM_TYPE_Out, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_FONT_REPORT, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_DISPLAY_DATA );
    if ( pFontDataItem == NULL )
	bUserFonts = false;
    else
    {
	bUserFonts = true;
	nMinIndex = pFontDataItem->Attributes.LogicalMinimum;
	nMaxIndex = pFontDataItem->Attributes.LogicalMaximum;
    }

    return true;
}



bool HIDLCDDevice::initWriteParameters( )
{
    m_pRowItem = m_pDevice->ReportInfo().FindReportItem( m_pLCDReportCol, REPORT_ITEM_TYPE_Out, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_CHARACTER_REPORT, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_ROW );
    m_pColItem = m_pDevice->ReportInfo().FindReportItem( m_pLCDReportCol, REPORT_ITEM_TYPE_Out, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_CHARACTER_REPORT, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_COLUMN );
    if ( m_pRowItem == NULL || m_pColItem == NULL )
    {
        LOG_MSG(m_Logger, LogTypes::Warning, QString("Failed to find row or column item in character report") );
	return false;
    }
    m_pCharacterReportCol = m_pColItem->CollectionPath;
    m_nCharacterReportId = m_pColItem->ReportID;

    // Find the index to the first data item
    m_nCharacterDataIndex = 0;
    for ( ; m_nCharacterDataIndex < (int)m_pCharacterReportCol->ReportItems.size(); m_nCharacterDataIndex++ )
	if ( m_pCharacterReportCol->ReportItems[m_nCharacterDataIndex]->Attributes.Usage == USAGE_DISPLAY_DATA )
	    break;

    if ( m_nCharacterDataIndex == (int)m_pCharacterReportCol->ReportItems.size() )
    {
        LOG_MSG(m_Logger, LogTypes::Warning, QString("Failed to find display data item in character display report") );
	return false;
    }

    HID_ReportDetails_t pReportDetails = m_pDevice->ReportInfo().Reports[m_nCharacterReportId];
    int nBufLen = pReportDetails.OutReportLength;
    m_nCharacterReportOffset = 0;
    if ( m_pDevice->ReportInfo().Reports.size() > 1 )
	m_nCharacterReportOffset=1;

    m_bufLCDWrite.resize(nBufLen+m_nCharacterReportOffset);

    return true;
}




bool HIDLCDDevice::LCDWrite( int nRow, int nCol, QString sText )
{
    if ( m_pRowItem == NULL || m_pColItem == NULL )
    {
        LOG_MSG(m_Logger, LogTypes::Error, "LCD not successfully initialised" );
	return false;
    }
    if ( nRow < 0 || nRow >= m_nRows )
    {
        LOG_MSG(m_Logger, LogTypes::Error, QString("Invalid row (%1<=%2<%3)").arg(0).arg(nRow).arg(m_nRows) );
	return false;
    }
    if ( nCol < 0 || nCol >= m_nCols )
    {
        LOG_MSG(m_Logger, LogTypes::Error, QString("Invalid column (%1<=%2<%3)").arg(0).arg(nCol).arg(m_nCols) );
	return false;
    }
    if ( sText.length() > m_nCols - nCol )
        LOG_MSG(m_Logger, LogTypes::Warning, QString("Output string truncated. '%1' at column %2 is too long %3 > %4").arg(sText).arg(nCol).arg(sText.length()).arg(m_nCols-nCol) );
    
    m_pRowItem->Value = nRow;
    m_pColItem->Value = nCol;
    for ( int i = 0; i < sText.length() && m_nCharacterDataIndex+i < (int)m_nCols && m_nCharacterDataIndex+i < (int)m_pCharacterReportCol->ReportItems.size(); i++ )
	m_pCharacterReportCol->ReportItems[m_nCharacterDataIndex + i]->Value = sText[i].toAscii();
    if ( sText.length() < m_nCols )
	m_pCharacterReportCol->ReportItems[m_nCharacterDataIndex + sText.length()]->Value = 0;

    if ( m_nCharacterReportOffset )
	m_bufLCDWrite[0] = m_nCharacterReportId;

    HIDParser parser;
    parser.MakeOutputReport( m_bufLCDWrite.data() + m_nCharacterReportOffset, (byte)(m_bufLCDWrite.count() - m_nCharacterReportOffset), m_pDevice->ReportInfo().ReportItems, m_nCharacterReportId );

    // Send the report
    int nRet = m_pDevice->InterruptWrite( m_bufLCDWrite.data(), m_bufLCDWrite.count(), USB_TIMEOUT );
    LOG_MSG( m_Logger, LogTypes::Debug, QString("interrupt write returned %1\n").arg(nRet) );

    if ( nRet != m_bufLCDWrite.count() )
    {
	LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to send text data packet.  Interrupt write returned %1, expected %2\n").arg(nRet).arg(m_bufLCDWrite.count()) );
	return false;
    }
    return true;
}




bool HIDLCDDevice::LCDSendUserFont( byte nCharIndex, const QVector<byte> &data )
{
    HID_ReportItem_t *pCharIndex = m_pDevice->ReportInfo().FindReportItem( m_pLCDReportCol, REPORT_ITEM_TYPE_Out, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_FONT_REPORT, USAGEPAGE_ALPHANUMERIC_DISPLAY, USAGE_DISPLAY_DATA );
    if ( pCharIndex == NULL )
    {
	LOG_MSG( m_Logger, LogTypes::Error, "Failed to find USAGE_DISPLAY_DATA in font report" );
	return false;
    }
    HID_CollectionPath_t *pCollection = pCharIndex->CollectionPath;
    byte nReportId = pCharIndex->ReportID;

    // Find the index to the first data item
    unsigned int nIndex = 0;
    for ( ; nIndex < pCollection->ReportItems.size(); nIndex++ )
	if ( pCollection->ReportItems[nIndex]->Attributes.Usage == USAGE_FONT_DATA )
	    break;

    if ( nIndex == pCollection->ReportItems.size() )
    {
	LOG_MSG( m_Logger, LogTypes::Error, "Failed to find USAGE_FONT_DATA in font report" );
	return false;
    }

    pCharIndex->Value = nCharIndex;
    for ( int i = 0; i < data.count() && nIndex + i < pCollection->ReportItems.size(); i++ )
	pCollection->ReportItems[nIndex + i]->Value = data[i];

    HID_ReportDetails_t pReportDetails = m_pDevice->ReportInfo().Reports[nReportId];
    int nBufLen = pReportDetails.OutReportLength;
    int nOffset = 0;
    if ( m_pDevice->ReportInfo().Reports.size() > 1 )
	nOffset=1;

    QVarLengthArray<byte> buf(nBufLen+nOffset);
    if ( nOffset )
	buf[0] = nReportId;

    HIDParser parser;
    parser.MakeOutputReport( buf.data() + nOffset, (byte)(buf.count() - nOffset), m_pDevice->ReportInfo().ReportItems, nReportId );

    // Send the report
    int nRet = m_pDevice->InterruptWrite( buf.data(), buf.count() + nOffset, USB_TIMEOUT );
    LOG_MSG( m_Logger, LogTypes::Debug, QString("interrupt write returned %1\n").arg(nRet) );

    if ( nRet != buf.count() )
    {
	LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to send font data packet.  Interrupt write returned %1, expected %2\n").arg(nRet).arg(buf.count()) );
	return false;
    }
    return true;
}



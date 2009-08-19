#include "stdafx.h"
#include "hiddatathread.h"

#ifdef _WIN32
    #define TIMEOUT_ERROR	-116
#else
    #define TIMEOUT_ERROR	-110
#endif



HIDDataThread::HIDDataThread( HIDDevice *pDevice )
: QThread()
, m_bRunning( false )
, m_pDevice( pDevice )
{

}

HIDDataThread::~HIDDataThread()
{
    m_bRunning = false;
    wait(LOOP_TIMEOUT*2);
}


void HIDDataThread::stop()
{
    m_bRunning = false;
}

void HIDDataThread::run()
{
    if ( !m_pDevice->Open() )
	return;

    if ( !m_pDevice->Claim() )
    {
	m_pDevice->Close();
	return;
    }

    // Find the largest report
    int nLongestReport = 0;
    int nMultiReport = 0;
    std::map<byte, HID_ReportDetails_t>::iterator it;
    for ( it = m_pDevice->ReportInfo().Reports.begin(); it != m_pDevice->ReportInfo().Reports.end(); it++ )
    {
	if ( it->second.InReportLength > nLongestReport )
	    nLongestReport = it->second.InReportLength;
    }

    // When there is more than one report, the report id is inserted before the packet.
    if ( m_pDevice->ReportInfo().Reports.size() > 1 )
    {
	nLongestReport++;
	nMultiReport = 1;
    }

    // Request an initial copy of the report(s)
    // TODO:

    QVector<byte> buf( nLongestReport, 0 );
    m_bRunning = true;
    std::map<byte, HID_ReportDetails_t>::iterator itReport = m_pDevice->ReportInfo().Reports.begin();
    bool bInitialising = true;
    while ( m_bRunning )
    {
	int nRead = 0;
	memset( buf.data(), 0, buf.size() );

	if ( bInitialising )
	{
	    while ( itReport != m_pDevice->ReportInfo().Reports.end() && (itReport->second).InReportLength == 0 )
		itReport++;

	    if ( itReport == m_pDevice->ReportInfo().Reports.end() )
	    {
		bInitialising = false;
		continue;
	    }

	    bool bGotData = m_pDevice->GetReport( (itReport->second).ReportId, REPORT_ITEM_TYPE_In, buf.data(), (byte)((itReport->second).InReportLength + nMultiReport) );
	    if ( bGotData )
		nRead = (itReport->second).InReportLength + nMultiReport;
	    else
		nRead = -1;
	    itReport++;
	}
	else
	{
	    nRead = m_pDevice->InterruptRead( buf.data(), nLongestReport, LOOP_TIMEOUT );
	}

	if ( nRead == TIMEOUT_ERROR )
        {
            // Nothing to do.
        }
        else if ( nRead < 0 )
	{
	}
	else if ( nRead > 0 )
	{
	    {
		//QWriteLocker lock(&m_queueLock);
		//m_dataQueue.append( buf.mid(0,n) );
	    }
	    emit newData( buf.mid(0,nRead) );

	    //char s[1280];
	    //*s=0;
	    //for ( int i = 0; i < n; i++ )
	    //{
	    //	char s1[10];
	    //	_snprintf( s1, sizeof(s1), "%02X ", buf.data()[i] );
	    //	strcat( s, s1 );
	    //}
	    //LOG_TRACE("%s\n", s);
	}

	//bRun = false;
    }

    m_pDevice->Unclaim();
    m_pDevice->Close();
}

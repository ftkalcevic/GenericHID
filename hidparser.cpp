// hidcomp/hidconfig, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

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


// Most of this code from...


/*
LUFA Library
Copyright (C) Dean Camera, 2009.

dean [at] fourwalledcubicle [dot] com
www.fourwalledcubicle.com
*/

/*
Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)

Permission to use, copy, modify, and distribute this software
and its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of the author not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

The author disclaim all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall the author be liable for any
special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
*/


#include "common.h"
#include "hidparser.h"
#include "log.h"
#include <deque>
#include <stack>
#include <assert.h>

#include <QString>


#ifdef _WIN32
#define TRACE_DESC(s, ...)		{ if ( m_Logger.WillLog(LogTypes::Debug) ) _snprintf_s( sDescriptor, sizeof(sDescriptor), s, __VA_ARGS__); }
#else
#define TRACE_DESC(s, a...)		{ if ( m_Logger.WillLog(LogTypes::Debug) ) snprintf( sDescriptor, sizeof(sDescriptor), s, ##a); }
#endif


// This is basically a std::queue, but with the ability to clear it.
template<class T> class queue: std::deque<T>
{
public:
    T pop() { T tmp = std::deque<T>::front();  std::deque<T>::pop_front(); return tmp; }
    void push( T n ) { std::deque<T>::push_back( n ); }
    bool empty() { return std::deque<T>::empty(); }
    void clear() { std::deque<T>::clear(); }
};

/* Macros: */
#define DATA_SIZE_MASK           	0x03		// 0b00000011
#define DATA_SIZE_OFFSET			0
#define TYPE_MASK                	0x0C		// 0b00001100
#define TYPE_OFFSET					2
#define TAG_MASK                 	0xF0		// 0b11110000
#define TAG_OFFSET					4

#define DATA_SIZE_0              	0x00		// 0b00000000
#define DATA_SIZE_1              	0x01		// 0b00000001
#define DATA_SIZE_2              	0x02		// 0b00000010
#define DATA_SIZE_4              	0x03		// 0b00000011

#define TYPE_MAIN                	(0x00<<TYPE_OFFSET)		// 0b00000000
#define TYPE_GLOBAL              	(0x01<<TYPE_OFFSET)		// 0b00000100
#define TYPE_LOCAL               	(0x02<<TYPE_OFFSET)		// 0b00001000

#define TAG_MAIN_INPUT           	0x80		// 0b10000000
#define TAG_MAIN_OUTPUT          	0x90		// 0b10010000
#define TAG_MAIN_COLLECTION      	0xA0		// 0b10100000
#define TAG_MAIN_FEATURE         	0xB0		// 0b10110000
#define TAG_MAIN_ENDCOLLECTION   	0xC0		// 0b11000000
#define TAG_GLOBAL_USAGEPAGE     	0x00		// 0b00000000
#define TAG_GLOBAL_LOGICALMIN    	0x10		// 0b00010000
#define TAG_GLOBAL_LOGICALMAX    	0x20		// 0b00100000
#define TAG_GLOBAL_PHYSMIN       	0x30		// 0b00110000
#define TAG_GLOBAL_PHYSMAX       	0x40		// 0b01000000
#define TAG_GLOBAL_UNITEXP       	0x50		// 0b01010000
#define TAG_GLOBAL_UNIT          	0x60		// 0b01100000
#define TAG_GLOBAL_REPORTSIZE    	0x70		// 0b01110000
#define TAG_GLOBAL_REPORTID      	0x80		// 0b10000000
#define TAG_GLOBAL_REPORTCOUNT   	0x90		// 0b10010000
#define TAG_GLOBAL_PUSH          	0xA0		// 0b10100000
#define TAG_GLOBAL_POP           	0xB0		// 0b10110000
#define TAG_LOCAL_USAGE          	0x00		// 0b00000000
#define TAG_LOCAL_USAGEMIN       	0x10		// 0b00010000
#define TAG_LOCAL_USAGEMAX       	0x20		// 0b00100000
#define TAG_LOCAL_DESIGNATOR_INDEX	0x30		// 0b0011 10 nn
#define TAG_LOCAL_DESIGNATOR_MIN 	0x40		// 0b0100 10 nn
#define TAG_LOCAL_DESIGNATOR_MAX	0x50		// 0b0101 10 nn
#define TAG_LOCAL_STRING_INDEX 		0x70		// 0b0111 10 nn
#define TAG_LOCAL_STRING_MIN 		0x80		// 0b1000 10 nn
#define TAG_LOCAL_STRING_MAX 		0x90		// 0b1001 10 nn
#define TAG_LOCAL_DELIMITER 		0xA0		// 0b1010 10 nn


/* Type Defines: */
struct HID_StateTable_t
{
    uint8_t                     ReportCount;
    uint8_t                     ReportID;
    uint8_t                     BitSize;  /**< Size in bits of the report item's data. */

    bool						IsUsageRange;
    uint16_t					Usage;
    uint16_t					UsagePage;
    uint16_t					UsageMinimum;
    uint16_t					UsageMaximum;
    uint8_t                     UnitType;
    int8_t                      UnitExponent;
    int32_t                     LogicalMinimum;
    int32_t                     LogicalMaximum;
    int32_t                     PhysicalMinimum;
    int32_t                     PhysicalMaximum;
    bool						IsStringRange;
    uint8_t						StringIndex;
    uint8_t						StringMinimum;
    uint8_t						StringMaximum;
    bool						IsDesignatorRange;
    uint8_t						DesignatorIndex;
    uint8_t						DesignatorMinimum;
    uint8_t						DesignatorMaximum;
};

struct HID_OffsetTable_t
{
    uint16_t BitOffsetIn;
    uint16_t BitOffsetOut;
    uint16_t BitOffsetFeature;
};

HIDParser::HIDParser( const QString &sModule, const QString &sComponent )
: m_Logger( sModule, sComponent )
{
}

HIDParser::~HIDParser(void)
{
}


static uint32_t UnsignedData( const uint8_t *ReportData )
{
    uint32_t ReportItemData = 0;

    switch (*ReportData & DATA_SIZE_MASK)
    {
    case DATA_SIZE_4:
	ReportItemData = *((uint32_t*)(ReportData + 1));
	break;
    case DATA_SIZE_2:
	ReportItemData = *((uint16_t*)(ReportData + 1));
	break;
    default:
	assert( false );
    case DATA_SIZE_1:
	ReportItemData = *((uint8_t*)(ReportData + 1));
	break;
    }
    return ReportItemData;
}


static int32_t SignedData( const uint8_t *ReportData )
{
    int32_t ReportItemData = 0;

    switch (*ReportData & DATA_SIZE_MASK)
    {
    case DATA_SIZE_4:
	ReportItemData = *((int32_t*)(ReportData + 1));
	break;
    case DATA_SIZE_2:
	ReportItemData = *((int16_t*)(ReportData + 1));
	break;
    default:
	assert( false );
    case DATA_SIZE_1:
	ReportItemData = *((int8_t*)(ReportData + 1));
	break;
    }
    return ReportItemData;
}


int HIDParser::ParseReportData( const uint8_t* ReportData, uint16_t ReportSize, HID_ReportInfo_t &ParserData )
{
    if ( m_Logger.WillLog(LogTypes::Debug) )
    {
        QString s;
        for ( int i = 0, j = 0; i < ReportSize; i++, j++ )
        {
	    if ( j == 16 )
	    {
                LOG_MSG( m_Logger, LogTypes::Debug, s );
                s.clear();
	        j = 0;
	    }
	    s += QString("%1 ").arg( *(ReportData+i), 2, 16, QChar('0') );
        }
        if ( !s.isEmpty() )
            LOG_MSG( m_Logger, LogTypes::Debug, s );
    }

    std::stack<HID_StateTable_t> StateTable;
    StateTable.push( HID_StateTable_t() );
    HID_StateTable_t &CurrStateTable = StateTable.top();
    queue<uint16_t> UsageStack;

    std::map<byte,HID_OffsetTable_t> ReportOffsets;
    uint16_t LastUsage = 0;

    HID_CollectionPath_t* CurrCollectionPath = NULL;

    memset(&CurrStateTable, 0x00, sizeof(HID_StateTable_t));

    int nIndent = 0;
    while (ReportSize)
    {
	char sDescriptor[MAX_DESCRIPTOR_SIZE+1];
	sDescriptor[0] = 0;

	switch (*ReportData & (TYPE_MASK | TAG_MASK))
	{
	case (TYPE_GLOBAL | TAG_GLOBAL_PUSH):
	    TRACE_DESC("PUSH");
	    StateTable.push( StateTable.top() );
	    CurrStateTable = StateTable.top();
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_POP):
	    TRACE_DESC("POP");
	    if (StateTable.empty())
		return HID_PARSE_HIDStackUnderflow;
	    StateTable.pop();
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_USAGEPAGE):
	    TRACE_DESC("USAGE_PAGE (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.UsagePage = (uint16_t)UnsignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_LOGICALMIN):
	    TRACE_DESC("LOGICAL_MINIMUM (%d)", SignedData( ReportData ) );
	    CurrStateTable.LogicalMinimum = SignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_LOGICALMAX):
	    TRACE_DESC("LOGICAL_MAXIMUM (%d)", SignedData( ReportData ) );
	    CurrStateTable.LogicalMaximum = SignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_PHYSMIN):
	    TRACE_DESC("PHYSICAL_MINIMUM (%d)", SignedData( ReportData ) );
	    CurrStateTable.PhysicalMinimum = SignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_PHYSMAX):
	    TRACE_DESC("PHYSICAL_MAXIMUM (%d)", SignedData( ReportData ) );
	    CurrStateTable.PhysicalMaximum = SignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_UNITEXP):
	    TRACE_DESC("UNIT_EXPONENT (%d)", SignedData( ReportData ) );
	    CurrStateTable.UnitExponent    = (int8_t)SignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_UNIT):
	    TRACE_DESC("UNIT (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.UnitType        = (uint8_t)UnsignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_REPORTSIZE):
	    TRACE_DESC("REPORT_SIZE (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.BitSize = (uint8_t)UnsignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_REPORTCOUNT):
	    TRACE_DESC("REPORT_COUNT (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.ReportCount = (uint8_t)UnsignedData( ReportData );
	    break;

	case (TYPE_GLOBAL | TAG_GLOBAL_REPORTID):
	    TRACE_DESC("REPORT_ID (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.ReportID = (uint8_t)UnsignedData( ReportData );
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_USAGE):
	    TRACE_DESC("USAGE (%d)", UnsignedData( ReportData ) );
	    UsageStack.push((uint16_t)UnsignedData( ReportData ));
	    CurrStateTable.IsUsageRange = false;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_USAGEMIN):
	    TRACE_DESC("USAGE_MINIMUM (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.UsageMinimum = (uint16_t)UnsignedData( ReportData );
	    CurrStateTable.IsUsageRange = true;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_USAGEMAX):
	    TRACE_DESC("USAGE_MAXIMUM (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.UsageMaximum = (uint16_t)UnsignedData( ReportData );
	    CurrStateTable.IsUsageRange = true;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_STRING_INDEX):
	    TRACE_DESC("STRING_INDEX (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.StringIndex = (uint8_t)UnsignedData( ReportData );
	    CurrStateTable.IsStringRange = false;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_STRING_MIN):
	    TRACE_DESC("STRING_MINIMUM (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.StringMinimum = (uint8_t)UnsignedData( ReportData );
	    CurrStateTable.IsStringRange = true;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_STRING_MAX):
	    TRACE_DESC("STRING_MAXIMUM (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.StringMaximum = (uint8_t)UnsignedData( ReportData );
	    CurrStateTable.IsStringRange = true;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_DESIGNATOR_INDEX):
	    TRACE_DESC("DESIGNATOR_INDEX (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.DesignatorIndex = (uint8_t)UnsignedData( ReportData );
	    CurrStateTable.IsDesignatorRange = false;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_DESIGNATOR_MIN):
	    TRACE_DESC("DESIGNATOR_MINIMUM (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.DesignatorMinimum = (uint8_t)UnsignedData( ReportData );
	    CurrStateTable.IsDesignatorRange = true;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_DESIGNATOR_MAX):
	    TRACE_DESC("DESIGNATOR_MAXIMUM (%d)", UnsignedData( ReportData ) );
	    CurrStateTable.DesignatorMaximum = (uint8_t)UnsignedData( ReportData );
	    CurrStateTable.IsDesignatorRange = true;
	    break;

	case (TYPE_LOCAL | TAG_LOCAL_DELIMITER):
	    TRACE_DESC("DELIMITER (%d)", UnsignedData( ReportData ) );
	    assert(false);	// Not Supported
	    break;

	case (TYPE_MAIN | TAG_MAIN_COLLECTION):
	    {
		TRACE_DESC("COLLECTION (%d)", UnsignedData( ReportData ) );
		nIndent++;

		HID_CollectionPath_t *newPath = new HID_CollectionPath_t;
		ParserData.Collections.push_back( newPath );
		newPath->Parent = CurrCollectionPath;
		CurrCollectionPath = newPath;

		CurrCollectionPath->Type = (uint8_t)UnsignedData( ReportData );
		CurrCollectionPath->UsagePage = CurrStateTable.UsagePage;
		CurrCollectionPath->StringIndex = CurrStateTable.StringIndex;
		CurrStateTable.StringIndex = 0;

		if (!UsageStack.empty())
		    LastUsage = CurrCollectionPath->Usage = UsageStack.pop();
		else
		    CurrCollectionPath->Usage = LastUsage;
		break;
	    }

	case (TYPE_MAIN | TAG_MAIN_ENDCOLLECTION):
	    TRACE_DESC("END_COLLECTION" );
	    nIndent--;

	    if (CurrCollectionPath == NULL)
		return HID_PARSE_UnexpectedEndCollection;
	    CurrCollectionPath = CurrCollectionPath->Parent;
	    break;
	case (TYPE_MAIN | TAG_MAIN_INPUT):
	case (TYPE_MAIN | TAG_MAIN_OUTPUT):
	case (TYPE_MAIN | TAG_MAIN_FEATURE):
	    for (uint8_t ReportItemNum = 0; ReportItemNum < CurrStateTable.ReportCount; ReportItemNum++)
	    {
		HID_ReportItem_t* CurrReportItem = new HID_ReportItem_t();
		ParserData.ReportItems.push_back( CurrReportItem );

		CurrReportItem->ItemFlags      = (uint8_t)UnsignedData( ReportData );
		CurrReportItem->ReportID       = CurrStateTable.ReportID;
		CurrReportItem->BitSize        = CurrStateTable.BitSize;
		CurrReportItem->Attributes.LogicalMinimum = CurrStateTable.LogicalMinimum;
		CurrReportItem->Attributes.LogicalMaximum = CurrStateTable.LogicalMaximum;
		CurrReportItem->Attributes.PhysicalMinimum = CurrStateTable.PhysicalMinimum;
		CurrReportItem->Attributes.PhysicalMaximum = CurrStateTable.PhysicalMaximum;
		CurrReportItem->Attributes.UnitExponent = CurrStateTable.UnitExponent;
		CurrReportItem->Attributes.UnitType = CurrStateTable.UnitType;
		CurrReportItem->Attributes.UsagePage = CurrStateTable.UsagePage;
                CurrReportItem->CollectionPath = CurrCollectionPath;
                CurrReportItem->Value = 0;
                if ( CurrCollectionPath != NULL )
                    CurrCollectionPath->ReportItems.push_back( CurrReportItem );


		if ( CurrStateTable.IsUsageRange )
		{
		    ushort nUsage = CurrStateTable.UsageMinimum + ReportItemNum;
		    if ( nUsage > CurrStateTable.UsageMaximum )
			nUsage = CurrStateTable.UsageMaximum;
		    CurrReportItem->Attributes.Usage = nUsage;
		}
		else
		{
		    if (!UsageStack.empty())
			LastUsage = CurrReportItem->Attributes.Usage = UsageStack.pop();
		    else
			CurrReportItem->Attributes.Usage = LastUsage;
		}

		if ( CurrStateTable.IsStringRange )
		{
		    uint8_t nIndex = CurrStateTable.StringMinimum + ReportItemNum;
		    if ( nIndex > CurrStateTable.StringMaximum )
			nIndex = CurrStateTable.StringMaximum;
		    CurrReportItem->Attributes.StringIndex = nIndex;
		}
		else
		{
		    CurrReportItem->Attributes.StringIndex = CurrStateTable.StringIndex;
		    CurrStateTable.StringIndex = 0;
		}

		if ( CurrStateTable.IsDesignatorRange )
		{
		    uint8_t nIndex = CurrStateTable.DesignatorMinimum + ReportItemNum;
		    if ( nIndex > CurrStateTable.DesignatorMaximum )
			nIndex = CurrStateTable.DesignatorMaximum;
		    CurrReportItem->Attributes.DesignatorIndex = nIndex;
		}
		else
		{
		    CurrReportItem->Attributes.DesignatorIndex = CurrStateTable.DesignatorIndex;
		    CurrStateTable.DesignatorIndex = 0;
		}


		std::map<byte,HID_OffsetTable_t>::iterator it = ReportOffsets.find( CurrReportItem->ReportID );
		if ( it == ReportOffsets.end() )
		{
		    ReportOffsets[CurrReportItem->ReportID] = HID_OffsetTable_t();
		    it = ReportOffsets.find( CurrReportItem->ReportID );
		}

		HID_OffsetTable_t &offsets = (it->second);
		switch (*ReportData & TAG_MASK)
		{
		case TAG_MAIN_INPUT:
		    CurrReportItem->ItemType  = REPORT_ITEM_TYPE_In;
		    CurrReportItem->BitOffset = offsets.BitOffsetIn;
		    offsets.BitOffsetIn = offsets.BitOffsetIn + CurrStateTable.BitSize;
		    TRACE_DESC("INPUT (%d)", UnsignedData( ReportData ) );
		    break;

		case TAG_MAIN_OUTPUT:
		    CurrReportItem->ItemType  = REPORT_ITEM_TYPE_Out;
		    CurrReportItem->BitOffset = offsets.BitOffsetOut;
		    offsets.BitOffsetOut = offsets.BitOffsetOut + CurrStateTable.BitSize;
		    TRACE_DESC("OUTPUT (%d)", UnsignedData( ReportData ) );
		    break;

		case TAG_MAIN_FEATURE:
		    CurrReportItem->ItemType  = REPORT_ITEM_TYPE_Feature;
		    CurrReportItem->BitOffset = offsets.BitOffsetFeature;
		    offsets.BitOffsetFeature = offsets.BitOffsetFeature + CurrStateTable.BitSize;
		    TRACE_DESC("FEATURE (%d)", UnsignedData( ReportData ) );
		    break;
		}
	    }

	    UsageStack.clear();
	    LastUsage = 0;

	    break;
	}

	if ((*ReportData & TYPE_MASK) == TYPE_MAIN)
	    UsageStack.clear();

        if ( m_Logger.WillLog(LogTypes::Debug) )
        {
            QString s = QString("%1 %2").arg(QString(nIndent*2, QChar(' '))).arg(sDescriptor,-(40 - nIndent*2));
	    int nDataLen = *ReportData & DATA_SIZE_MASK;
	    for ( int i = 0; i < nDataLen + 1; i++ )
	    {
                s += QString("%1 ").arg(*(ReportData + i),2,16,QChar('0'));
	    }
            LOG_MSG( m_Logger, LogTypes::Debug, s );
	}

	switch (*ReportData & DATA_SIZE_MASK)
	{
	case DATA_SIZE_4:
	    ReportSize -= 5;
	    ReportData += 5;
	    break;
	case DATA_SIZE_2:
	    ReportSize -= 3;
	    ReportData += 3;
	    break;
	case DATA_SIZE_1:
	    ReportSize -= 2;
	    ReportData += 2;
	    break;
	case DATA_SIZE_0:
	    ReportSize -= 1;
	    ReportData += 1;
	    break;
	}
    }

#define REPORT_LEN(n)       ((n)%8==0?(n)/8:(n)/8+1)
    std::map<byte,HID_OffsetTable_t>::iterator it;
    for ( it = ReportOffsets.begin(); it != ReportOffsets.end(); it++ )
    {
	byte nId = it->first;

	ParserData.Reports[nId] = HID_ReportDetails_t( nId, REPORT_LEN(it->second.BitOffsetIn), REPORT_LEN(it->second.BitOffsetOut), REPORT_LEN(it->second.BitOffsetFeature) );
    }

    if ( m_Logger.WillLog(LogTypes::Debug) )
    {
        for ( int i = 0; i < (int)ParserData.ReportItems.size(); i++ )
        {
	    HID_ReportItem_t *p = ParserData.ReportItems[i];
            LOG_MSG( m_Logger, LogTypes::Debug, QString("%1: ReportId:%2  Type:%3 Offset:%4 Bits:%5\n").arg(i,2).arg(p->ReportID,2).arg(p->ItemType == REPORT_ITEM_TYPE_In ? "In" : (p->ItemType == REPORT_ITEM_TYPE_Out ? "Out" : "Feature")).arg(p->BitOffset).arg(p->BitSize) );
        }
    }

    return HID_PARSE_Successful;
}



// Extract each bit out of the report and store it in the value field for each report item.
// The report items are in order.
bool HIDParser::DecodeReport( byte *pReport, byte nReportLen, std::vector<HID_ReportItem_t *> &ReportItems, byte nReportId, HID_ReportItemTypes_t ReportType )
{
    QString sDebug;
    std::vector<HID_ReportItem_t *>::iterator it;

    byte *pData = pReport;
    byte ReportByte = *pData;
    byte nBitsLeft = 8;
    byte nDataOffset = 0;

    pData++;
    for ( it = ReportItems.begin(); it != ReportItems.end(); it++ )
    {
	HID_ReportItem_t *pItem = *it;
	if ( pItem->ReportID == nReportId && pItem->ItemType == ReportType )
	{
	    byte nLen = pItem->BitSize;

	    int data = 0;

	    while ( nLen != 0 )
	    {
		if ( nLen <= nBitsLeft )
		{
		    data |= (ReportByte & ((1 << nLen)-1)) << nDataOffset;
		    ReportByte >>= nLen;
		    nBitsLeft = nBitsLeft - nLen;
		    nLen = 0;
		    nDataOffset = 0;
		}
		else
		{
		    data |= (ReportByte & ((1 << nBitsLeft)-1)) << nDataOffset;
		    nLen = nLen - nBitsLeft;
		    nDataOffset = nDataOffset + nBitsLeft;

		    if ( pData >= pReport + nReportLen )
			return false;

		    ReportByte = *pData;
		    pData++;
		    nBitsLeft = 8;
		}
	    }

	    if ( pItem->Attributes.LogicalMinimum < 0 )
	    {
		// sign extend the data
		if ( data & (1<<(pItem->BitSize-1)) )
		    data |= 0xFFFFFFFF << pItem->BitSize;
	    }
	    pItem->Value = data;

            if ( m_Logger.WillLog( LogTypes::Debug ) )
            {
                sDebug += QString::number( data );
                sDebug += " ";
            }
	}
    }

    LOG_MSG(m_Logger, LogTypes::Debug, sDebug.toLatin1().constData() );
    return true;
}


bool HIDParser::MakeOutputReport( byte *pReport, byte nReportLen, std::vector<HID_ReportItem_t *> &ReportItems, byte nReportId )
{
    byte nBitsLeft = 8;
    byte *pData = pReport;
    *pData = 0;

    std::vector<HID_ReportItem_t *>::iterator it;
    for ( it = ReportItems.begin(); it != ReportItems.end(); it++ )
    {
	HID_ReportItem_t *pItem = *it;
	if ( pItem->ReportID == nReportId && pItem->ItemType == REPORT_ITEM_TYPE_Out )
	{
	    byte nLen = pItem->BitSize;
	    int nData = pItem->Value;

	    while ( nLen > 0 )
	    {
		if ( nLen <= nBitsLeft )
		{
		    int nTemp = nData;
		    nTemp &= (1 << (nLen+1))-1;
		    nTemp <<= (8-nBitsLeft);
		    *pData |= nTemp;

		    nBitsLeft = nBitsLeft - nLen;
		    nLen = 0;
		}
		else
		{
		    int nTemp = nData;
		    nTemp &= (1 << (nBitsLeft+1)) - 1;
		    nTemp <<= (8-nBitsLeft);
		    *pData |= nTemp;

		    nData >>= nBitsLeft;
		    nLen = nLen - nBitsLeft;

		    pData++;
		    if ( pData >= pReport + nReportLen )
			return false;

		    *pData = 0;
		    nBitsLeft = 8;
		}
	    }
	}
    }
    return true;
}


HID_ReportInfo_t::~HID_ReportInfo_t()
{
    for ( unsigned int i = 0; i < ReportItems.size(); i++ )
	delete ReportItems[i];
    for ( unsigned int i = 0; i < Collections.size(); i++ )
	delete Collections[i];
}


// Search a collection for a specific report
HID_ReportItem_t *HID_ReportInfo_t::FindReportItem( HID_CollectionPath_t *pCol, HID_ReportItemTypes_t type, 
						    unsigned short nCollectionUsagePage, unsigned short nCollectionUsage, 
						    unsigned short nUsagePage, unsigned short nUsage )
{
    // Check collection
    if ( pCol->UsagePage == nCollectionUsagePage && pCol->Usage == nCollectionUsage )
    {
	for ( unsigned int i = 0; i < pCol->ReportItems.size(); i++ )
	    if ( pCol->ReportItems[i]->ItemType == type && pCol->ReportItems[i]->Attributes.UsagePage == nUsagePage && pCol->ReportItems[i]->Attributes.Usage == nUsage )
		return pCol->ReportItems[i];
    }

    // Look for sub collections
    for ( unsigned int i = 0; i < Collections.size(); i++ )
    {
	HID_CollectionPath_t *pSubCol = Collections[i];
	HID_CollectionPath_t *pTempCol = pSubCol;
	while ( pTempCol != NULL && pTempCol->Parent != pCol )
	    pTempCol = pTempCol->Parent;
	if ( pTempCol != NULL && pSubCol->UsagePage == nCollectionUsagePage && pSubCol->Usage == nCollectionUsage )
	{
	    for ( unsigned int i = 0; i < pSubCol->ReportItems.size(); i++ )
		if ( pSubCol->ReportItems[i]->ItemType == type && pSubCol->ReportItems[i]->Attributes.UsagePage == nUsagePage && pSubCol->ReportItems[i]->Attributes.Usage == nUsage )
		    return pSubCol->ReportItems[i];
	}
    }
    return NULL;
}

void HID_ReportInfo_t::clear()
{
    ReportItems.clear();
    Reports.clear();
    Collections.clear();
}

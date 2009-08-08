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


#ifndef _HIDPARSER_H_
#define _HIDPARSER_H_

#include <QString>
#include <QCoreApplication>
#include <vector>
#include <map>

#include "types.h"
#include "log.h"

#define MAX_DESCRIPTOR_SIZE     128

#define IOF_CONSTANT             (1 << 0)		/** HID_ReportItem_t.ItemFlags flag for constant data. */
#define IOF_DATA                 (0 << 0)		/** HID_ReportItem_t.ItemFlags flag for data. */
#define IOF_VARIABLE             (1 << 1)		/** HID_ReportItem_t.ItemFlags flag for variable data. */
#define IOF_ARRAY                (0 << 1)		/** HID_ReportItem_t.ItemFlags flag for array data. */
#define IOF_RELATIVE             (1 << 2)		/** HID_ReportItem_t.ItemFlags flag for relative data. */
#define IOF_ABSOLUTE             (0 << 2)		/** HID_ReportItem_t.ItemFlags flag for absolute data. */
#define IOF_WRAP                 (1 << 3)		/** HID_ReportItem_t.ItemFlags flag for wrapped value data. */
#define IOF_NOWRAP               (0 << 3)		/** HID_ReportItem_t.ItemFlags flag for non-wrapped value data. */
#define IOF_NONLINEAR            (1 << 4)		/** HID_ReportItem_t.ItemFlags flag for non linear data. */
#define IOF_LINEAR               (0 << 4)		/** HID_ReportItem_t.ItemFlags flag for linear data. */
#define IOF_NOPREFERRED          (1 << 5)		/** HID_ReportItem_t.ItemFlags flag for no preferred state. */
#define IOF_PREFERREDSTATE       (0 << 5)		/** HID_ReportItem_t.ItemFlags flag for preferred state items. */
#define IOF_NULLSTATE            (1 << 6)		/** HID_ReportItem_t.ItemFlags flag for null state items. */
#define IOF_NONULLPOSITION       (0 << 6)		/** HID_ReportItem_t.ItemFlags flag for no null position data. */
#define IOF_BUFFEREDBYTES        (1 << 8)		/** HID_ReportItem_t.ItemFlags flag for buffered bytes. */
#define IOF_BITFIELD             (0 << 8)		/** HID_ReportItem_t.ItemFlags flag for bitfield data. */



/** Enum for indicating what type of report item an entry in a HID_ReportInfo_t ReportItem array is */
enum HID_ReportItemTypes_t
{
    REPORT_ITEM_TYPE_In                   = 0, /**< Indicates that the item is an IN report type. */
    REPORT_ITEM_TYPE_Out                  = 1, /**< Indicates that the item is an OUT report type. */
    REPORT_ITEM_TYPE_Feature              = 2, /**< Indicates that the item is a FEATURE report type. */
};

/** Enum for the possible error codes in the return value of the ProcessHIDReport() function */
enum HID_Parse_ErrorCodes_t
{
    HID_PARSE_Successful                  = 0, /**< Successful parse of the HID report descriptor, no error. */
    HID_PARSE_HIDStackUnderflow           = 2, /**< A POP was found when the state table stack was empty. */
    HID_PARSE_UnexpectedEndCollection     = 4, /**< END COLLECTION found without matching COLLECTION item. */
    HID_PARSE_InsufficientCollectionPaths = 5, /**< More than HID_MAX_COLLECTIONS collections in the report. */
};


/** Type define for all the data attributes of a report item, except flags. */
struct HID_ReportItem_Attributes_t
{
    uint16_t				UsagePage;
    uint16_t				Usage;
    int8_t				UnitExponent;
    uint8_t				UnitType;
    int32_t				LogicalMinimum;
    int32_t				LogicalMaximum;
    int32_t				PhysicalMinimum;
    int32_t				PhysicalMaximum;
    uint8_t				StringIndex;
    uint8_t				DesignatorIndex;
} ;

/** Type define for a COLLECTION object. Contains the collection attributes and a reference to the
*  parent collection if any.
*/
struct HID_ReportItem_t;    // Forward declaration
typedef struct CollectionPath
{
    uint8_t                     Type;   /**< Collection type (for example "Generic Desktop"). */
    uint16_t			UsagePage;  /**< Collection usage. */
    uint16_t			Usage;  /**< Collection usage. */
    struct CollectionPath*      Parent; /**< Reference to parent collection, or NULL if root collection. */
    std::vector<HID_ReportItem_t *> ReportItems; /**< Collection of reports in this collection */
} HID_CollectionPath_t;

/** Type define for a report item (IN, OUT or FEATURE) attributes and other details. */
struct HID_ReportItem_t
{
    uint16_t                     BitOffset;      /**< Bit offset in the IN, OUT or FEATURE report of the item. */
    uint8_t                      BitSize;        /**< Size in bits of the report item's data. */
    HID_ReportItemTypes_t	 ItemType;       /**< Report item type, a value in HID_Types_t. */
    uint16_t                     ItemFlags;      /**< Item data flags, such as constant/variable, etc. */
    uint8_t                      ReportID;       /**< Report ID this item belongs to, or 0x00 if device has only one report */
    HID_ReportItem_Attributes_t  Attributes;     /**< Report item attributes. */
    int32_t                      Value;          /**< Current value of the report item. */
    HID_CollectionPath_t*        CollectionPath; /**< Collection path of the item. */
};

/** Type define for a complete processed HID report, including all report item data and collections. */
struct HID_ReportDetails_t
{
    byte ReportId;
    int InReportLength;
    int OutReportLength;
    int FeatureReportLength;

    HID_ReportDetails_t() : InReportLength(0), OutReportLength(0), FeatureReportLength(0) {}
    HID_ReportDetails_t( byte nId, int in, int out, int feature ) : ReportId(nId), InReportLength(in), OutReportLength(out), FeatureReportLength(feature) {}
};

/** Type define for a complete processed HID report, including all report item data and collections. */
class HID_ReportInfo_t
{
public:
    ~HID_ReportInfo_t();

    std::vector<HID_ReportItem_t *> ReportItems;
    std::map<byte, HID_ReportDetails_t> Reports;
    std::vector<HID_CollectionPath_t *> Collections;
} ;



class HIDParser
{
public:
    HIDParser( const QString &sModule = QCoreApplication::applicationName(), const QString &sComponent = "HIDParser" );
    ~HIDParser(void);

    int ParseReportData( const uint8_t* ReportData, uint16_t ReportSize, HID_ReportInfo_t &ParserData );
    bool DecodeReport( byte *pReport, byte nReportLen, std::vector<HID_ReportItem_t *> &ReportItems, byte nReportId, HID_ReportItemTypes_t ReportType );
    bool MakeOutputReport( byte *pReport, byte nReportLen, std::vector<HID_ReportItem_t *> &ReportItems, byte nReportId );
private:
    Logger m_Logger;
};

#endif

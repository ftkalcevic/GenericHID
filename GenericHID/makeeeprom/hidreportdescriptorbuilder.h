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

#ifndef _HIDREPORTDESCRIPTORBUILDER_H_
#define _HIDREPORTDESCRIPTORBUILDER_H_

#include "bytebuffer.h"

namespace EDataType
{
    enum EDataType
    {
        Data,
        Constant
    };
};
namespace EVarType
{
    enum EVarType
    {
        Array,
        Variable
    };
};
namespace ERelType
{
    enum ERelType
    {
        Absolute,
        Relative
    };
};
namespace EWrapType
{
    enum EWrapType
    {
        NoWrap,
        Wrap
    };
};
namespace ELinearType
{
    enum ELinearType
    {
        Linear,
        NonLinear
    };
};
namespace EPreferedType
{
    enum EPreferedType
    {
        PreferredState,
        NoPreferred
    };
};
namespace ENullPositionType
{
    enum ENullPositionType
    {
        NoNullPosition,
        NullState
    };
};
namespace EVolatileType
{
    enum EVolatileType
    {
        NonVolatile,
        Volatile
    };
};
namespace EBufferType
{
    enum EBufferType
    {
        BitField,
        Buffered
    };
};
namespace CollectionType
{
    enum CollectionType
    {
        Physical = 0,
        Application = 1,
        Logical = 2,
        Report = 3,
        NamedArray = 4,
        UsageSwitch = 5,
        UsageModifier = 6
    };
}

class HIDReportDescriptorBuilder: public ByteBuffer
{
public:
    HIDReportDescriptorBuilder(void);
    ~HIDReportDescriptorBuilder(void);

public:
    enum
    {
        TagInput = 0x80,
        TagOutput = 0x90,
        TagFeature = 0xB0,
        TagCollection = 0xA0,
        TagEndCollection = 0xC0,

        TagUsagePage = 0x04,
        TagLogicalMinimum = 0x14,
        TagLogicalMaximum = 0x24,
        TagPhysicalMinimum = 0x34,
        TagPhysicalMaximum = 0x44,
        TagUnitExponent = 0x54,
        TagUnit = 0x64,
        TagReportSize = 0x74,
        TagReportID = 0x84,
        TagReportCount = 0x94,
        TagPush = 0xA4,
        TagPop = 0xB4,

        TagUsage = 0x08,
        TagUsageMinimum = 0x18,
        TagUsageMaximum = 0x28,
        TagDesignatorIndex = 0x38,
        TagDesignatorMinimum = 0x48,
        TagDesignatorMaximum = 0x58,
        TagStringIndex = 0x78,
        TagStringMinimum = 0x88,
        TagStringMaximum = 0x98,
        TagDelimiter = 0xA8,
    };

private:
    void AddValue(byte tag);
    void AddValue(byte tag, int n);
    void AddValue(byte tag, uint n);
    void AddInputOutputFeature(byte tag, EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType,  EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EVolatileType::EVolatileType volType, EBufferType::EBufferType bufferType );
public:
    void UsagePage(uint n);
    void Usage(uint n);
    void UsageMin(uint n);
    void UsageMax(uint n);
    void Collection(CollectionType::CollectionType t);
    void EndCollection();
    void ReportID(uint n);
    void LogicalMinimum(int n);
    void LogicalMaximum(int n);
    void PhysicalMinimum(int n);
    void PhysicalMaximum(int n);
    void ReportSize(uint n);
    void ReportCount(uint n);
    void StringIndex(uint n);
    void StringMin(uint n);
    void StringMax(uint n);
    void Input(EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType,  EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EBufferType::EBufferType bufferType );
    void Output(EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType, EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EVolatileType::EVolatileType volType, EBufferType::EBufferType bufferType);
    void Feature(EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType, EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EVolatileType::EVolatileType volType, EBufferType::EBufferType bufferType);
};

#endif

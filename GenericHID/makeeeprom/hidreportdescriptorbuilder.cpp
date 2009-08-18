#include "stdafx.h"
#include "hidreportdescriptorbuilder.h"



HIDReportDescriptorBuilder::HIDReportDescriptorBuilder(void)
{
}

HIDReportDescriptorBuilder::~HIDReportDescriptorBuilder(void)
{
}



void HIDReportDescriptorBuilder::AddValue(byte tag)
{
    AddByte(tag);
}

void HIDReportDescriptorBuilder::AddValue(byte tag, int n)
{
    // only send the number of bytes we need to
    int8_t n1 = (int8_t)n;
    short n2 = (short)n;

    if (n1 == n)
    {
        AddByte((byte)(tag | 1));   // 1 byte
        AddByte((byte)n1);
    }
    else if (n2 == n)
    {
        AddByte((byte)(tag | 2));   // 2 bytes
        AddShort((ushort)n2);
    }
    else
    {
        AddByte((byte)(tag | 3));   // 4 bytes
        AddArray((byte *)&n,sizeof(n));
    }

}

void HIDReportDescriptorBuilder::AddValue(byte tag, uint n)
{
    // only send the number of we need to
    byte n1 = (byte)n;
    ushort n2 = (ushort)n;

    if (n1 == n)
    {
        AddByte((byte)(tag | 1));   // 1 byte
        AddByte((byte)n1);
    }
    else if (n2 == n)
    {
        AddByte((byte)(tag | 2));   // 2 bytes
        AddShort((ushort)n2);
    }
    else
    {
        AddByte((byte)(tag | 3));   // 4 bytes
        AddArray((byte *)&n,sizeof(n));
    }
}

void HIDReportDescriptorBuilder::UsagePage(uint n)
{
    AddValue(TagUsagePage, n);
}

void HIDReportDescriptorBuilder::Usage(uint n)
{
    AddValue(TagUsage, n);
}
void HIDReportDescriptorBuilder::UsageMin(uint n)
{
    AddValue(TagUsageMinimum, n);
}

void HIDReportDescriptorBuilder::UsageMax(uint n)
{
    AddValue(TagUsageMaximum, n);
}


void HIDReportDescriptorBuilder::Collection(CollectionType::CollectionType t)
{
    AddValue(TagCollection, (uint)t);
}

void HIDReportDescriptorBuilder::EndCollection()
{
    AddValue( TagEndCollection );
}

void HIDReportDescriptorBuilder::ReportID(uint n)
{
    AddValue(TagReportID, n);
}
void HIDReportDescriptorBuilder::LogicalMinimum(int n)
{
    AddValue(TagLogicalMinimum, n);
}
void HIDReportDescriptorBuilder::LogicalMaximum(int n)
{
    AddValue(TagLogicalMaximum, n);
}
void HIDReportDescriptorBuilder::PhysicalMinimum(int n)
{
    AddValue(TagPhysicalMinimum, n);
}
void HIDReportDescriptorBuilder::PhysicalMaximum(int n)
{
    AddValue(TagPhysicalMaximum, n);
}
void HIDReportDescriptorBuilder::ReportSize(uint n)
{
    AddValue(TagReportSize, n);
}
void HIDReportDescriptorBuilder::ReportCount(uint n)
{
    AddValue(TagReportCount, n);
}
void HIDReportDescriptorBuilder::StringIndex(uint n)
{
    AddValue(TagStringIndex, n);
}
void HIDReportDescriptorBuilder::StringMin(uint n)
{
    AddValue(TagStringMinimum, n);
}
void HIDReportDescriptorBuilder::StringMax(uint n)
{
    AddValue(TagStringMaximum, n);
}


void HIDReportDescriptorBuilder::Input(EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType,  EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EBufferType::EBufferType bufferType )
{
    AddInputOutputFeature( TagInput, dataType, varType, relType, wrapType, linearType, preferredType, nullType, EVolatileType::NonVolatile, bufferType );
}
void HIDReportDescriptorBuilder::Output(EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType, EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EVolatileType::EVolatileType volType, EBufferType::EBufferType bufferType)
{
    AddInputOutputFeature(TagOutput, dataType, varType, relType, wrapType, linearType, preferredType, nullType, volType, bufferType);
}
void HIDReportDescriptorBuilder::Feature(EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType, EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EVolatileType::EVolatileType volType, EBufferType::EBufferType bufferType)
{
    AddInputOutputFeature(TagFeature, dataType, varType, relType, wrapType, linearType, preferredType, nullType, volType, bufferType);
}
void HIDReportDescriptorBuilder::AddInputOutputFeature(byte tag, EDataType::EDataType dataType, EVarType::EVarType varType, ERelType::ERelType relType,  EWrapType::EWrapType wrapType, ELinearType::ELinearType linearType, EPreferedType::EPreferedType preferredType, ENullPositionType::ENullPositionType nullType, EVolatileType::EVolatileType volType, EBufferType::EBufferType bufferType )
{
    uint nValue = 0;
    if ( dataType == EDataType::Constant )
	nValue |= 0x01;
    if ( varType == EVarType::Variable )
	nValue |= 0x02;
    if ( relType == ERelType::Relative )
	nValue |= 0x04;
    if ( wrapType == EWrapType::Wrap )
	nValue |= 0x08;
    if ( linearType == ELinearType::NonLinear )
	nValue |= 0x10;
    if ( preferredType == EPreferedType::NoPreferred )
	nValue |= 0x20;
    if ( nullType == ENullPositionType::NullState )
	nValue |= 0x40;
    if ( volType == EVolatileType::Volatile )
	nValue |= 0x80;
    if ( bufferType == EBufferType::Buffered )
	nValue |= 0x100;

    AddValue(tag, nValue);
}
#include "stdafx.h"
#include "shapecodedselectorswitch.h"

ShapeCodedSelectorSwitch::ShapeCodedSelectorSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapeCodedSelectorSwitch::~ShapeCodedSelectorSwitch(void)
{
}

bool ShapeCodedSelectorSwitch::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    int nOutputs = GetPropertyValueInt( "Outputs", values, 0 );

    QSet<QString> pinsToCheck, notPinsToCheck;
    notPinsToCheck << "1" << "2" << "4" << "8";

    int nBits = 0;
    nOutputs--;
    while ( nOutputs != 0 )
    {
	nBits++;
	nOutputs >>= 1;
    }

    for ( int i = 0; i < nBits; i++ )
	pinsToCheck << QString::number( 1 << i );

    notPinsToCheck.subtract( pinsToCheck );

    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
	bSuccess = false;

    return bSuccess;
}

void ShapeCodedSelectorSwitch::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    int nBits = GetPropertyValueInt("Bits",values,0);
    QStringList outputs;
    for ( int i = 0; i < nBits; i++ )
	outputs << GetPort( pins, QString::number( 1<<i ) );

    MakeRotarySwitchControl( elem, 
			     GetPropertyValueString("Name",values,""), 
			     GetPropertyValueUsagePage("Usage",values,1),
			     GetPropertyValueUsage("Usage",values,1),
			     GetPropertyValueBool("Pullup",values,true),
			     GetPropertyValueBool("Debounce",values,true),
			     true,
			     nBits,
			     outputs );
}

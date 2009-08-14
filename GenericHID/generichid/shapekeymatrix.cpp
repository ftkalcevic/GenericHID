#include "stdafx.h"
#include "shapekeymatrix.h"

const int MAX_ROWS = 8;
const int MAX_COLUMNS = 8;

ShapeKeyMatrix::ShapeKeyMatrix(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapeKeyMatrix::~ShapeKeyMatrix(void)
{
}

bool ShapeKeyMatrix::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    QSet<QString> pinsToCheck, notPinsToCheck;

    int nRows = GetPropertyValueInt( "Rows", values, 0 );
    int nColumns = GetPropertyValueInt( "Columns", values, 0 );

    for ( int i = 0; i < MAX_ROWS; i++ )
    {
	QString s = QString("R%1").arg(i+1);
	if ( i < nRows )
	    pinsToCheck << s;
	else
	    notPinsToCheck << s;
    }

    for ( int i = 0; i < MAX_COLUMNS; i++ )
    {
	QString s = QString("C%1").arg(i+1);
	if ( i < nColumns )
	    pinsToCheck << s;
	else
	    notPinsToCheck << s;
    }


    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
	bSuccess = false;

    return bSuccess;
}


void ShapeKeyMatrix::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    int nRows = GetPropertyValueInt("Rows",values,0);
    int nCols = GetPropertyValueInt("Columns",values,0);

    QStringList rows, cols;
    for ( int i = 0; i < nRows; i++ )
	rows << QString("R%1").arg(i);

    for ( int i = 0; i < nCols; i++ )
	cols << QString("C%1").arg(i);

    MakeKeyMatrixControl( elem, 
			  GetPropertyValueString("Name",values,""), 
			  GetPropertyValueUsagePage("Usage",values,1),
		          GetPropertyValueUsage("Usage",values,1),
		          GetPropertyValueBool("Pullup",values,true),
		          GetPropertyValueBool("Debounce",values,true),
		          rows,
		          cols );
}
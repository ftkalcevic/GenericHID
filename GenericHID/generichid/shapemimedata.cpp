#include "stdafx.h"
#include "shapemimedata.h"

ShapeMimeData::ShapeMimeData( const Shape *pShape )
: QMimeData()
, m_pShape(pShape)
{
}

ShapeMimeData::~ShapeMimeData()
{
}

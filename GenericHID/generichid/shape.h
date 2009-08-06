#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "pin.h"
#include "shapeproperty.h"

namespace ShapeType
{
    enum ShapeType
    {
	AT90USB128,
	Pot,
	DigitalEncoder,
	Switch,
	KeyMatrix,
	RotarySwitch,
	CodedRotarySwitch,
	LCD,
	LED,
	BiColourLED,
	TriColourLED,
	DirSwitch,
	None,
    };

    extern enum ShapeType fromString( const QString &s );
    extern const char * toString( enum ShapeType eType );
};

class Shape
{
public:
    Shape(QDomElement &node, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~Shape(void);

    static Shape *CreateFromXML( QDomElement &doc );
    const QString &iconFile() const { return m_sIconFile; }
    const QString &shapeFile() const  { return m_sImageFile; }
    const QString &name() const  { return m_sShapeId; }
    const QString &description() const  { return m_sDescription; }

private:
    QString m_sShapeId;
    bool m_bSource;
    QString m_sImageFile;
    int m_nImageWidth;
    int m_nImageHeight;
    QString m_sIconFile;
    int m_nMaxInstances;
    QString m_sDescription;

    QList<Pin *> m_Pins;
    QList<ShapeProperty *> m_Properties;
};

#endif

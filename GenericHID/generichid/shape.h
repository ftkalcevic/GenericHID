#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "pin.h"
#include "shapeproperties.h"
#include "pinitem.h"

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
	RGB,
	BiColourLED,
	TriColourLED,
	DirSwitch,
	Counter,
	PWM,
	Power,
	None,
    };

    extern enum ShapeType fromString( const QString &s );
    extern const char * toString( enum ShapeType eType );
};

class Shape
{
public:
    Shape(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~Shape(void);

    static Shape *CreateFromXML( QDomElement &doc, QString &sError );
    const QString &iconFile() const { return m_sIconFile; }
    const QString &shapeFile() const  { return m_sImageFile; }
    const QString &id() const  { return m_sShapeId; }
    const QString &name() const  { return m_sShapeName; }
    const QString &description() const  { return m_sDescription; }
    int maxInstances() const  { return m_nMaxInstances; }
    ShapeType::ShapeType shapeType() const { return m_eShapeType; }
    QPixmap pixmap();
    const QList<Pin *> &pins() const { return m_Pins; }
    const ShapeProperties  &properties() const { return m_Properties; }
    bool source() const { return m_bSource; }	// source or sink device
    virtual bool Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values ) const;
    int findPin( const QString &sName ) const;
    bool CheckPins( QString &sErrors, const QList<class PinItem *> &pins, const QSet<QString> &pinsToCheck ) const;
    bool CheckNotPins( QString &sErrors, const QList<class PinItem *> &pins, const QSet<QString> &notPinsToCheck ) const;
    virtual void MakeDeviceXML( QDomElement &elem, int nCurrent, const QString &sPowerPin, const QList<PropertyValue *> &values  ) const;
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
    PropertyValue *GetPropertyValue( const QString &sName, const QList<class PropertyValue *> &values ) const;
    bool GetPropertyValueBool( const QString &sName, const QList<PropertyValue *> &values, bool bDefault ) const;
    QString GetPropertyValueEnum( const QString &sName, const QList<PropertyValue *> &values, const QString &sDefault ) const;
    int GetPropertyValueInt( const QString &sName, const QList<class PropertyValue *> &values, int nDefault ) const;
    QString GetPropertyValueString( const QString &sName, const QList<class PropertyValue *> &values, const QString &sDefault ) const;
    unsigned short GetPropertyValueUsagePage( const QString &sName, const QList<class PropertyValue *> &values, int nDefault ) const;
    unsigned short GetPropertyValueUsage( const QString &sName, const QList<class PropertyValue *> &values, int nDefault ) const;
    QString GetPort( QList<PinItem *> pins, const QString &sName  ) const;

    virtual void PropertyChanged( QtBrowserItem *, QList<PropertyValue *> & ) const {}
    virtual void populateProperties( QList<PropertyValue *> &values ) const;
    virtual void retrieveProperties( QList<PropertyValue *> &values ) const;
    virtual void wireAddedEvent( WireItem *, QList<PropertyValue *> & ) const {}

    void MakeDirectionalSwitchControl( QDomElement &node, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bPullup, bool bDebounce, const QString &sDirections, const QString &sNorthPort, const QString &sSouthPort, const QString &sEastPort, const QString &sWestPort, const QString &sNEPort, const QString &sNWPort, const QString &sSEPort, const QString &sSWPort ) const;
    void MakePotentiometerControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nBits, const QString &sPort ) const;
    void MakeSwitchControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bPullup, bool bDebounce, const QString &sPort ) const;
    void MakeTricolourLEDControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bSource, const QString &sPort1, const QString &sPort2 ) const;
    void MakeDigitalEncoderControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, const QString &sPortA, const QString &sPortB ) const;
    void MakeKeyMatrixControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsageMin, bool bDebounce, const QStringList &rows, const QStringList &cols  ) const;
    void MakeRotarySwitchControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bPullUp, bool bDebounce, bool bEncoded, int nBits, const QStringList &outputs ) const;
    void MakeLCDControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nRows, int nCols, bool b8Bit,
			 const QString &sPortRS, const QString &sPortRW, const QString &sPortE, const QString &sPortD0, const QString &sPortD1, const QString &sPortD2,
			 const QString &sPortD3, const QString &sPortD4, const QString &sPortD5, const QString &sPortD6, const QString &sPortD7, 
			 int nAddrRow0, int nAddrRow1, int nAddrRow2, int nAddrRow3 ) const;
    void MakeLEDControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, const QString &sPort, bool bSource ) const;
    void MakeBiColourControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, const QString &sPortA, const QString &sPortB ) const;
    void MakeRGBControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bSource, const QString &sPortR, const QString &sPortG, const QString &sPortB ) const;
    void MakePWMControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nResolution, const QString &sPort ) const;
    void MakeCounterControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nPeriod, int nBits ) const;

protected:
    QString m_sShapeName;
    ShapeType::ShapeType m_eShapeType;
    QString m_sShapeId;
    bool m_bSource;
    QString m_sImageFile;
    int m_nImageWidth;
    int m_nImageHeight;
    QString m_sIconFile;
    int m_nMaxInstances;
    QString m_sDescription;

    QList<Pin *> m_Pins;
    ShapeProperties m_Properties;
    QPixmap m_pixmap;
};

#endif



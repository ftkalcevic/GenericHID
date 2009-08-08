#ifndef _PIN_H_
#define _PIN_H_


namespace PinType
{
    enum PinType
    {
	None = 0,
	ADC = 1,
	Interrupt = 2,
	IO = 4,
	PWM = 8
    };
    extern enum PinType fromString(const QString &sPinType);
};

namespace PinHAlign
{
    enum PinHAlign
    {
	Centre,
	Left,
	Right,
	OutsideLeft,
	OutsideRight,
    };
    extern enum PinHAlign fromString(const QString &s);
};

namespace PinVAlign
{
    enum PinVAlign
    {
	Centre,
	Top,
	Bottom,
	Above,
	Below
    };

    extern enum PinVAlign fromString(const QString &s);
};

class Shape;

class Pin
{
public:
    Pin( const Shape *pShape, const QString &sId, PinType::PinType ePinType, const QRect &rect, const QString &sOtherUser, bool bEnabled, PinHAlign::PinHAlign eHAlign, PinVAlign::PinVAlign eVAlign, double dRotate );
    ~Pin(void);

    static Pin *CreateFromXML( QDomElement node, const Shape *pShape );
    const QString &id() const { return m_sId; }
    PinType::PinType pinType() const { return  m_ePinType; }
    const QRect geometry() const { return m_geometry; }
    const QString &otherUse() const { return m_sOtherUse; }
    bool enabled() const { return m_bEnabled; }
    PinHAlign::PinHAlign hAlign() const { return m_eHAlign; }
    PinVAlign::PinVAlign vAlign() const { return m_eVAlign; }
    double rotate() const { return m_dRotate; }
    const Shape *shape() const { return m_pShape; }

private:
    QString m_sId;
    PinType::PinType m_ePinType;
    QRect m_geometry;
    QString m_sOtherUse;
    bool m_bEnabled;
    PinHAlign::PinHAlign m_eHAlign;
    PinVAlign::PinVAlign m_eVAlign;
    double m_dRotate;
    const Shape *m_pShape;
};


#endif

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
    extern QString fromString( enum PinType ePinType );
};

class Pin
{
public:
    Pin( const QString &sId, PinType::PinType ePinType, const QRect &rect, const QString &sOtherUser );
    ~Pin(void);

    static Pin *CreateFromXML( QDomElement node );
private:
    QString m_sId;
    PinType::PinType m_ePinType;
    QRect m_geometry;
    QString m_sOtherUse;
};


#endif

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

#ifndef _PIN_H_
#define _PIN_H_


namespace PinType
{
    enum PinType
    {
	None =	    0x0000,
	ADC =	    0x0001,
	Interrupt = 0x0002,
	IO =	    0x0004,
	PWM =	    0x0008,
	SS =	    0x0010,
	SCK =	    0x0020,
	MOSI =	    0x0040,
	MISO =	    0x0080
    };
    extern enum PinType fromString(const QString &sPinType);
    extern QString toString(enum PinType eType);
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
    Pin( const Shape *pShape, const QString &sId, PinType::PinType ePinType, const QRect &rect, const QString &sOtherUser, bool bEnabled, PinHAlign::PinHAlign eHAlign, PinVAlign::PinVAlign eVAlign, double dRotate, bool bShared );
    ~Pin(void);

    static Pin *CreateFromXML( QDomElement node, const Shape *pShape, QString &sError );
    const QString &id() const { return m_sId; }
    PinType::PinType pinType() const { return  m_ePinType; }
    const QRect geometry() const { return m_geometry; }
    const QString &otherUse() const { return m_sOtherUse; }
    bool enabled() const { return m_bEnabled; }
    PinHAlign::PinHAlign hAlign() const { return m_eHAlign; }
    PinVAlign::PinVAlign vAlign() const { return m_eVAlign; }
    double rotate() const { return m_dRotate; }
    const Shape *shape() const { return m_pShape; }
    bool shared() const { return m_bShared; }

private:
    const Shape *m_pShape;
    QString m_sId;
    PinType::PinType m_ePinType;
    QRect m_geometry;
    QString m_sOtherUse;
    bool m_bEnabled;
    PinHAlign::PinHAlign m_eHAlign;
    PinVAlign::PinVAlign m_eVAlign;
    double m_dRotate;
    bool m_bShared;
};


#endif

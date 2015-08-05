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

#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

#define _USE_MATH_DEFINES
#include <math.h>

class Vector2D
{
    double m_dX;
    double m_dY;

public:
    double X() { return m_dX;}
    double Y() { return m_dY;}

    Vector2D(double x, double y)
    {
        m_dX = x;
        m_dY = y;
    }

    void Normalise()
    {
        double d = sqrt(m_dX * m_dX + m_dY * m_dY);
        m_dX /= d;
        m_dY /= d;
    }

    void Scale( double s )
    {
        m_dX *= s;
        m_dY *= s;
    }

    Vector2D MakePerpendicular( bool bRight )
    {
#if PERPENDICULAR_NEG_INVERSE
        Vector2D v;
        if (m_dX == 0)
            v = new Vector2D(-m_dY, 0 );
        else if ( m_dY == 0 )
            v = new Vector2D(0, -m_dX);
        else
            v = new Vector2D(1.0, -m_dX / m_dY);
        v.Normalise();
        return v;
#else
        double dAngle = atan2(m_dY, m_dX);
        if (bRight)
            dAngle -= M_PI / 2.0;
        else
            dAngle += M_PI / 2.0;

        return Vector2D(cos(dAngle), sin(dAngle));
#endif
    }

    QPointF toPoint()
    {
        return QPointF(m_dX, m_dY);
    }

    //static public bool operator ==(Vector2D v1, Vector2D v2)
    //{
    //    if ((object)v1 == null || (object)v2 == null)
    //        return false;
    //    else if (System.Object.ReferenceEquals(v1, v2))
    //        return true;
    //    else
    //        return Utility.IsNear(v1.m_dX, v2.m_dX) && Utility.IsNear(v1.m_dY, v2.m_dY);
    //}
    //static public bool operator !=(Vector2D v1, Vector2D v2)
    //{
    //    return !(v1 == v2);
    //}
};

#endif

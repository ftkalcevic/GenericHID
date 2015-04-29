// hidcomp/hidconfig/generichid, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

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

#include "stdafx.h"
#include "hatswitchwidget.h"

HatSwitchWidget::HatSwitchWidget(QWidget *parent)
: QWidget(parent)
, m_Logger( QCoreApplication::applicationName().toAscii().constData(), "HatSwitchWidget" )
{
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4127)
#endif
    Q_INIT_RESOURCE(qtcommon);
#ifdef _WIN32
#pragma warning(pop)
#endif

    m_pDot = new QPixmap( ":/hatswitch/resources/hatdot.bmp" );
    m_pDotHighlight = new QPixmap( ":/hatswitch/resources/hatdothi.bmp" );
    m_pArrow = new QPixmap( ":/hatswitch/resources/hatarrow.bmp" );
    m_pArrowHighlight = new QPixmap( ":/hatswitch/resources/hatarrowhi.bmp" );
    m_nValue = 0;

    // Hatswitches are fixed size
    const int SPACING = 5;
    int nWidth = m_pArrow->width() + m_pArrow->height() * 2 + 2 * SPACING;

    setFixedSize( QSize(nWidth, nWidth));
}

HatSwitchWidget::~HatSwitchWidget()
{
}

void HatSwitchWidget::paintEvent( QPaintEvent * )
{
    QPainter p( this );

    int n = minimumWidth();
    p.fillRect(0,0,n,n, QColor(255,255,255));

    LOG_MSG( m_Logger, LogTypes::Debug, QString("Hatswitch value=%1").arg(m_nValue) );
    // Put the dot in the middle
    QPixmap *pDot = m_nValue < 0 ? m_pDotHighlight : m_pDot;
    p.drawPixmap( minimumWidth()/2 - m_pDot->width()/2, minimumHeight()/2 - m_pDot->width()/2, *pDot );

    // Now the arrows.
    for ( int i = 0; i < 8; i++ )
    {
        QTransform trans;
        int xc = minimumWidth()/2;
        int yc = minimumHeight()/2;
        trans.translate( xc, yc );
        trans.rotate( (qreal)i * 45.0 );
        trans.translate( -xc, -yc );

        p.setWorldTransform( trans );
        p.setWorldMatrixEnabled(true);

        QPixmap *pArrow = m_nValue == i ? m_pArrowHighlight : m_pArrow;
        p.drawPixmap( minimumWidth()/2 - m_pArrow->width()/2, 0, *pArrow );
    }
}


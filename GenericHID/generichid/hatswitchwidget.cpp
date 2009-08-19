#include "stdafx.h"
#include "hatswitchwidget.h"

HatSwitchWidget::HatSwitchWidget(QWidget *parent)
: QWidget(parent)
, m_Logger( QCoreApplication::applicationName().toAscii().constData(), "HatSwitchWidget" )
{
    m_pDot = new QPixmap( ":GenericHID/HatDot" );
    m_pDotHighlight = new QPixmap( ":GenericHID/HatDotHi" );
    m_pArrow = new QPixmap( ":GenericHID/HatArrow" );
    m_pArrowHighlight = new QPixmap( ":GenericHID/HatArrowHi" );
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
    QPixmap *pDot = (m_nValue < 0 || m_nValue > 7) ? m_pDotHighlight : m_pDot;
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


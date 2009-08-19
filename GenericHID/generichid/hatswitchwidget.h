#ifndef HATSWITCHWIDGET_H
#define HATSWITCHWIDGET_H

#include <QWidget>

class HatSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    HatSwitchWidget(QWidget *parent=NULL);
    ~HatSwitchWidget();

    void setValue( int nValue ) { m_nValue = nValue; update(); }

private:
    Logger m_Logger;
    QPixmap *m_pDot;
    QPixmap *m_pDotHighlight;
    QPixmap *m_pArrow;
    QPixmap *m_pArrowHighlight;
    int m_nValue;

    virtual void paintEvent( QPaintEvent * evnt );
};

#endif // HATSWITCHWIDGET_H

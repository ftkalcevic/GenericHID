#include "stdafx.h"
#include "timereditwidget.h"
#include "timerconfigdlg.h"


static inline void setupTreeViewEditorMargin(QLayout *lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    else
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
}


TimerEditWidget::TimerEditWidget(QWidget *parent) :
    QWidget(parent),
    m_label(new QLabel),
    m_button(new QToolButton)
{
    QHBoxLayout *lt = new QHBoxLayout(this);
    setupTreeViewEditorMargin(lt);
    lt->setSpacing(0);
    lt->addWidget(m_label);
    lt->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));

    m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    m_button->setFixedWidth(20);
    setFocusProxy(m_button);
    setFocusPolicy(m_button->focusPolicy());
    m_button->setText(tr("..."));
    m_button->installEventFilter(this);
    connect(m_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    lt->addWidget(m_button);
    m_label->setText(m_sTimerDetails);
}

void TimerEditWidget::setValue(const QString &s)
{
    if (m_sTimerDetails != s) 
    {
        m_sTimerDetails = s;
        m_label->setText(s);
    }
}

void TimerEditWidget::buttonClicked()
{
    TimerConfigDlg dlg( m_nBits, m_sPrescales, this );
    dlg.setValue( m_sTimerDetails );
    if ( dlg.exec() == QDialog::Accepted )
    {
        setValue(dlg.value());
        emit valueChanged(m_sTimerDetails);
    }
}

bool TimerEditWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == m_button) 
    {
        switch (ev->type()) 
	{
	    case QEvent::KeyPress:
	    case QEvent::KeyRelease: 
	    {	// Prevent the QToolButton from handling Enter/Escape meant control the delegate
		switch (static_cast<const QKeyEvent*>(ev)->key()) 
		{
		    case Qt::Key_Escape:
		    case Qt::Key_Enter:
		    case Qt::Key_Return:
			ev->ignore();
			return true;
		    default:
			break;
		    }
		}
		break;
	    default:
		break;
        }
    }
    return QWidget::eventFilter(obj, ev);
}


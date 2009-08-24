#include "stdafx.h"
#include "keymatrixeditwidget.h"
#include "keymatrixnamedlg.h"


static inline void setupTreeViewEditorMargin(QLayout *lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    else
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
}


KeyMatrixEditWidget::KeyMatrixEditWidget(QWidget *parent) :
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
//    m_label->setText(TimerCounter::MakeUserDisplay(m_sNames));
    m_label->setText(m_sNames);
}

void KeyMatrixEditWidget::setValue(const QString &s)
{
    if (m_sNames != s) 
    {
        m_sNames = s;
        //m_label->setText(TimerCounter::MakeUserDisplay(m_sNames));
	m_label->setText(m_sNames);
    }
}

void KeyMatrixEditWidget::buttonClicked()
{
    KeyMatrixNameDlg dlg(this );
    dlg.setRC( m_nRows, m_nColumns );
    dlg.setValue( m_sNames );
    if ( dlg.exec() == QDialog::Accepted )
    {
        setValue(dlg.value());
        emit valueChanged(m_sNames);
    }
}

bool KeyMatrixEditWidget::eventFilter(QObject *obj, QEvent *ev)
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


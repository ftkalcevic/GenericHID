#ifndef TIMEREDITWIDGET_H
#define TIMEREDITWIDGET_H

#include <QWidget>

class TimerEditWidget : public QWidget
{
    Q_OBJECT

public:
    TimerEditWidget(QWidget *parent);

    bool eventFilter(QObject *obj, QEvent *ev);
    void setBits( int nBits ) { m_nBits = nBits; }
    void setPrescales( QStringList &sPrescales ) { m_sPrescales = sPrescales; }

public Q_SLOTS:
    void setValue(const QString &value);

private Q_SLOTS:
    void buttonClicked();

Q_SIGNALS:
    void valueChanged(const QString &value);

private:
    QString m_sTimerDetails;
    QLabel *m_label;
    QToolButton *m_button;
    int m_nBits;
    QStringList m_sPrescales;
};

#endif // TIMEREDITWIDGET_H




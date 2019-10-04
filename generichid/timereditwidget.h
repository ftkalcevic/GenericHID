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

#ifndef TIMEREDITWIDGET_H
#define TIMEREDITWIDGET_H

#include <QWidget>

class TimerEditWidget : public QWidget
{
    Q_OBJECT

public:
    TimerEditWidget(QWidget *parent );

    bool eventFilter(QObject *obj, QEvent *ev);
    void setBits( int nBits ) { m_nBits = nBits; }
    void setPrescales( QStringList &sPrescales ) { m_sPrescales = sPrescales; }
    void setCPUClock( unsigned int n ) { m_nCPUClockFrequency = n; }

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
    unsigned int m_nCPUClockFrequency;
};

#endif // TIMEREDITWIDGET_H




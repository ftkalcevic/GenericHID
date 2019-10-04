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

#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include "columnlayout.h"
#include "testitem.h"
#include "hiddatathread.h"

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    TestWidget(QWidget *parent);
    ~TestWidget();
    void Activate();
    void Deactivate();

public slots:
    void onDevicesIndexChanged( int index );
    void onRefreshPressed();
    void onNewData( QVector<byte> data );
    void onValueChanged(int);

private:
    void DisplayDevice( class HIDDevice *pDevice );
    void StartListening();
    void StopListening();

    Logger m_Logger;
    QComboBox *m_cboDevices;
    QPushButton *m_btnRefresh;
    class HIDDevices *m_pDevices;
    class HIDDevice *m_pActiveDevice;
    ColumnLayout *m_pDeviceLayout;
    bool m_bLoading;
    QList<TestItem *> m_pTestItems;
    HIDDataThread *m_pThread;
    QString m_sLastDevice;
};

#endif // TESTWIDGET_H

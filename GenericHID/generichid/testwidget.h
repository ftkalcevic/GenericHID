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

    QComboBox *m_cboDevices;
    QPushButton *m_btnRefresh;
    class HIDDevices *m_pDevices;
    class HIDDevice *m_pActiveDevice;
    ColumnLayout *m_pDeviceLayout;
    bool m_bLoading;
    QList<TestItem *> m_pTestItems;
    HIDDataThread *m_pThread;
};

#endif // TESTWIDGET_H

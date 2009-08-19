#ifndef HIDDATATHREAD_H
#define HIDDATATHREAD_H

#include "hiddevice.h"

class HIDDataThread : public QThread
{
    Q_OBJECT

public:
    HIDDataThread(HIDDevice *pDevice);
    ~HIDDataThread();

signals:
    void newData( QVector<byte> data );

public:
    virtual void run ();
    void stop();
    enum 
    {
	LOOP_TIMEOUT = 1000
    };
private:
    bool m_bRunning;
    HIDDevice *m_pDevice;
    QReadWriteLock m_queueLock;
    QVector<QVector<byte> > m_dataQueue;
};

#endif // HIDDATATHREAD_H

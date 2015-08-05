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
        LOOP_TIMEOUT = 100
    };
private:
    bool m_bRunning;
    HIDDevice *m_pDevice;
    QReadWriteLock m_queueLock;
    QVector<QVector<byte> > m_dataQueue;
};

#endif // HIDDATATHREAD_H

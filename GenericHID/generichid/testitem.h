#ifndef _TESTITEM_H_
#define _TESTITEM_H_

#include <QWidget>

class TestItem : public QWidget
{
    Q_OBJECT

public:
    TestItem(byte m_nReportId, QWidget *parent = NULL);
    virtual ~TestItem();

    virtual bool isInput() const = 0;
    byte reportId() const { return m_nReportId; }

    static TestItem *CreateItem( class HIDDevice *pDevice, struct HID_ReportItem_t *pItem );

protected:
    byte m_nReportId;
};

#endif // TESTITEM_H

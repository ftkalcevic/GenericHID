#ifndef SHAPEPROPERTYBROWSER_H
#define SHAPEPROPERTYBROWSER_H

#include <QtTreePropertyBrowser>

class ShapePropertyBrowser : public QtTreePropertyBrowser
{
    Q_OBJECT

public:
    ShapePropertyBrowser(QWidget *parent);
    ~ShapePropertyBrowser();

signals:
    void itemDataChanged( QtBrowserItem * item );

private:
    virtual void itemChanged ( QtBrowserItem * item )
    {
	QtTreePropertyBrowser::itemChanged( item );
	emit itemDataChanged( item );
    }
};

#endif // SHAPEPROPERTYBROWSER_H

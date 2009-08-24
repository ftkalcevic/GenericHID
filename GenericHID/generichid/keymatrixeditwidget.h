#ifndef KEYMATRIXEDITWIDGET_H
#define KEYMATRIXEDITWIDGET_H

#include <QWidget>

class KeyMatrixEditWidget : public QWidget
{
    Q_OBJECT

public:
    KeyMatrixEditWidget(QWidget *parent);

    bool eventFilter(QObject *obj, QEvent *ev);
    void setRC( int nRows, int nColumns ) { m_nRows = nRows; m_nColumns = nColumns; }

public Q_SLOTS:
    void setValue(const QString &value);

private Q_SLOTS:
    void buttonClicked();

Q_SIGNALS:
    void valueChanged(const QString &value);

private:
    QString m_sNames;
    QLabel *m_label;
    QToolButton *m_button;
    int m_nRows;
    int m_nColumns;
};

#endif // KeyMatrixEditWidget_H




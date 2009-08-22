#ifndef TIMEREDITORFACTORY_H
#define TIMEREDITORFACTORY_H

#include <QtAbstractEditorFactoryBase>
#include <QtAbstractPropertyManager>

class TimerEditorFactoryPrivate;

class TimerEditorFactory : public QtAbstractEditorFactory<QtStringPropertyManager>
{
    Q_OBJECT

public:
    TimerEditorFactory(int nBits, QStringList sPrescales, QObject *parent=0);
    ~TimerEditorFactory();

protected:
    void connectPropertyManager(QtStringPropertyManager *manager);
    QWidget *createEditor(QtStringPropertyManager *manager, QtProperty *property, QWidget *parent);
    void disconnectPropertyManager(QtStringPropertyManager *manager);
private:
    TimerEditorFactoryPrivate *d_ptr;
    int m_nBits;
    QStringList m_sPrescales;
    Q_DECLARE_PRIVATE(TimerEditorFactory)
    Q_DISABLE_COPY(TimerEditorFactory)

public slots:
    void slotPropertyChanged(QtProperty *, const QString &);
    void slotEditorDestroyed(QObject *);
    void slotSetValue(const QString &);
};

#endif // TIMEREDITORFACTORY_H



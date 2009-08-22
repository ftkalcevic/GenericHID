#ifndef TIMEREDITORFACTORY_H
#define TIMEREDITORFACTORY_H

#include <QtAbstractEditorFactoryBase>
#include <QtAbstractPropertyManager>

class TimerEditorFactoryPrivate;

class TimerStringPropertyManager: public QtStringPropertyManager
{
    Q_OBJECT

public:
    TimerStringPropertyManager(QObject *parent=0) : QtStringPropertyManager(parent) {}
    QString valueText(const QtProperty *property) const;
};

class TimerEditorFactory : public QtAbstractEditorFactory<TimerStringPropertyManager>
{
    Q_OBJECT

public:
    TimerEditorFactory(int nBits, QStringList sPrescales, QObject *parent=0);
    ~TimerEditorFactory();

public:
    void connectPropertyManager(TimerStringPropertyManager *manager);
    QWidget *createEditor(TimerStringPropertyManager *manager, QtProperty *property, QWidget *parent);
    void disconnectPropertyManager(TimerStringPropertyManager *manager);
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



#ifndef KEYMATRIXEDITORFACTORY_H
#define KEYMATRIXEDITORFACTORY_H

#include <QtAbstractEditorFactoryBase>
#include <QtAbstractPropertyManager>

class KeyMatrixEditorFactoryPrivate;

class KeyMatrixStringPropertyManager: public QtStringPropertyManager
{
    Q_OBJECT

public:
    KeyMatrixStringPropertyManager(QObject *parent=0) : QtStringPropertyManager(parent) {}
    QString valueText(const QtProperty *property) const;
};

class KeyMatrixEditorFactory : public QtAbstractEditorFactory<KeyMatrixStringPropertyManager>
{
    Q_OBJECT

public:
    KeyMatrixEditorFactory(QObject *parent=0);
    ~KeyMatrixEditorFactory();

    void setRows( int nRows) { m_nRows = nRows; }
    void setColumns( int nColumns ) { m_nColumns = nColumns; }

public:
    void connectPropertyManager(KeyMatrixStringPropertyManager *manager);
    QWidget *createEditor(KeyMatrixStringPropertyManager *manager, QtProperty *property, QWidget *parent);
    void disconnectPropertyManager(KeyMatrixStringPropertyManager *manager);
private:
    KeyMatrixEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(KeyMatrixEditorFactory)
    Q_DISABLE_COPY(KeyMatrixEditorFactory)
    int m_nRows;
    int m_nColumns;

public slots:
    void slotPropertyChanged(QtProperty *, const QString &);
    void slotEditorDestroyed(QObject *);
    void slotSetValue(const QString &);
};


#endif // KeyMatrixEditorFactory_H



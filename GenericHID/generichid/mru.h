#ifndef _MRU_H_
#define _MRU_H_

const int MAX_MRU = 4;


class MRU: public QObject
{
    Q_OBJECT
public:
    MRU(void);
    ~MRU(void);
    void setMenu( QMenu *pMenu );
    void append( const QString &s );
    int count() const { return m_MRU.count(); }
    QString operator []( int i ) const { if ( i < m_MRU.count() ) return m_MRU[i]; else return QString(); }
signals:
    void MRUSelected(const QString &sFile);
private slots:
    void onActionTriggered(QAction*);
private:
    void Update();
    QList<QString> m_MRU;
    QAction *m_pSeparator;
    QList<QAction *> m_Actions;
    QActionGroup *m_pActionGroup;
};

#endif

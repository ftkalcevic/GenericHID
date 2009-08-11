#ifndef GENERICHID_H
#define GENERICHID_H

#include "ui_generichid.h"
#include "shapecollection.h"
#include "shapescene.h"
#include "mru.h"

class GenericHID : public QMainWindow, public Editor
{
    Q_OBJECT

public:
    GenericHID(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~GenericHID();
    void ProcessCommandline();

public slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileExit();
    void onFileNew();
    void onMicrocontrollerProgram();
    void onMicrocontrollerExport();
    void onMicrocontrollerImportAndProgram();
    void onDropShapeEvent( const ::Shape *pShape, QPointF pos );
    void onRotateTool();
    void onMirrorTool();
    void onPointerTool();
    void onWireLinkTool();
    void onSelectionChanged();
    void onMRUSelected(const QString &sFile);
    void onPropertiesCurrentItemChanged( QtBrowserItem * current );

private:
    virtual void closeEvent( QCloseEvent * event );

    void SetCursor( QCursor &cur1, QCursor &cur2 );
    void updateWindowTitle();
    bool DoSave();
    bool DoSaveAs();
    bool DoOpen( const QString &sFile );
    void writeSettings();
    void readSettings();
    void Clear();
    bool CheckDataChanged();

    QString m_sLastFile;
    QString m_sLastFileContents;
    Ui::GenericHIDClass ui;
    ShapeCollection *m_pShapes;
    ShapeScene *m_pScene;
    ShapeItem *m_pLastSelectedShape;
    QSettings m_Settings;
    MRU m_MRU;
};

#endif // GENERICHID_H

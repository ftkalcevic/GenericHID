#ifndef GENERICHID_H
#define GENERICHID_H

#include "ui_generichid.h"
#include "shapecollection.h"
#include "shapescene.h"

class GenericHID : public QMainWindow, public Editor
{
    Q_OBJECT

public:
    GenericHID(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~GenericHID();

public slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileExit();
    void onMicrocontrollerProgram();
    void onMicrocontrollerExport();
    void onMicrocontrollerImportAndProgram();
    void onDropShapeEvent( const ::Shape *pShape, QPointF pos );
    void onRotateTool();
    void onMirrorTool();
    void onPointerTool();
    void onWireLinkTool();
    void onSelectionChanged();

private:
    void SetCursor( QCursor &cur1, QCursor &cur2 );

    Ui::GenericHIDClass ui;
    ShapeCollection *m_pShapes;
    ShapeScene *m_pScene;
    ShapeItem *m_pLastSelectedShape;
};

#endif // GENERICHID_H

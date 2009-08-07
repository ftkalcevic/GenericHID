#ifndef GENERICHID_H
#define GENERICHID_H

#include "ui_generichid.h"
#include "shapecollection.h"
#include "shapeinstancecollection.h"

class GenericHID : public QMainWindow
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

private:
    Ui::GenericHIDClass ui;
    ShapeCollection *m_pShapes;
    ShapeInstanceCollection m_pShapeInstances;
    QGraphicsScene *m_pScene;
    QCursor *m_curMirror;
    QCursor *m_curRotate;
    QCursor *m_curWire;
};

#endif // GENERICHID_H

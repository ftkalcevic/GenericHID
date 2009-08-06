#ifndef GENERICHID_H
#define GENERICHID_H

#include "ui_generichid.h"
#include "shapecollection.h"

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

private:
    Ui::GenericHIDClass ui;
    ShapeCollection *m_pShapes;
};

#endif // GENERICHID_H

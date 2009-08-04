#ifndef GENERICHID_H
#define GENERICHID_H

#include <QtGui/QMainWindow>
#include "ui_generichid.h"

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
};

#endif // GENERICHID_H

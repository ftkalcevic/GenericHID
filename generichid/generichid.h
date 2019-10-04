// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
    bool initialised() const { return m_bInitialised; }

public slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileExit();
    void onFileNew();
    void onMicrocontrollerProgram();
    void onMicrocontrollerExport();
    void onMicrocontrollerImportAndProgram();
    void onOptionsDebug();
    void onHelpAbout();
    void onDropShapeEvent( const ::Shape *pShape, QPointF pos );
    void onRotateTool();
    void onMirrorTool();
    void onPointerTool();
    void onWireLinkTool();
    void onSelectionChanged();
    void onMRUSelected(const QString &sFile);
    void onPropertiesCurrentItemChanged( QtBrowserItem * current );
    void onPropertiesItemDataChanged( QtBrowserItem * current );
    void onTabChanged( int index ) ;
    void onZoomIndexChanged( const QString & text );
    void onZoomEditTextChanged( const QString & text );
    void onSceneScaleChanged( double);
    void onStatusChanged(const QString &s);

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
    void RetrieveProperties();
    void setMenus( bool bActive );
    void DoProgram( QString &sEEPROM );

    Logger m_Logger;
    QString m_sLastFile;
    QString m_sLastFileContents;
    QString m_sLastExportFile;
    Ui::GenericHIDClass ui;
    ShapeCollection *m_pShapes;
    ShapeScene *m_pScene;
    ShapeItem *m_pLastSelectedShape;
    QSettings m_Settings;
    MRU m_MRU;
    QComboBox *m_cboZoom;
    bool m_bInitialised;
};

#endif // GENERICHID_H

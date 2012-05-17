
/*****************************************************************************
 *   Copyright (C) 2006 Robert Szmurlo <robert@iem.pw.edu.pl>                *
 *                                                                           *
 *   Licensed under the Apache License, Version 2.0 (the "License");         *
 *   you may not use this file except in compliance with the License.        *
 *   You may obtain a copy of the License at                                 *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *   Unless required by applicable law or agreed to in writing, software     *
 *   distributed under the License is distributed on an "AS IS" BASIS,       *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *   See the License for the specific language governing permissions and     *
 *   limitations under the License.                                          *
 *****************************************************************************/



#include "fivermain.h"
#include <QDockWidget>
#include <QMenuBar>
#include <QLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QtDebug>

#include <fviewer.h>
#include <boxmgr.h>
#include <fvopen.h>
#include <fvobject.h>
#include <guiattributes.h>
#include <main.h>
#include <geom.h>

#include <QGLViewer/vec.h>
#include <QGLViewer/camera.h>
#include <QGLViewer/qglviewer.h>

#ifndef NO_QCONSOLE
    #include <fviewerinjector.h>
    #include <fvobjectinjector.h>
    #include <fvboxmanagerinjector.h>
    #include <qstringconverter.h>
#endif

/** GLOBAL FUNCTIONS AND VARIABLES*/
FViewer * __current_FViewer = 0;
FViewer * getCurrentViewer( )
{
	return __current_FViewer;
}
/** END OF: GLOBAL FUNCTIONS AND VARIABLES*/


FiverMain::FiverMain()
    : QMainWindow( 0 )
{
#ifndef NO_QCONSOLE
    console = 0;
#endif
    setupUI();
    buildMenu();
}


FiverMain::~FiverMain()
{
}

void FiverMain::buildMenu( )
{
        fileMenu = menuBar()->addMenu( tr("File") );
        fileMenu->addAction( tr("Open..."), this, SLOT(slotOpen()) );
        fileMenu->addAction( tr("Save as..."), this, SLOT(slotSaveAs()) );
        fileMenu->addSeparator();
	fileMenu->addAction( tr("Exit"), this, SLOT(slotExit()) );
	
        editMenu = menuBar()->addMenu( tr("Edit") );

        insertMenu = menuBar()->addMenu( tr("Insert") );
        insertMenu->addAction( tr("Sphere"), this, SLOT(slotSphere()) );
        insertMenu->addAction( tr("Coil"), this, SLOT(slotCoil()) );
        insertMenu->addAction( tr("Mesh Joiner"), this, SLOT(slotMeshJoiner()));


        settingsMenu = menuBar()->addMenu( tr("Settings") );
	settingsMenu->addAction( tr("FIVER Settings"), this, SLOT( slotFIVERSettings() ) );
        settingsMenu->addAction( tr("Macro Editor"), this, SLOT( slotMacroEditor() ) );

	layoutMenu = menuBar()->addMenu( tr("View") );
	layoutMenu->addAction( tr("Auto arrange boxes"), this, SLOT(slotArrangeBoxes() ) );
	layoutMenu->addSeparator();
        layoutMenu->addAction( tr("Show entire scene"), this, SLOT(slotShowEntireScene() ) );
        layoutMenu->addAction( tr("Save view state"), this, SLOT(slotSaveViewState() ) );
	layoutMenu->addAction( tr("Restore view state"), this, SLOT(slotRestoreViewState() ) );

}

void FiverMain::setupUI( )
{
        setWindowTitle(__FIVER_NAME__);

	dwNavi = new QDockWidget( tr ("Navigator"), this );
        addDockWidget( Qt::LeftDockWidgetArea, dwNavi);
	//bm = new FVBoxMgr( dwNavi );
	//QVBoxLayout * layout = new QVBoxLayout(dwNavi);

        bm = new FVBoxMgr( dwNavi );
        bm->setMinimumHeight( 200 );
        bm->setMinimumWidth( 200 );
        dwNavi->setWidget( bm );
        connect( bm,SIGNAL(signalSelected(BoxObj*)), this, SLOT( slotBoxSelected(BoxObj*) ) );
		
        dwAttr = new QDockWidget( tr ("Attributes"), this );
        addDockWidget( Qt::LeftDockWidgetArea, dwAttr);
	
        fvAttribs = new FVAttribs(dwAttr);
        connect( fvAttribs, SIGNAL( signalApply() ), this, SLOT( slotApplyAttribs() ) );
        dwAttr->setWidget( fvAttribs );

        FViewer * fv = new FViewer(this);
        __current_FViewer = fv;
        setCentralWidget( fv );

#ifndef NO_QCONSOLE
    if (console == 0) {
        console = new QConsole();
        FViewerInjector *fview = new FViewerInjector();
        FVObjectInjector *fvobject = new FVObjectInjector();
        FVBoxManagerInjector *fvboxmgr = new FVBoxManagerInjector();
        QStringConverter::init();
        console->introduce(fview);
        console->introduce(fvobject);
        console->introduce(fvboxmgr);

        console->inject((void*) getCurrentViewer(), "fviewer",
             fview);
        console->inject((void*) bm, "fmgr", fvboxmgr);
    }

    dwConsole = new QDockWidget( tr("Console"), this );
    addDockWidget( Qt::LeftDockWidgetArea, dwConsole );
    dwConsole->setWidget( console );
#endif

	fvOpen = new FVOpen(this);
}


void FiverMain::slotOpen( )
{
        fvOpen->exec(bm);
}

void FiverMain::slotSaveAs( )
{
        fvOpen->exec(bm, true);
}

void FiverMain::slotExit( )
{
    QSettings settings("ZETiIS","FIVER2");
    settings.setValue("/RSoft/FViewer/FiverMainGeometry",saveGeometry());
    close();
}

void FiverMain::slotBoxSelected( BoxObj * obj )
{
	FVObject * fo = (FVObject*) obj;
	if (obj == 0) {
		qDebug("Slot selected obj = 0.");
	} else {
		fvAttribs->setModel( fo->getModel() );
		getCurrentViewer()->setInteractionModel( fo->getInteractionModel() );
		getCurrentViewer()->update();

                editMenu->clear();
                editMenu->addActions( obj->getContextMenuObj()->actions());
	}
}

void FiverMain::slotApplyAttribs( )
{
	FVObject * fo = (FVObject*) bm->getSelected();
	if (fo != 0) {
		fo->updateAttributes();
	}
}

void FiverMain::slotFIVERSettings( )
{
	GUIAttributes * ga = new GUIAttributes(this);        
	ga->exec();
}

void FiverMain::slotArrangeBoxes( )
{
	FVObject * fo = (FVObject*) bm->getSelected();
	if (fo != 0) {
		bm->autoArrangeChildren( fo );
                bm->update();
	}
}

void FiverMain::slotSaveViewState()
{
	QString fname = QFileDialog::getSaveFileName(this, tr("Save state file"));
	if (fname.length() > 0) {
		getCurrentViewer()->setStateFileName( fname );
		getCurrentViewer()->saveStateToFile();
		getCurrentViewer()->setStateFileName( QString::null );
	}
}

void FiverMain::slotRestoreViewState()
{
	QString fname = QFileDialog::getOpenFileName(this, tr("Open state file"));
	if (fname.length() > 0) {
		getCurrentViewer()->setStateFileName( fname );
		getCurrentViewer()->restoreStateFromFile();
		getCurrentViewer()->setStateFileName( QString::null );
	}
}

void FiverMain::slotShowEntireScene()
{
    getCurrentViewer()->showEntireScene();
}


void FiverMain::slotMacroEditor()
{
#ifndef NO_QCONSOLE
    if (console == 0) {
        console = new QConsole();
        FViewerInjector *injector = new FViewerInjector();
        console->introduce(injector);
        console->inject((void*) getCurrentViewer(), "fviewer",
             injector);

    }

    console->show();
#endif
}


void FiverMain::slotSphere()
{
    bm->addBoxSphere( tr("Sphere") );
}

void FiverMain::slotCoil()
{
    bm->addBoxCoil( tr("Coil") );
}

void FiverMain::slotMeshJoiner()
{
    bm->addMeshJoiner( tr("Mesh Joiner") );
}

void FiverMain::slotMagellan(float x, float y, float z, float A, float B, float C)
//void FiverMain::slotMagellan(float , float , float , float , float , float )
{

	qglviewer::Camera * c = getCurrentViewer()->camera();
	qglviewer::Vec pos = c->position();
	
	// translate the camera
	pos = pos + c->viewDirection() * c->sceneRadius() / 360 * (-z) * 0.1;
	pos = pos + c->rightVector() * c->sceneRadius() / 360 * (-x) * 0.1;
	pos = pos + c->upVector() * c->sceneRadius() / 360 * (-y) * 0.1;
		
	qglviewer::Vec dir;
	qglviewer::Vec up;	
	qglviewer::Vec right;	
	
	// Rotate around Up vector
	up = c->upVector();
	dir = c->viewDirection();
    pos =  rotate (pos.x, pos.y, pos.z, up.x, up.y, up.z, - B / 360 * 0.01 * M_PI);
	dir = rotate (dir.x, dir.y, dir.z, up.x, up.y, up.z, - B / 360 * 0.01 * M_PI);	
	c->setPosition( pos );
	c->setViewDirection(dir);		
	
	// Rotate around viewDirection vector
	dir = c->viewDirection();
	up = c->upVector();
	up = rotate (up.x, up.y, up.z, dir.x, dir.y, dir.z, -C / 360 * 0.01 * M_PI);	
	c->setUpVector(up);
	
	// Rotate around rightVector vector
	up = c->upVector();
	dir = c->viewDirection();
	right = c->rightVector();
	pos = rotate (pos.x, pos.y, pos.z, right.x, right.y, right.z, - A / 360 * 0.01 * M_PI);	
	dir = rotate (dir.x, dir.y, dir.z, right.x, right.y, right.z, - A / 360 * 0.01 * M_PI);	
	c->setPosition( pos );
	c->setViewDirection(dir);		
	
	
	
	getCurrentViewer()->update();

}

qglviewer::Vec FiverMain::rotate(float px, float py, float pz, float sx, float sy, float sz, float angle) {
    double P[3]= {px,py,pz};
    double S[3] = {sx, sy, sz};

    Helpers::rotate(P,S,angle);

    qglviewer::Vec res;
    res.setValue(P[0], P[1], P[2]);

    return res;
}

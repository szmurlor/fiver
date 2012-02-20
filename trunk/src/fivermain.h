/***************************************************************************
 *   Copyright (C) 2006 by Robert Szmurlo   *
 *   robert@iem.pw.edu.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef FIVER2_H
#define FIVER2_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMenu>

#include <fvopen.h>
#include <boxobj.h>
#include <fviewer.h>
#include <fvboxmgr.h>
#include <fvattribs.h>

#ifndef NO_QCONSOLE
    #include <qconsole.h>
#endif

/** Global Functions, which give access to current viewer.
    Currently in FIVER there is only one active viewer.
    Those functions are provided for future compatibility.
*/
FViewer * getCurrentViewer();
extern FViewer * __current_FViewer;

#define __FIVER_NAME__ "FIVER 2.1.11"


/**
	MainFiver class - main window of the application.
    Responsibilities:
	 - menu managment
	 - creation and full control of the viewer
	 - control of the Navigator and Attributes widgets
*/
class FiverMain: public QMainWindow
{
  Q_OBJECT

public:
	FiverMain();
	~FiverMain();

public slots:
	/** Navigator and Attributes related slots: */
	void slotBoxSelected( BoxObj * obj );
	void slotApplyAttribs();
	
	/** Menu related slots: */
        void slotFIVERSettings();
        void slotOpen();
        void slotSaveAs();
	void slotExit();
	void slotArrangeBoxes();
	void slotSaveViewState();
	void slotRestoreViewState();
        void slotShowEntireScene();
        void slotMacroEditor();
        void slotSphere();
        void slotCoil();
        void slotMeshJoiner();
	
	void slotMagellan(float,float,float,float,float,float);

protected:
	
	/** Application menu entries. */
	QMenu * fileMenu;
        QMenu * editMenu;
        QMenu * insertMenu;
	QMenu * settingsMenu;
	QMenu * layoutMenu;
	
	/** Navigator of objects in memory.	*/
	QDockWidget * dwNavi;
	FVBoxMgr *bm;
	
	/** Atributes widget. */
	QDockWidget * dwAttr;
    FVAttribs * fvAttribs;
	
    /** Console widget. */
    QDockWidget * dwConsole;

    /** Controller of file openning procedures */
	FVOpen * fvOpen; 

#ifndef NO_QCONSOLE
    QConsole * console;
#endif
	/** Build application menu. */
	void buildMenu();
	
	void setupUI();

    qglviewer::Vec rotate(float px, float py, float pz, float sx, float sy, float sz, float angle);
};

#endif

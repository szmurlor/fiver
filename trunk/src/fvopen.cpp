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
#include "fvopen.h"
#include <QFileDialog>
#include <QtDebug>
#include <QMessageBox>
#include <fvopenerdiffpackgrid.h>
#include <fvopenerdiffpacksim.h>
#include <fvopenercblock.h>
#include <fvopenervtkfile.h>
#include <fvopenerstlfile.h>
#include <fvopenercoil.h>
#include <main.h>
#include <fvopenerObjFile.h>
#include <fvopenerFiverProj.h>



#define DIFFGRID "1 - Diffpack grid file (*.grid)"
#define DIFFSIM "2 - Diffpack simulation (*.dp)"
#define CBLOCK "3 - CBlock (*)"
#define VTKFILE "4 - VTKFile (*.pvd)"
#define STLFILE "5 - STLFile (*.stl)"
#define COIL "6 - Coil (*.cl)"
#define OBJFILE "7 - Obj file (*.obj)"
#define FIVERPROJ "8 - Fiver project (*.fvp)"

#define FIVERPROJSAVE "1 - Fiver project (*.fvp)"

FVOpen::FVOpen(QObject * parent)
 : QObject(parent)
{
    // Here we have to register known formats.
    filters[tr(DIFFGRID)] = new FVOpenerDiffpackGrid();
    filters[tr(DIFFSIM)] = new FVOpenerDiffpackSim();
    filters[tr(CBLOCK)] = new FVOpenerCBlock();
    filters[tr(VTKFILE)] = new FVOpenerVTKFile();
    filters[tr(STLFILE)] = new FVOpenerSTLFile();
    filters[tr(COIL)] = new FVOpenerCoil();
    filters[tr(OBJFILE)] = new FVOpenerObjFile();
    filters[tr(FIVERPROJ)] = new FVOpenerFiverProj();

    saveFilters[tr(FIVERPROJSAVE)] = new FVOpenerFiverProj(true);
}


FVOpen::~FVOpen()
{
}

int FVOpen::exec(FVBoxMgr * bm, bool saveMode)
{
	QStringList files;
	QString file;
	
	QString startDir (fvsettings.value("/RSoft/FViewer/RecentDir","~").toString());
    QString lastFilter;
    if( !saveMode )
        lastFilter = fvsettings.value("/RSoft/FViewer/LastFilter",filters.keys().first()).toString();
    else
        lastFilter = saveFilters.keys().front();

        //qDebug() << "Start dir = " << startDir;
        //qDebug() << "Last filter = " << lastFilter;
	
	QFileDialog od(bm);

    if( !saveMode )
        od.setFilters( filters.keys() );
    else
    {
        od.setFilters( saveFilters.keys() );
        od.setAcceptMode( QFileDialog::AcceptSave );
        //TODO należy dorobić żeby się dopisywało automatycznie rozszerzenie pliku jesli użytkownik nie wpisał w box
    }


	od.setModal(true);
	od.setDirectory( startDir );
	od.selectNameFilter(lastFilter );
	if (od.exec()) {
		files = od.selectedFiles();
		lastFilter = od.selectedFilter();
		
		foreach (file, files) {
            openFile( bm, file, lastFilter, saveMode );
		}	
	}

    if (file.length() > 0) {
                // qDebug() << "Saving: " << file.section('/',0,-2);
		fvsettings.setValue("/RSoft/FViewer/RecentDir",file.section('/',0,-2));
                // qDebug() << "Saving: " << lastFilter;
        if( !saveMode )
            fvsettings.setValue("/RSoft/FViewer/LastFilter",lastFilter);
		fvsettings.sync();
	}

	return 0;
}


int FVOpen::openFile( FVBoxMgr * bm, QString fname, QString type, bool saveMode )
{
	FVOpener * o;

    if( !saveMode )
        o = filters[type];
    else
        o = saveFilters[type];
        qDebug() << "TYPE=" << type;
	if (o != 0) {
		o->open(bm,fname,0);
		return 0;
	}
	
	QMessageBox::critical(bm, tr("Incorrect file type."), tr("Unsupported file type: ") + type);
	
        return -1;
}


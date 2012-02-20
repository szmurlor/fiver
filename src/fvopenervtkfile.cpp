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
#include "fvopenervtkfile.h"
#include <QtDebug>
#include <fvboxsimulation.h>
#include <fvboxmgr.h>
#include <vtkfile.h>
#include <field.h>
#include <grid.h>
#include <boxbox.h>

FVOpenerVTKFile::FVOpenerVTKFile( )
    : FVOpener(0)
{
}

FVOpenerVTKFile::FVOpenerVTKFile(QObject * parent)
    : FVOpener(parent)
{
}


FVOpenerVTKFile::~FVOpenerVTKFile()
{
}

FVObject* FVOpenerVTKFile::open( FVBoxMgr * bm,  QString fname, int  )
{
    FVBoxGrid *bg;
    unsigned int i;

    qDebug() << "Opening VTK File: " << fname;

    FVBoxSimulation * bs = bm->addBoxSimulation( fname, tr( "VTKFile" ) );

    VTKFile * f = new VTKFile();
    f->loadFromFile( fname.toStdString() );
    vector< Grid * > grids = f->getGrids();
    vector< Field * > fields = f->getFields();
    vector< FVBoxGrid *> boxGrids;

    for (i = 0; i < grids.size(); i++) {
        boxGrids.push_back( bm->addBoxGrid( bs, grids[i], fname, tr("VTKGrid") ) );
    }

    for (i = 0; i < fields.size(); i++) {
        QString sGridRefNum = fields[i]->getAttr("gridRefNum").c_str();
        bg = boxGrids[sGridRefNum.toInt()-1];
        bm->addBoxField( (FVObject*)bg, fields[i], QString("%1").arg(fields[i]->getAttr("name").c_str()), QString("VTKField") );
    }

    bm->autoArrangeChildren( bs );

    return bs;
}




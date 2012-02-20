/***************************************************************************
 *   Copyright (C) 2011 by Robert Szmurlo   *
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
#include "fvopenerstlfile.h"
#include <QtDebug>
#include <stlfile.h>
#include <fvboxmgr.h>
#include <grid.h>
#include <boxbox.h>
#include <fvboxgrid.h>
#include <fvboxstl.h>

FVOpenerSTLFile::FVOpenerSTLFile( )
    : FVOpener(0)
{
}

FVOpenerSTLFile::FVOpenerSTLFile(QObject * parent)
    : FVOpener(parent)
{
}


FVOpenerSTLFile::~FVOpenerSTLFile()
{
}

FVObject* FVOpenerSTLFile::open( FVBoxMgr * bm,  QString fname, int  )
{
    qDebug() << "Opening STL File: " << fname;

    STLFile * f = new STLFile();
    Grid * grid = f->loadFromFile( fname.toStdString() );
    FVBoxSTL * boxSTL =  bm->addBoxSTL(0, grid, fname, tr("STLGrid") );

    return boxSTL;
}




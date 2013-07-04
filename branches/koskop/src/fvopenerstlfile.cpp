/*****************************************************************************
 *   Copyright (C) 2011 Robert Szmurlo <robert@iem.pw.edu.pl>                *
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




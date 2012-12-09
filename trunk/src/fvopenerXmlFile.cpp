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

#include "fvopenerXmlFile.h"
#include <QtDebug>
#include <stlfile.h>
#include <fvboxmgr.h>
#include <grid.h>
#include <boxbox.h>
#include <fvboxgrid.h>
#include <fvboxXml.h>
#include <fvboxstl.h>
#include <dolfin/mesh/Mesh.h>

FVOpenerXmlFile::FVOpenerXmlFile( )
    : FVOpener(0)
{
}

FVOpenerXmlFile::FVOpenerXmlFile(QObject * parent)
    : FVOpener(parent)
{
}


FVOpenerXmlFile::~FVOpenerXmlFile()
{
}

FVObject* FVOpenerXmlFile::open( FVBoxMgr * bm,  QString fname, int  )
{
    qDebug() << "Opening XML File: " << fname;
    dolfin::Mesh *m =  new dolfin::Mesh(fname.toStdString());

//    std::cout << "geometry" << mesh.geometry().str(true) << std::endl;
//    std::cout << "topology" << mesh.topology().str(true) << std::endl;
//    std::cout << "data" << mesh.data().str(true) << std::endl;


    FVBoxXml * boxXML =  bm->addBoxXml(0, m, fname, tr("XMLGrid") );

    return boxXML;
}




#include "fvopenerObjFile.h"
#include <QtDebug>
#include <objFile.h>
#include <fvboxmgr.h>
#include <grid.h>
#include <boxbox.h>
#include <fvboxgrid.h>

FVOpenerObjFile::FVOpenerObjFile( )
    : FVOpener(0)
{
}

FVOpenerObjFile::FVOpenerObjFile(QObject * parent)
    : FVOpener(parent)
{
}


FVOpenerObjFile::~FVOpenerObjFile()
{
}

FVObject* FVOpenerObjFile::open( FVBoxMgr * bm,  QString fname, int  )
{
    qDebug() << "Opening Obj File: " << fname;
    ObjFile * f = new ObjFile();
    Grid * grid = f->loadFromFile( fname.toStdString() );
    FVBoxGrid * boxGrid =  bm->addBoxGrid( 0, grid, fname, tr("ObjGrid") );
    qDebug() << "File loaded.\n";
    return boxGrid;
}




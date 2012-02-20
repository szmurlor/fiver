#include "fvopenerFiverProj.h"
#include <QtDebug>
#include <fvboxmgr.h>
#include <grid.h>
#include <boxbox.h>
#include <fvboxgrid.h>
#include "fvBoxExporter.h"

FVOpenerFiverProj::FVOpenerFiverProj( bool saveMode )
    : FVOpener(0)
{
    this->saveMode = saveMode;
}

FVOpenerFiverProj::FVOpenerFiverProj(QObject * parent, bool saveMode)
    : FVOpener(parent)
{
    this->saveMode = saveMode;
}

FVOpenerFiverProj::~FVOpenerFiverProj()
{
}

FVObject* FVOpenerFiverProj::open( FVBoxMgr * bm,  QString fname, int  )
{

    if( saveMode )
    {
        qDebug() << "Saving Fiver Project File: " << fname;

        FVBoxExporter exporter;
        if( !exporter.exportFromBoxMgr(bm, fname) )
        {
            qDebug() << "Project export ERROR\n";
            return NULL;
        }

        qDebug() << "Project file exported.\n";
    }
    else
    {
        qDebug() << "Opening Fiver Project File: " << fname;

       /* if( !importToBoxMgr(bm, fname) )
        {
            qDebug() << "Project import ERROR\n";
            return NULL;
        }
*/
        qDebug() << "Project file loaded.\n";
    }
    return NULL;
}

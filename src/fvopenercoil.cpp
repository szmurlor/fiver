#include "fvopenercoil.h"
#include <QtDebug>
#include <fvboxcoil.h>
#include <QMessageBox>

FVOpenerCoil::FVOpenerCoil( )
: FVOpener(0)
{
}

FVOpenerCoil::FVOpenerCoil(QObject * parent)
 : FVOpener(parent)
{
}


FVOpenerCoil::~FVOpenerCoil()
{
}

FVObject* FVOpenerCoil::open( FVBoxMgr * bm, QString fname, int type )
{
        bm = bm;
        fname = fname;
        type = type;

        qDebug() << "Opening Coil: " << fname;

        FVBoxCoil *coil = bm->addBoxCoil( "Coil" );
        if (coil->loadFromFile( fname ) == false) {
            printf("It is possible that the file has wrong format:\n");
            QMessageBox::warning(bm,tr("Coil opening warning."), tr("It is possible that the file has wrong format: %1.").arg(fname));
        }


    return coil;
}


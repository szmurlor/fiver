#ifndef _SIMUMGR_H
#define _SIMUMGR_H

#include <QString>

#include <grid.h>
#include <field.h>
#include <simu.h>

#include "vtkreader.h"


class VTKFile : public Simu {

protected:
    void readVTKDataSet( QString fileName, string name );

public:


    VTKFile( int argc, char ** argv );
    VTKFile();
    virtual ~VTKFile();

    virtual void loadFromArgv( int argc, char ** argv );
    virtual void loadFromFile( string fileName );

    virtual void saveSimu( char * fileName );


};

#endif

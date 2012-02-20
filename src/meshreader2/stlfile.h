#ifndef STLFILE_H
#define STLFILE_H

#include <grid.h>
#include <field.h>
#include <simu.h>
#include <QtDebug>

#include <cmath>

//#include "vtkreader.h"


class STLFile {

public:
    STLFile();
    virtual ~STLFile();

    int findEqual ( double x, double y, double z, Grid* grid,int nodesCounter);
    virtual Grid * loadFromFile( string fileName );
    int countElements ( string fileName );

};

#endif // STLFILE_H

#ifndef FVBOXSTL_H
#define FVBOXSTL_H

#include <fvobject.h>
#include <fvboxgrid.h>

#include <boxbox.h>
#include <fvboxmgr.h>
#include <boxcon.h>
#include <grid.h>
#include <attrmodel.h>
#include <fvgridinterface.h>
#include <fvinteractiontranslation.h>
#include <fvinteractionrotation.h>

class FVBoxSTL : public FVBoxGrid
{
    Q_OBJECT

public:

        FVBoxSTL(FVBoxMgr * manager, Grid * g, int x = 0, int y = 0);
        virtual ~FVBoxSTL();

        virtual QString classType() { return QString("FVBoxSTL"); }
};

#endif // FVBOXSTL_H

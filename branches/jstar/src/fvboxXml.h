#ifndef FVBOXXML_H
#define FVBOXXML_H

#include <fvobject.h>
#include <fvboxmesh.h>

#include <boxbox.h>
#include <fvboxmgr.h>
#include <boxcon.h>
//#include <grid.h>
#include <dolfin/mesh/Mesh.h>
//#include <attrmodel.h>
#include <fvgridinterface.h>
#include <fvinteractiontranslation.h>
#include <fvinteractionrotation.h>

class FVBoxXml : public FVBoxMesh
{
    Q_OBJECT

public:

        FVBoxXml(FVBoxMgr * manager, dolfin::Mesh * m, int x = 0, int y = 0);
        virtual ~FVBoxXml();

        virtual QString classType() { return QString("FVBoxXml"); }
};

#endif // FVBOXXML_H

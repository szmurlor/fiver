#include "fvboxXml.h"
#include <fivermain.h>
#include <main.h>

FVBoxXml::FVBoxXml( FVBoxMgr * manager,  dolfin::Mesh * m, int x, int y )
 : FVBoxMesh(manager,m,x,y)
{
        cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_GRID ).value<QColor>();

        setupAttributes();
        setupMenu();

        rRect.setWidth( 150 );
}


FVBoxXml::~FVBoxXml()
{
}




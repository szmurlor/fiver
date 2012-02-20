#include "fvboxstl.h"
#include <fivermain.h>
#include <main.h>

FVBoxSTL::FVBoxSTL( FVBoxMgr * manager,  Grid * g, int x, int y )
 : FVBoxGrid(manager,g,x,y)
{
        cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_GRID ).value<QColor>();

        //setupAttributes();
        //setupMenu();

        rRect.setWidth( 150 );
}


FVBoxSTL::~FVBoxSTL()
{
}



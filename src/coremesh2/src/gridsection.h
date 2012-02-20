#ifndef _GRIDSECTION_IS_INCLUDED
#define _GRIDSECTION_IS_INCLUDED

#include "grid.h"

class GridSection {
public:
	GridSection();
	~GridSection();
	
	void section( Grid * srcGrid, Grid * dstGrid );
};

#endif


#ifndef _FIELDSECTION_IS_INCLUDED
#define _FIELDSECTION_IS_INCLUDED

#include "grid.h"
#include "simplefield.h"

class FieldSection {
public:
	FieldSection();
	~FieldSection();
	
	void setParams( double aP[3], double aN[3] );
	
	void section(Grid * srcGrid, SimpleField * srcField, Grid * dstGrid, SimpleField * dstField);
	
protected:
	double P[3];
	double N[3];
};


#endif


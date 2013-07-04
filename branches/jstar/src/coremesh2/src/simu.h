#ifndef _SIMU_IS_INCLUDED
#define _SIMU_IS_INCLUDED

#include "grid.h"
#include "simplefield.h"
#include "field.h"

/** Container
*/
class Simu {

protected:
	vector<Field*> fields;
	vector<Grid*> grids;

public:
	Simu();
	virtual ~Simu();

	virtual void loadFromFile( string  ) {};
	virtual void loadFromArgv( int , char **  ) {};
	virtual void saveSimu( string  ) {};

	vector<Field*> getField( string name );
	vector<Field*> getFields();
	
	vector<Grid*> getGrids();
	
	Field* getField( int idx = 0 );
	Grid* getGrid(int idx = 0);
	
	Field * newField();
	Grid * newGrid();
	
};

#endif

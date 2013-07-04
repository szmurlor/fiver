#ifndef _SIMUMGR_H
#define _SIMUMGR_H

#include <grid.h>
#include <field.h>
#include "diffreader.h"


class SimuMgr {

protected:
	DpFieldFileInfo dffi;
	DpFieldReader dfr;
	DpGridFileInfo dgfi;
	DpReader dgr;

	vector<Field*> fields;
	vector<Grid*> grids;
public:
	

	SimuMgr( int argc, char ** argv );
	SimuMgr();
	~SimuMgr();

	void analyzeArgv( int argc, char ** argv );

	vector<Field*> getField( char * name );
	vector<Field*> getFields();
	Field* getField( int idx = 0 );
	Grid* getGrid(int idx = 0);
	
	Field * newField();
	Grid * newGrid();
	
	/** This will create the 3 files:
		simName.dp
		.simName.grid
		.simName.field
	*/
	void saveSimu( char * simName );
};

#endif

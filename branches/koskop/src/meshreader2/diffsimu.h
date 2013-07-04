#ifndef _SIMUMGR_H
#define _SIMUMGR_H

#include <grid.h>
#include <field.h>
#include <simu.h>

#include "diffreader.h"


class DiffSimu : public Simu {

protected:
	DpFieldFileInfo dffi;
	DpFieldReader dfr;
	DpGridFileInfo dgfi;
	DpReader dgr;

public:
	

	DiffSimu( int argc, char ** argv );
	DiffSimu();
	virtual ~DiffSimu();

	virtual void loadFromArgv( int argc, char ** argv );
	virtual void loadFromFile( string simName );

	/** This will create the 3 files:
		simName.dp
		.simName.grid
		.simName.field	*/
	virtual void saveSimu( char * simName );


};

#endif

#include "simu.h"

Simu::Simu()
{
}

Simu::~Simu()
{
	for (unsigned int i = 0; i < fields.size(); i++)
		delete fields[i];
	
	for (unsigned int i = 0; i < grids.size(); i++)
		delete grids[i];
}

vector<Field*> Simu::getFields()
{
	return fields;
}

vector< Grid * > Simu::getGrids( )
{
	return grids;	
}

vector<Field*> Simu::getField( string name )
{
	vector<Field*> tmp;
	for (unsigned int i = 0; i < fields.size(); i++) {
		if ( fields[i]->name() == name) 
			tmp.push_back( fields[i] );
	}
	
	return tmp;
}

Field* Simu::getField( int idx )
{
	if (fields.size() < (unsigned int) idx) {
		return fields[idx];
	} else {
		return 0;
	}
}

Grid* Simu::getGrid(int idx)
{
	if (grids.size() > (unsigned int) idx)
		return grids[idx];
	else
		return 0;
}


Field * Simu::newField()
{
	fields.push_back( new Field() );
	return fields.back();
}

Grid * Simu::newGrid()
{
	grids.push_back( new Grid() );
	return grids.back();
}






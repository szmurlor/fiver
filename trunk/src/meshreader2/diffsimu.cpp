#include "diffsimu.h"
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

DiffSimu::DiffSimu( int argc, char ** argv )
{
	loadFromArgv( argc, argv );
}

DiffSimu::DiffSimu()
{
}
  
DiffSimu::~DiffSimu()
{
}

void buildName( char * fn, const char * simName, const char * extension )
{
	char * pos;
	char tmp[1024];

	strcpy(fn,"");
	strcpy(tmp,simName);
	pos = strrchr(tmp,'.');
	if (pos != NULL) {
		*pos = 0;
	}
	strcat(tmp,extension);

	pos = strrchr(tmp,'/');
	if (pos != NULL) {
		*pos = 0;
		strcat(fn,tmp);
		strcat(fn,"/");
		strcat(fn,".");
		strcat(fn,++pos);
	} else {
		strcat(fn,".");
		strcat(fn,tmp);
	}
	
}

void DiffSimu::loadFromFile( string simName )
{
	Grid * grid;
	Field * field;
	int i;

	char gridfn[1024];
	char fieldfn[1024];

        setlocale( LC_ALL, "C");

	buildName(gridfn, simName.c_str(), ".grid");
	buildName(fieldfn, simName.c_str(), ".field");


	// printf("Reading grid file info: %s\n", gridfn);
	// printf("Reading field file info: %s\n", fieldfn);

	dgfi.ReadFromFile( gridfn );
	dffi.ReadFromFile( fieldfn );

	for (i=0; i < dgfi.count(); i++) {
		grid = newGrid();
		// printf("Reading grid: %s (idx = %d)\n", gridfn, i);
		dgr.ReadFromFile(grid, gridfn, dgfi.getInfo( i )->position) ;
		// printf("Completed reading grid.\n");
	}

	vector<string> fields;
	vector<string> times;

	fields = dffi.getFieldNames();
	
	for (unsigned int i=0; i < fields.size(); i++) {
		times = dffi.getFieldTimes( fields[i] );
		field = newField();
		for (unsigned int j = 0; j < times.size(); j++) {
                        //printf("Reading field: %s (idx = %d, name = %s, time = %s)\n", fieldfn, i, fields[i].c_str(), times[j].c_str());
                        SimpleField * sfield = new SimpleField();
			dfr.ReadFromFileName( sfield, fieldfn, &dffi, fields[i], times[j] );
			double t = atof( times[j].c_str() );
                        //printf("t = %lf\n",t);
			field->add( sfield, t );
			field->setAttr("gridRefNum", sfield->getAttr("gridRefNum"));
			field->setAttr("name", fields[i]);
			//field->name( dffi.getInfo( i )->name );
			//printf("SimpleField size = %d\n", sfield->size() );
			//printf("SimpleField min = %lf\n", sfield->min() );
			//printf("Added %s field %lf with maximum value = %lf, and minimum value = %lf.\n", fields[i].c_str(), t, field->min(), field->max());
		}
	}
}

void DiffSimu::loadFromArgv( int argc, char ** argv )
{
	unsigned int i =0;
	int ignr = 0;
	Grid * grid;
	
	while ( i< (unsigned int) argc ) {
		if (strcmp( argv[i], "-g" ) == 0) {
			ignr = atoi( argv[++i] );
		}
		
		if (strcmp( argv[i], "-gn" ) == 0) {
			char * fname = argv[++i];
			
			grid = newGrid();
					
			// printf("Reading grid: %s (idx = %d)\n", fname, ignr);
			dgfi.ReadFromFile( fname );
			dgr.ReadFromFile(grid, fname, dgfi.getInfo( ignr )->position) ;
			// printf("Completed reading grid.\n");
		}
		
		if (strcmp( argv[i], "-f" ) == 0) {
			char * fname = argv[++i];
			int nf = atoi( argv[++i] );
			
			dffi.ReadFromFile( fname );
			
			// printf("Reading fields from file: %s\n", fname );
			while (nf-- > 0) {
				int idx = atoi( argv[++i] );
				Field * field = newField();
                                SimpleField * sfield = new SimpleField();
				
				// printf("Reading field (%d of %d): (%d) \n", tnf - nf, tnf, idx);
				dfr.ReadFromFile( sfield, fname, dffi.getInfo( idx )->position );
				field->add(sfield);
				field->name( dffi.getInfo( idx )->name );
				// printf("Completed reading field.\n");
			}
		}
		
		i++;
	}
}

void DiffSimu::saveSimu( char * simName )
{
	char s[1024];
	unsigned int i;
	
	///// Save simulation file.
	strncpy(s,simName,1020);
	strcat(s,".dp");
	FILE * _f = fopen(s, "w");
	if (_f != NULL) {
		fprintf(_f,"Case %s\n",simName);
		fclose(_f);
	}
	
	// Save file with grids (all).
	strcpy(s,".");
	strncat(s,simName,1017);
	strcat(s,".grid");	
	dgr.bSaveIxFile = true;
	if (grids.size() > 0) {
		if (dgfi.count() >0) 
			dgr.SaveToFile(grids[0], s, dgfi.getInfo(1));
		else
			dgr.SaveToFile(grids[0], s, dgfi.getInfo(1));
		
		for (i=1; i < grids.size(); i++) {
			if (dgfi.count() > (int) i)
				dgr.AppendToFile( grids[i], s, dgfi.getInfo(i+1));
			else
				dgr.AppendToFile( grids[i], s );
		}
	}
		
	// Save file with fields (all)
	strcpy(s,".");
	strncat(s,simName,1016);
	strcat(s,".field");
	dfr.bSaveIxFile = true;
	printf("saving to implement!!!!! \n");
/*
	if (fields.size()>0) {
		if (dffi.size()>0) 
			dfr.SaveToFile(fields[0], s, dffi.getInfo(1));
		else
			dfr.SaveToFile(fields[0], s);
		for (i=1; i < fields.size(); i++) {
			if (dffi.size() > (int) i)
				dfr.AppendToFile( fields[i], s, dffi.getInfo(i+1));
			else
				dfr.AppendToFile( fields[i], s );
		}
	}
*/
}

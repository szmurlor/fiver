/* Author: Robert Szmurlo
 * created: 2005.01... 
 */ 


#include <fieldenergy.h>
#include <simplefield.h>
#include <grid.h>
#include <diffreader.h>
#include <iostream>
#include <vector>

#define ENERGY 1
#define POWER 2
#define FLUXDENSITY 1
#define FIELDINTENSITY 2

bool DEBUG;

using namespace std;

void printVersion()
{
	cout << "$Revision: 1.2 $" << endl;
	cout << "Author: Robert Szmurlo (szmurlor@iem.pw.edu.pl)" << endl;
}

void printHelp(char ** argv)
{
	cout << endl;
	cout << "Calculates field energy or power in subdomains and in total model." << endl;
	cout << "Requires grid and field in Diffpack format."<< endl;
	cout << "Requires material description file in EsFEM format with modification:"<< endl;
	cout << "Th eline where material defintion starts must begin with word: 'mat>'"<< endl << endl;
	cout << "usage: "<<argv[0] <<" fname.grid fname.field material.dat [-g g_nr] [-f f_count f1 ..] [-s count nr1 nr2...] [-m mat_col] [-N isub] [-PWdFIvh]"<<endl;
	cout << "   fname.grid   - file with grid specification" << endl;
	cout << "   fname.field  - file with field values" << endl;
	cout << "   material.dat - file with material description in EsFEM format," << endl;
	cout << "                  line with definition must begin: mat> nr_of_materials," << endl;
	cout << "   -g g_nr      - index of grid to be used for calculations" << endl;
	cout << "   -f f_nr      - index of field to be used for calculations" << endl;
	cout << "   -m mat_col   - column in material.dat file to be used as material" << endl;
	cout << "                  (by defualt second column is taken for calculations)" << endl;
	cout << "   -s s_nr n1.. - list of subdomains where energy will be integrated" << endl;
	cout << "   -P (default) - force the calculation of power ( p = (E * J) )" << endl;
	cout << "   -W           - force the calculation of energy ( w = 0.5 * (E * D) )" << endl;
	cout << "   -F (defualt) - indicates that field values in file are flux density ( w = 0.5 * (D * D) / mat )" << endl;
	cout << "   -I           - indicates that field values in file are field intensity ( w = 0.5  * (E * E) * mat )" << endl;
	cout << "   -d           - turn on debugging mode" << endl;
	cout << "   -v           - prints information about version" << endl;
	cout << "   -h           - prints this option help" << endl;
	cout << "   -N isub      - calibrate (scale) the flux density or field intensity to maximum value in isub" << endl;
	cout << "                  (if isub == 0 normalize to maximum value in total model) " << endl;
	cout << "                  calibration is calculated from equation: E_new = E / Emax" << endl;
	cout << "   -Nval fval   - calibrate (scale) the flux density or field intensity to given value 'fval'" << endl;
	cout << "                  (each field value is recalculated with the following fomula: newE = old E / fval " << endl;
	cout << "                  fval is considered as module (length of vector) of E (or D)" << endl;
	cout << "                  The exact equation for density of energy is thus: w = gamma*|E|^2 / (2 * fval ^ 2)" << endl;
	cout << "   -EX          - standard deviation of power of energy density (in selected subdomains or total model)" << endl;
	cout << "                  (EX = sqrt( sum( vol_elem_i * (p_elem_i - p_avg) ) / tot_vol )" << endl; 
	cout << "   -Over dval%  - calcluate the volume where power of energy density is more than 'dval' percent of maximum" << endl;
	cout << "example:" << endl;
	cout << "   meshenergy g1.grid .J.field head.data -g 1 -f 3 1 2 3 -s 1 1" << endl;
}

int main(int argc, char ** argv)
{
	DpGridFileInfo dgfi;
	DpReader dr;
	DpFieldFileInfo dffi;
	DpFieldReader dfr;
	FILE * fdat;
	SimpleField f[3];
	Grid g;
	char * fgname;
	char * ffname;
	char * fdname;
	int grid_idx = 1;
	int i;
	vector<int> field_idx(1);
	int mat_count;
	char line[256];
	int matno;
	float v1,v2;
	int iMatCol = 2;
	CFieldEnergy fe;

	
	//////////////////////////////////////////////////////
	// Analyzing command line parameters (see printHelp() 
	// for details on usage)
	//////////////////////////////////////////////////////

	DEBUG = false;
	field_idx[0] = 1;
	for (i = 0; i < argc; i++) {
		if (strcmp( argv[i], "-g") == 0)
			grid_idx = atoi(argv[++i]);
		if (strcmp( argv[i], "-f") == 0) {
			field_idx.resize( atoi( argv[++i] ) );
			for (int j = 0; j < field_idx.size(); j++)
				field_idx[j] = atoi( argv[++i] );
		}
		if (strcmp( argv[i], "-s") == 0) {
			fe.sub_idx.resize( atoi(argv[++i]) );
			for (int j = 0; j < fe.sub_idx.size(); j++)
				fe.sub_idx[j] = atoi( argv[++i] );
		}
		if (strcmp( argv[i], "-W") == 0) 
			fe.calcType = ENERGY;
		if (strcmp( argv[i], "-P") == 0) 
			fe.calcType = POWER;
		if (strcmp( argv[i], "-d") == 0) {
			DEBUG = true;
			fe.DEBUG = true;
		}
		if (strcmp( argv[i], "-F") == 0) 
			fe.fieldType = FLUXDENSITY;
		if (strcmp( argv[i], "-I") == 0) 
			fe.fieldType = FIELDINTENSITY;
		if (strcmp( argv[i], "-m") == 0) 
			 iMatCol = atoi(argv[++i]);
		if (strcmp( argv[i], "-v") == 0) {
			printVersion();
			exit(0);
		}
		if ((strcmp( argv[i], "-h") == 0) || (strcmp( argv[i], "--help") == 0)) {
			printHelp(argv);
			exit(0);
		}
		if (strcmp( argv[i], "-Over") == 0) {
			fe.dOver = atof( argv[++i] );
			fe.bOver  = true;
		}
		if (strcmp( argv[i], "-N") == 0) {
			if (fe.fNormVal != 0)
			  printf("Warning! -N and -Nval can not be ginvven together!\nUsing option -N!\n");
			fe.iNormSub = atoi(argv[++i]);
			fe.fNormVal = 0;
			fe.bNorm = true;
		}
		if (strcmp( argv[i], "-Nval") == 0) {
			if (fe.iNormSub != -1)
			  printf("Warning! -N and -Nval can not be ginvven together!\nUsing option -Nval!\n");
			 fe.fNormVal = atof(argv[++i]);
			 fe.iNormSub = -1;
			 fe.bNorm = true;
		}
			 
		if (strcmp( argv[i], "-EX") == 0) 
			fe.bEX = true;
	}

	if (argc < 3) {
		printHelp(argv);
		exit(-1);
	}

	fgname = argv[1];
	ffname = argv[2];
	fdname = argv[3];
	

	//////////////////////////////////////////////////////
	// Reading subdomains information from .dat file.
	// WARNING!!! Only isotropic materials!
	//////////////////////////////////////////////////////
	if ( (fdat=fopen(fdname,"r")) == NULL ) {
		printf("Unable to open file with materials description. This file must be in EsFEM format!\n");
		exit(-1);
	} else {
		fe.mat.resize(0);
		line[0] = 0;
		while ( (!feof(fdat)) && (strstr(line, "mat") == NULL) )
			fgets(line,255,fdat);
		if (strstr(line, "mat") != NULL) {
			sscanf(line,"mat>%d",&mat_count);
			if (DEBUG) printf("%d materials to read from dat file.\n", mat_count);

			for (i=0; i < mat_count; i++) {
				fscanf(fdat,"%f %f", &v1, &v2);
				if (iMatCol == 2)
					fe.mat.push_back( v2 );
				if (iMatCol == 1)
					fe.mat.push_back( v1 );
			}
		}
		fclose(fdat);
	}


	//////////////////////////////////////////////////////
	// Reading grid and fields section.
	//////////////////////////////////////////////////////
	if (DEBUG) printf("Scanning grid file...\n");
	dgfi.ReadFromFile( fgname );
	if (DEBUG) printf("Reading grid...\n");
	dr.ReadFromFile( &g, fgname, dgfi.getInfo( grid_idx )->position );
	if (DEBUG) printf("Reading grid finished.( Nodes = %d, Elems = %d)\n", 
	                  g._nodes.size(), g._elems.size() );


	if (DEBUG) printf("Scanning field file...\n");
	dffi.ReadFromFile( ffname );
	for (i=0; i<field_idx.size(); i++) {
		if (DEBUG) printf("Reading component %d of %d.\n", i+1, field_idx.size());
		dfr.ReadFromFile( (SimpleField*) &f[i], ffname, dffi.getInfo( field_idx[i] )->position );
	}
	if (DEBUG) printf("Successfully read field(s).\n");

	//////////////////////////////////////////////////////
	// Dumping some informations...
	//////////////////////////////////////////////////////

	if (DEBUG) printf("Number of fields: %d\n", field_idx.size());
	if (DEBUG) printf("Pointer to first field = %p\n", &f[i]);
	if (DEBUG) for (i=0; i < field_idx.size(); i++)
		printf("Number of field values[%d]: %d\n", i+1, f[i].size());

	if (fe.sub_idx.size() == 0) {
		if (DEBUG) printf("Selected all subdomains (the result should be same as total energy.\n");
	} else {
		if (DEBUG)
			for (i=0; i < fe.sub_idx.size(); i++)
				printf("Selected subdomain = %d\n", fe.sub_idx[i]); 
	}

	//////////////////////////////////////////////////////
	// Calculating total energy...
	//////////////////////////////////////////////////////
	if (field_idx.size() == 1) 
		fe.calculateScalar( &g, &f[0] );
	if (field_idx.size() == 3) 
		fe.calculate3D( &g, &f[0], &f[1], &f[2] );
	if ((field_idx.size() != 1) || (field_idx.size() != 3))
		printf("Currently only scalar and 3D fields are supported!\n");

	return(0);
}

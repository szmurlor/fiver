#include <grid.h>
#include <tri.h>
#include <elem.h>
#include <node.h>
#include <field.h>
#include <fieldsection.h>
#include <stdio.h>
#include <stdlib.h>
#include <gridreader.h>
#include <diffreader.h>

void printHelp()
{
	printf("slice name.grid Px Py Pz Nx Ny Nz name_out.grid field_in.field field_out.field \n");
}

int main( int argc, char ** argv )
{
	string fname;
	string ffin;
	string ffout;
	char * foutname;
	double P[3], N[3];
	
	if (argc < 11) {
		printHelp();
		return 1;
	}
	fname = argv[1];
	P[0] = (double) atof(argv[2]);
	P[1] = (double) atof(argv[3]);
	P[2] = (double) atof(argv[4]);
	N[0] = (double) atof(argv[5]);
	N[1] = (double) atof(argv[6]);
	N[2] = (double) atof(argv[7]);
	foutname = argv[8];
	
	ffin = argv[9];
	ffout = argv[10];
	
	DpGridFileInfo dgfi;
	DpReader dr;
	
	dgfi.ReadFromFile( fname );
	printf("Found %d grids. Reading the first one.", dgfi.count() );
	
	Grid sg;
	dr.ReadFromFile( &sg, fname, dgfi.getInfo( 0 )->position );
	
	DpFieldFileInfo dffi;
	DpFieldReader fr;
	
	printf("Scanning field file...\n");
	dffi.ReadFromFile( ffin );
	printf("Found %d fields.\n", dffi.count());
	printf("The first field is: %s\n", dffi.getInfo( 1 )->name.c_str() );
	Field field;
	printf("Starting reading field...\n");
	fr.ReadFromFileName( &field, ffin, &dffi, string("u"), string(""));
	
	printf("Finished reading grids and fields. Starting slicing...\n");
	FieldSection fs;
	fs.setParams( P, N );
	Grid dg;
	Field fieldOut;
	fs.section(&sg,&field,&dg,&fieldOut);

    printf("Finished slicing.\n");
    printf("Field.size() = %d\n", fieldOut.size());
    printf("Grid size = %d\n", dg._elems.size() );
	
	dr.SaveToFile( &dg, foutname );
	fr.SaveToFile( &fieldOut, ffout );
}

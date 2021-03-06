#include "vtkfile.h"
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <QFile>
#include <QDomDocument>
#include <QtDebug>
#include <iostream>

using namespace std;

VTKFile::VTKFile( int argc, char ** argv )
{
    loadFromArgv( argc, argv );
}

VTKFile::VTKFile()
{
}

VTKFile::~VTKFile()
{
}

void VTKFile::readVTKDataSet( QString fileName, string name ){
    Grid * grid;
    Field * field;
    char line[1024];
    int lineno;
    char *colno;

    //setlocale( LC_ALL, "C");

    FILE* in= fopen( fileName.toStdString().c_str(), "r" );
    if( in != NULL )
        cerr << "Reading VTK DataSet from " << fileName.toStdString() << "...";
    else {
        cerr << "Failed to open " << fileName.toStdString() << " for reading" << endl;
        return;
    }
    lineno= 0;
    while( !feof(in) && fgets(line,1024,in) != NULL && strstr(line,"<UnstructuredGrid>") == NULL )
        lineno++;
    if( feof(in) || fgets(line,1024,in) == NULL) {
        ERR: qWarning() << "Error while reading VTK DataSet file: " << fileName << " line=" << lineno << endl << line;
             return;
         }

    int np= 0, ne= 0;
    lineno++;
    if( (colno=strstr(line,"NumberOfPoints")) != NULL )
        sscanf( colno, "NumberOfPoints=\"%d",&np);
    else
        goto ERR;
    lineno++;
    if( (colno=strstr(line,"NumberOfCells")) != NULL )
        sscanf( colno, "NumberOfCells=\"%d",&ne);
    else
        goto ERR;
    cerr << endl << "File: " << fileName.toStdString() << " np=" << np << " ne=" << ne << endl;
    if( lineno++, fgets(line,1024,in) == NULL // <Points>
        || lineno++, fgets(line,1024,in) == NULL // <DataArray  type="Float32"  NumberOfComponents="3"  format="ascii">
        )
        goto ERR;
    grid= newGrid();
    grid->setGridName("Grid");
    grid->NumberOfSpaceDim = 3;
    grid->_nodes.resize( np );
    grid->_elems.resize( ne );
    grid->attr["etype"] = "T4n3D";
    double x,y,z;
    cerr << "Reading nodes...";
    for( int i= 0; i < np; i++ ) {
        lineno++;
        if( fgets(line,1024,in) == NULL )
            goto ERR;
        int tmp;
        x= y= z= 0;
        if( (tmp=sscanf( line, "%lf %lf %lf", &x, &y, &z )) == 3 ) {
            grid->_nodes[i].nr = i+1;
            grid->_nodes[i]._parent = grid;
            grid->_nodes[i][0] = x;
            grid->_nodes[i][1] = y;
            grid->_nodes[i][2] = z;
            //cerr << i << " ";
        } else {
            cerr << "\"" << line << "\" -> " << tmp << ": x=" << x << " y=" << y <<" z=" << z << endl;
            goto ERR;
        }
    }
    cerr << "got all." << endl;
    if( lineno++, fgets(line,1024,in) == NULL // </DataArray>
        || lineno++, fgets(line,1024,in) == NULL // </Points">
        || lineno++, fgets(line,1024,in) == NULL // <Cells>
        || lineno++, fgets(line,1024,in) == NULL // <DataArray  type="Float32"  NumberOfComponents="3"  format="ascii">
        )
        goto ERR;
    int n[4];
    cerr << "Reading " << ne << " elements ..." << endl;
    Elem *e= 0;
    for( int i= 0; i < ne; i++ ) {
        // cerr << "Reading element " << i << endl;
        int nn;
        lineno++;
        if( fgets(line,1024,in) == NULL )
            goto ERR;
        if( (nn= sscanf(line,"%d %d %d %d", &n[0],&n[1],&n[2], &n[3])) == 4 ) {
            e = new ElemT4n3D( grid );
            ((ElemT4n3D*)e)->setNodes( n[0] + 1, n[1] + 1, n[2] + 1, n[3] + 1 ); // FIVER starts numbering nodes with 1 and FENICS starts with 0
        } else if( nn == 3 ) {
            e = new ElemT3n2D( grid );
            ((ElemT3n2D*)e)->setNodes( n[0] + 1, n[1] + 1, n[2] + 1 ); // FIVER starts numbering nodes with 1 and FENICS starts with 0
        } else {
            cerr << "Unknown element type in line " << lineno << " # of nodes in elememnt = " << nn << endl;
        }
        grid->_elems[ i ] = e;
        e->nr = i+1;
        grid->addElemToSubdomain(i+1,1);
    }
    cerr << "got all." << endl;
    if( lineno++, fgets(line,1024,in) == NULL // </DataArray>
        || lineno++, fgets(line,1024,in) == NULL // <DataArray  type="UInt32"  Name="offsets"  format="ascii">
        )
        goto ERR;
    for( int i= 0; i < ne; i++ ) { // skipping offsets
        lineno++;
        if( fgets(line,1024,in) == NULL )
            goto ERR;
    }
    if( lineno++, fgets(line,1024,in) == NULL // </DataArray>
        || lineno++, fgets(line,1024,in) == NULL // <DataArray  type="UInt8"  Name="types"  format="ascii">
        )
        goto ERR;
    for( int i= 0; i < ne; i++ ) { // skipping types
        lineno++;
        if( fgets(line,1024,in) == NULL )
            goto ERR;
    }
    if( lineno++, fgets(line,1024,in) == NULL // </DataArray>
        || lineno++, fgets(line,1024,in) == NULL // </Cells">
        )
        goto ERR;
    if( lineno++, fgets(line,1024,in) == NULL ) // <?Data  Vectors="u">
        goto ERR;
    char dataType[32], fieldType[32];
    sscanf( line, "<%s %s",  dataType, fieldType);
    if( lineno++, fgets(line,1024,in) == NULL // <DataArray  type="Float32"  Name="u"  NumberOfComponents="3"  format="ascii">
        )
        goto ERR;
    cerr << "Data type=\"" << dataType << "\" FieldType=\"" << fieldType << "\"" << endl;
    field = newField();
    SimpleField * sfield = new SimpleField();
    int nvals = ( strcmp(dataType,"CellData") == 0 ? ne : np);
    if( strstr(fieldType,"Vectors") == fieldType ) {
        sfield->dim(3);
        for( int i= 0; i < nvals; i++ ) {
            lineno++;
            if( fgets(line,1024,in) == NULL )
                goto ERR;
            int tmp;
            x= y= z= 0;
            if( (tmp=sscanf( line, "%lf %lf %lf", &x, &y, &z )) == 3 ) {
                sfield->set(0,i,x);
                sfield->set(1,i,y);
                sfield->set(2,i,z);
            } else {
                cerr << "\"" << line << "\" -> " << tmp << ": x=" << x << " y=" << y <<" z=" << z << endl;
                goto ERR;
            }
        }
    } else {
        sfield->dim(1);
        for( int i= 0; i < nvals; i++ ) {
            lineno++;
            if( fgets(line,1024,in) == NULL )
                goto ERR;
            int tmp;
            x= 0;
            if( (tmp=sscanf( line, "%lf", &x )) == 1 ) {
                sfield->set(i,x);
            } else {
                cerr << "\"" << line << "\" -> " << tmp << ": x=" << x <<  endl;
                goto ERR;
            }
        }
    }
    field->add( sfield, 0 );
    field->setAttr("gridRefNum", "1");
    field->setAttr("name", name);
    fclose(in);
}

void VTKFile::loadFromFile( string fileName )
{

    setlocale( LC_ALL, "C");

    QString qfn( fileName.c_str() );
    QString path( fileName.substr(0,fileName.find_last_of("/")+1).c_str());
    QFile file( qfn );
    QDomDocument ds;
    QString errmsg;
    int errline, errcol;
    if( ds.setContent(&file, &errmsg, &errline, &errcol ) ) {
        QDomElement root = ds.documentElement();
        QDomElement collection= root.firstChildElement("Collection");
        QDomElement dataset= collection.firstChildElement("DataSet");
        readVTKDataSet(path+dataset.attribute("file"),fileName.substr(0,fileName.find_first_of(".")));
    } else {
        qWarning() << "Error while reading VTKFile: " << qfn << " line=" << errline << ", column=" << errcol << " Error=\"" << errmsg << "\"";
    }
    file.close();
    cerr << "Got " << grids.size() << " grid(s) and " << fields.size() << " field(s)." << endl;
}

void VTKFile::loadFromArgv( int argc, char ** argv )
{
    int i= 0;
    while ( i <  argc ) {
        std::cerr << "argv[" << i << "]= " << argv[i] << std::endl;
        i++;
    }
}

void VTKFile::saveSimu( char * simName )
{
}

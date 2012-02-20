#include "diffreader.h"
#include <sstream>
#include <string.h>
#include <logger_t.h>
#include <stdlib.h>

#define assert( co ) { \
if ( !(co) ) { \
                   printf("(%s,%d) assert error! exiting program. \n",__FILE__, __LINE__); \
                   exit(-1); \
    }\
} while (1==0)

#define assert_ignore( co ) { \
if ( !(co) ) {}; \
} while (1==0)

string DpIxFileNameContainer::ixFileName(string fname)
{
	if (_ixFileName[0] == 0) 
		return( buildIxFileName( fname ) );
	else 
		return( _ixFileName );
}

string DpIxFileNameContainer::buildIxFileName(string fname)
{
	setIxFileName(fname);
	_ixFileName = _ixFileName + "_ix";
	return (_ixFileName);
}

void DpIxFileNameContainer::setIxFileName(string fname)
{
	if (fname.size() > 250)
		printf("Warning! Diffpack index file name too long: (length = %d, name = '%s')\n",
			   	fname.size(), fname.c_str());

	_ixFileName = fname;
}

void DpIxFileNameContainer::SaveIxToFILE( FILE * fout, string fname )
{
	if (bSaveIxFile) {
		fseek(fout,0,SEEK_END);
		if (bClearIxFile) {
			dix.clear();
			dix.append(1,ftell(fout));
			dix.SaveToFile(ixFileName(fname));
		} else
			dix.AppendToFile(ixFileName(fname) , ftell(fout) );
	}
}

DpReader::DpReader()
:GridReader(), DpIxFileNameContainer()
{
    FReaderName = "DpReader";
    gridNum = 1;
    checkNum = 1;
    gridType = "BasisFuncGrid";
    dim = 3;
	bSaveIxFile = false;
}

void DpReader::GetGridInfo( stGridInfo * gridInfo )
{
	if (gridInfo != 0) {
		gridNum = gridInfo->gridNum;
		checkNum = gridInfo->checkNum;
		gridType = gridInfo->gridType;
	}
}


int DpReader::ReadFromFile(Grid *g, const string fname, int fstartpos)
{
    char line[1024];
    int i;
    int gFormat = 0;

    _g = g;


    //qDebug() << "Opening file: " << fname.c_str();
    if ((fin=fopen(fname.c_str(),"r")) == NULL) {
		printf("Error: Can not open file: %s\n",fname.c_str());
                    return(errno);
    };
    
    if (fstartpos>0) {
        //qDebug() << "Seeking position = " << fstartpos;
        fseek(fin,fstartpos,SEEK_SET);
    }

    while (!feof(fin)) {
        if (fgets(line,254,fin) != NULL) {            
           // qDebug() << "Line: " << line;
            if (strstr(line,"Finite element") != NULL) {

                ReadGridHeader(line);
                // skipping file to nodes...
                i = 0;
                while ( (i != '#') && ( !feof(fin)) ) {
                  i = fgetc(fin);
                }

                // checking whether binary or text
                i = fgetc(fin);
                if (i == '@')
                  gFormat = GRID_BINARY;
                else {
                  gFormat = GRID_TEXT;
                  ungetc(i,fin);
                }

                switch (gFormat) {
                  case GRID_BINARY:
                    ReadBinary();
                    break;
                  case GRID_TEXT:
                    ReadText();
                    break;
                  default:
                    printf("Unsupported file format. Should be BINARY or TEXT.\n");
                };

                break;
            }
        }
    }

    fclose(fin);
    return(0);
}

void DpReader::ReadBinary()
{
#ifdef Q_OS_UNIX
  char ename[1024];
  int nodesInElem = 0;
  unsigned int i,j;
  XDR xdr;
  int iVal, ic;
  int iDummy;
  int n[8];
  int precision;
  double x,y,z;
  double dx,dy,dz;
  
  //printf("reading binary...\n");
  if ( fscanf(fin,"%d %s",&nodesInElem, ename) != 2 ) {
      printf("(%s,%d): fscanf failed",__FILE__, __LINE__);
  }
  
  if ( (strcmp(ename,"ElmT4n3D") == 0) || (strcmp(ename,"ElmT3n2D") == 0) ) {
    
    // Skip Indicator names and some other stuff until '@; is encountered.
    i = 0;
    while ( (i != '@') && ( !feof(fin) ) )
      i = fgetc(fin);
    
    xdrstdio_create( &xdr, fin, XDR_DECODE );
    
    // reading elements.
    xdr_int( &xdr, &iVal );
    //printf("%d values to read found\n",iVal);
    xdr_int( &xdr, &iDummy );
    //printf("%d precision of single value\n",i);    

    if (strcmp(ename,"ElmT4n3D") == 0) 
        _g->_elems.resize( iVal / 4 );    
    if (strcmp(ename,"ElmT3n2D") == 0) 
        _g->_elems.resize( iVal / 3 );    

    for (i=0; i < _g->_elems.size(); i++) {
        if (strcmp(ename,"ElmT4n3D") == 0) {
            ElemT4n3D * e;
            for (int j=0; j < 4; j++)
              xdr_int( &xdr, &n[ j ] );
             
              e = new ElemT4n3D( _g );
            e->setNodes( n[0], n[1], n[2], n[3] );
            e->nr = i+1;
            _g->_elems[i] = e;
        }
        if (strcmp(ename,"ElmT3n2D") == 0) {
            ElemT3n2D * e;
            for (int j=0; j < 3; j++)
              xdr_int( &xdr, &n[ j ] );
             
              e = new ElemT3n2D( _g );
            e->setNodes( n[0], n[1], n[2] );
            //printf("%d,%d,%d\n",n[0],n[1],n[2]);
            e->nr = i+1;
            _g->_elems[i] = e;
        }


    }
    // Skip Indicator names and some other stuff until '@; is encountered.
    i = 0;
    while ( (i != '@') && ( !feof(fin) ) ) {
      i = fgetc(fin);
      //printf("skipping: %c\n",i);
    }
      
    // reading nodes.
    xdr_int( &xdr, &iVal );
    //printf("%d node coords values to read found\n",iVal);
    xdr_int( &xdr, &precision );
    //printf("%d precision\n",precision);    
    _g->_nodes.resize( iVal / _g->NumberOfSpaceDim );    
    
    for (i=0; i < _g->_nodes.size(); i++) {
      _g->_nodes[i].nr = i+1;
      _g->_nodes[i]._parent = _g;
      switch (precision) {
      case 4:
        xdr_double(&xdr,&x);
        xdr_double(&xdr,&y);
        if (_g->NumberOfSpaceDim > 2) xdr_double(&xdr,&z); else z = 0;
        //printf("%f, %f, %f\n", x,y,z);
        _g->_nodes[i][0] = x;
        _g->_nodes[i][1] = y;
        _g->_nodes[i][2] = z;
        break;
      case 8:
        xdr_double(&xdr,&dx);
        xdr_double(&xdr,&dy);
        if (_g->NumberOfSpaceDim > 2) xdr_double(&xdr,&dz); else dz = 0;
        //printf("%lf, %lf, %lf\n", dx,dy,dz);
        _g->_nodes[i][0] = (double) dx;
        _g->_nodes[i][1] = (double) dy;
        _g->_nodes[i][2] = (double) dz;
        break;
      default:
        printf("Unsupported XDR precision for node coordinates: %d\n", precision);      
      }
    }

    // skipping to subdomains
    while ( (i != '@') && ( !feof(fin) ) ) {
      i = fgetc(fin);
      //printf("skipping: %c\n",i);
    }

	//printf("loading element subdomain indicators...\n");
    xdr_int( &xdr, &ic );
    xdr_int( &xdr, &precision ); // this value is ingnored!!!!
    
    for (i = 0; i < (unsigned int) ic; i++) {
      xdr_int( &xdr, &iVal );
      (*_g).addElemToSubdomain(i+1,iVal);
    }    
    
	//printf("loading boundary indicators...\n");
    // reading boundary indicators
    xdr_int( &xdr, &ic );
    xdr_int( &xdr, &precision ); // number of boundary indicators
    
	//printf("number of boundary indicators: %d\n", ic);
    for (i = 0; i < (unsigned int) ic; i++) {
      for (j = 0; j < (unsigned int) precision; j++) {
        xdr_int(&xdr, &iVal);
        if (iVal == 1) {
				_g->addNodeToBoundary(i,j+1);
				_g->_nodes[i].addBoundary(j+1);
		}
      }
    }
	//printf("finished...\n");
    
  } else {
    printf("Unsupported element for binary grid file format: %s.\n", ename);
  }
#endif
}

void DpReader::ReadText()
{
    char line[1024];
    int in,ib; // inodes, iboundaries
    int ie,is; // ielement, isubdomain
	unsigned int i;
    float x,y,z;
    double dx,dy,dz;
    char ename[255];
    char sbnd[1024];
    int n[8];
  //  printf("reading text...\n");
  // Read nodes coordinates
	setlocale( LC_ALL, "C");
  if ( fgets(line, 1024, fin) == NULL) return;
  
  for (i=0; i < _g->_nodes.size(); i++) {
    assert( fgets(line,254,fin)!=NULL );
    if (_g->NumberOfSpaceDim == 2) {
        sscanf(line,"%d (%f,%f) [%d]",&in, &x,&y,&ib);
    }
    if (_g->NumberOfSpaceDim == 3) {
				//printf("line = %s\n",line);
        sscanf(line," %d ( %f, %f, %f ) [%d] %[ 0123456789]",&in, &x,&y,&z,&ib,sbnd);
	//printf("%s\n",line);
        //printf("node %d \n",in);
	//printf("[%f,%f,%f]\n",x,y,z);
		dx =x;dy =y; dz = z;
    } else { // inaczej to musi byc 2D
        sscanf(line," %d ( %f, %f ) [%d] %[ 0123456789]",&in, &x,&y,&ib,sbnd);
		dx =x;dy =y; dz = 0;
    }
    _g->_nodes[in-1].nr = in;
    _g->_nodes[in-1]._parent = _g;
    _g->_nodes[in-1][0] = dx;
    _g->_nodes[in-1][1] = dy;
    _g->_nodes[in-1][2] = dz;
    //printf("node %d after coordinates\n",in);
    for (int _i = 0; _i < ib; _i++) {
        int ibv;
						//printf("sbnd = %s\n", sbnd);
        sscanf(sbnd,"%d %[ 0123456789]",&ibv,sbnd);
						//printf(" node %d adding boundary: %d\n", in, ibv-1);
		int ibnd = _g->_bnds[ ibv - 1 ].nr;
						//printf("ibnd = %d\n", ibnd);
		_g->addNodeToBoundary(in-1,ibnd);
		_g->_nodes[in-1].addBoundary(ibnd);
     	// Tu byl jakis niezrozumialy dla mnie blad...
            //_g->addNodeToBoundaryIndex(in-1,ibv-1);
						
						//printf(" node %d adding boundary: %d after\n", in, ibv-1);
        }
		//printf("node %d finished\n",in);
  }
  
  // printf("reading text...nodes.. finished\n");
  
  // Skip whitespace...
  i =0;
  while ( (i != '#') && ( !feof(fin) ) )
    i = fgetc(fin);
    
  // Read elements...
  i =0;
  while ( (i < _g->_elems.size() ) && ( !feof(fin) )) {
    assert( fgets(line, 255, fin) != NULL );
    ename[0] = 0;
    sscanf(line,"%d %s %d",&ie,ename,&is);
    if (strcmp(ename,"ElmT4n3D")==0) {
        sscanf(line,"%d %s %d %d %d %d %d",&ie,ename,&is, &n[0],&n[1],&n[2], &n[3]);
        ElemT4n3D *e;
        e = new ElemT4n3D( _g );
        _g->_elems[ i ] = e;
        e->nr = ie;
        e->setNodes( n[0], n[1], n[2], n[3] );
        //printf("After setNodes...\n");
        (*_g).addElemToSubdomain(ie,is);
        //printf("After addElem...\n");
        i++;
    }
    if (strcmp(ename,"ElmT3n2D")==0) {
        sscanf(line,"%d %s %d %d %d %d",&ie,ename,&is, &n[0],&n[1],&n[2]);
        ElemT3n2D *e;
        e = new ElemT3n2D( _g );
        _g->_elems[ i ] = e;
        e->nr = ie;
        e->setNodes( n[0], n[1], n[2] );
        //printf("After setNodes...\n");
        (*_g).addElemToSubdomain(ie,is);
        //printf("After addElem...\n");
        i++;
    }
  }
  // printf("reading text...finished\n");
  
}


int DpReader::ReadGridHeader(char *line)
{
    char* bs,*bf;
    int iv, ispace,ielems,inodes, ic;
	unsigned int i;
    char buf[1024];
    bool bFollowing;

    bs = strstr(line,"(");
    bf = strstr(line,")");
    //printf("Starting reading header.\n");
    
    if ((bs==NULL) || (bf==NULL)) {
        printf("Incorrect file format. expected: 'Finite element mesh (xxxxx):' found: %s\n",line);
        return(-1);
    }
    //printf("Copying name of grid bs = %p, bf = %p\n",bs,bf);
    strncpy(buf,bs+1,bf-bs-1);
    buf[bf-bs-1] = 0;

    //qDebug() << "Storing name of the grid: " << _g;
    _g->setGridName(buf);

    assert( fgets(line,1024,fin) != NULL);
    //printf("Line: %s\n",line);
    if (strlen(line) < 3) assert( fgets(line,500,fin) != NULL );
    bs = strstr(line,"=");
    bs = bs + 2;
    strcpy(buf,bs);    
    //printf("Buf = %s\n",buf);
    sscanf(buf,"%d",&ispace);
    //printf("Number of space dim. = %d\n",ispace);

    assert( fgets(line,1024,fin) != NULL);
    bs = strstr(line,"=");
    bs = bs + 2;
    strcpy(buf,bs);
    sscanf(buf,"%d",&ielems);
    //qDebug() << "Number of elements: " << ielems;

    assert( fgets(line,1024,fin) != NULL);
    bs = strstr(line,"=");
    bs = bs + 2;
    strcpy(buf,bs);
    sscanf(buf,"%d",&inodes);
    //qDebug() << "Number of nodes: " << inodes;

    _g->_nodes.resize( inodes );
    _g->attr["etype"] = "T4n3D";
    //printf("Successfully set nodes count\n");
    _g->_elems.resize( ielems );
    //printf("Successfully set elements count\n");
    _g->NumberOfSpaceDim = ispace;
    
    assert( fgets(buf,1024,fin) != NULL);
    while ( (strstr(buf,"oundary indicators") == NULL) && (!feof(fin) ) )
      assert_ignore( fgets(buf,1024,fin) != NULL);
    
    if (strstr(buf,"boundary") != NULL) {
      sscanf(buf, "%d boundary indicators:%[ 0-9a-zA-Z]", &ic,buf);
    } else {
      sscanf(buf, "%d Boundary indicators:%[ 0-9a-zA-Z]", &ic,buf);
    }
    //printf("Trying to read %d boundary indicators.\n", ic);
    //printf("Buf after: %s\n",buf);
    
    // checking if the indicators are in the same line or in the following lines.    
    bFollowing = true;
    for (i=0; i< strlen( buf ); i++)
      if ( !isspace(buf[i]) ) {
	  	//printf("%c, %d\n", buf[i], (int) buf[i]);
        bFollowing = false;
	  }
    
    if (bFollowing) {  
      //printf("Following = true\n");
      for (i =0; i < (unsigned int) ic; i++) {
        if (fscanf(fin,"%d",&iv) != 1) {
            printf("(%s,%d) fscanf failed\n",__FILE__, __LINE__);
        }
        //printf("Read boundary indicator: %d\n", iv);
			  iv = i+1; // jstar - proteza, ale i tak nie obslugujemy nazw, tylko numery BI
        _g->addBoundary( iv );
      }
    } else {
      //printf("Following = false\n");
      for (i =0; i < (unsigned int) ic; i++) {
        sscanf(buf,"%d %[ 0-9a-zA-Z]",&iv, buf);
       // printf("Read boundary indicator: %d\n", iv);
			 iv = i+1; // jstar - proteza, ale i tak nie obslugujemy nazw, tylko numery BI
        _g->addBoundary( iv );
      }
    }              
    return(0);
}

DpGridFileInfo::DpGridFileInfo()
{
}

DpGridFileInfo::~DpGridFileInfo()
{
}
        
void DpGridFileInfo::ReadFromFile(string fname)
{
    FILE *fin;
    char line[255];
    gridInfo *gi;
    char s[255];
    char *s1;
    int nr;
    int iLastPosition = 0;
    
//  printf("Opening file: %s\n",fname.c_str());
	if ((fin=fopen(fname.c_str(),"r")) == NULL) {
		printf("Error: Can not open file: %s\n",fname.c_str());
                    return;
    };


    
    iLastPosition = ftell(fin);
    assert( fgets(line,254,fin) != NULL);
    while (!feof(fin)) {
        //qDebug() << "Line: " << line << " iLastPosition: " << iLastPosition;
        if (strstr(line,"Finite element mesh") != NULL) {
            //qDebug() << "Found Finite element mesh on position: " << iLastPosition;
            gi = add();
//          printf("new gridInfo added successfully\n");
            
            gi->position = iLastPosition;
            
            sscanf(line,"Finite element mesh (%s",s);
            s1 = strtok(s,":)");
//          printf("Field Name = %s\n",s1);
            
            strcpy(gi->name,s);
//          printf("Successfully stored grid name\n");
            line[0]=0;
            while (strlen(line) < 3)
                assert( fgets(line,254,fin) != NULL);
            sscanf(line," Number of space dim. = %d",&nr);
            gi->NoOfSpaceDim = nr;
            
            assert( fgets(line,254,fin) != NULL);
            sscanf(line," Number of elements =  %d",&nr);
            gi->NoOfElements = nr;
            
            assert( fgets(line,254,fin) != NULL);
            sscanf(line," Number of nodes =  %d",&nr);
            gi->NoOfNodes = nr;
        }
        iLastPosition = ftell(fin);

        assert_ignore( fgets(line,254,fin) != NULL);
    }
        
    fclose(fin);
}

int DpReader::AppendToFile(Grid *g, const string fname, stGridInfo * gridInfo)
{
    FILE * fout;
    unsigned int i,j;

	if ((fout = fopen(fname.c_str(),"a"))==NULL){
		printf("Can not open file: %s, for writing.\n",fname.c_str());
    } else {

				// Checks internal variables!
		SaveIxToFILE( fout, fname );
		
		GetGridInfo(gridInfo);

        // header
        fprintf(fout,"\n");
        fprintf(fout,">GRIDNUM  %d\n", gridNum);
        fprintf(fout,">CHECKNUM  %d\n", checkNum);
		fprintf(fout,">GRIDTYPE %s\n", gridType.c_str());
        fprintf(fout,">DIM %d\n", dim);
        
        
        fprintf(fout,"\n\n");
        fprintf(fout,"Finite element mesh (GridFE):\n");
        fprintf(fout,"  Number of space dim. =   %d\n", g->NumberOfSpaceDim);
        fprintf(fout,"  Number of elements   =   %d\n", g->_elems.size());
        fprintf(fout,"  Number of nodes      =   %d\n", g->_nodes.size());
        fprintf(fout,"\n");
        fprintf(fout,"  All elements are of the same type :   %s\n","dpTRUE");
        fprintf(fout,"  Max number of nodes in an element:  %d\n",4);
        fprintf(fout,"  Only one subdomain               :  %s\n","dpFALSE");
        fprintf(fout,"  Lattice data                     ? 0\n");
        fprintf(fout,"\n\n\n");
        
        fprintf(fout,"  %d Boundary indicators:  ",g->_bnds.size());
        for (i=0; i< g->_bnds.size(); i++)
            fprintf(fout,"%d ",g->_bnds[i].nr);
        fprintf(fout,"\n\n\n");

        fprintf(fout,"  Nodal coordinates and nodal boundary indicators,\n");
        fprintf(fout,"  the columns contain:\n");
        fprintf(fout,"  - node number\n");
        fprintf(fout,"  - coordinates\n");
        fprintf(fout,"  - no of boundary indicators that are set (ON)\n");
        fprintf(fout,"  - the boundary indicators that are set (ON) if any.\n");
        fprintf(fout,"#\n");
        fflush(fout);

        for (i=0;i<g->_nodes.size();i++){
            fprintf(fout,"  %d  ( %.5f, %.5f",
                    g->_nodes[i].nr,
                    g->_nodes[i].x(),
                    g->_nodes[i].y()
            );
            if( g->NumberOfSpaceDim == 3 )
		fprintf(fout, ", %.5f", g->_nodes[i].z() );
            fprintf(fout," ) [%d] ",
                    g->_nodes[i]._bnds.size()
            );
            for (j=0;j<g->_nodes[i]._bnds.size();j++)
                fprintf(fout,"%d ",g->indexOfBoundary( g->_nodes[i]._bnds[j]) + 1 );
            fprintf(fout,"\n");
        }

        fprintf(fout,"\n  Element types and connectivity\n");
        fprintf(fout,"  the columns contain:\n");
        fprintf(fout,"   - element number\n");
        fprintf(fout,"   - element type\n");
        fprintf(fout,"   - subdomain number\n");
        fprintf(fout,"   - the global node numbers of the nodes in the element.\n");
        fprintf(fout,"#\n");

        for (i=0; i < g->_elems.size(); i++) {
            if (g->_elems[i]->elemType() == T4n3D) {
                fprintf(fout,"  %d %s %d %d %d %d %d\n",
                        g->_elems[i]->nr,
                        "ElmT4n3D",
                        g->_elems[i]->subdomain(),
                        g->_elems[i]->_nodes[0],
                        g->_elems[i]->_nodes[1],
                        g->_elems[i]->_nodes[2],
                        g->_elems[i]->_nodes[3]);
            }
            if (g->_elems[i]->elemType() == T3n2D) {
                fprintf(fout,"  %d %s %d %d %d %d\n",
                        g->_elems[i]->nr,
                        "ElmT3n2D",
                        g->_elems[i]->subdomain(),
                        g->_elems[i]->_nodes[0],
                        g->_elems[i]->_nodes[1],
                        g->_elems[i]->_nodes[2]);
            }


        }
        
        fprintf(fout,"\n\n");
        // eof header
        fclose(fout);
    }
	return(0);
}

int DpReader::SaveToFile(Grid *g, const string fname, stGridInfo * gridInfo )
{
  FILE * f;
  if ( (f=fopen(fname.c_str(),"wr")) == NULL ) {
	  printf("Unable to open file for writing: %s\n", fname.c_str());
  } else {
    fclose(f);
		bClearIxFile = true;
    	AppendToFile(g,fname, gridInfo);
		bClearIxFile = false;
  }
  return(0);
}

DpFieldFileInfo::DpFieldFileInfo()
{
}

DpFieldFileInfo::~DpFieldFileInfo()
{
}

int DpFieldFileInfo::ReadFromFile( string fname )
{
    FILE * fin;
    char line[255];
    FieldInfo *fi = 0;
    int inum = 0, inum2 = 0;
    char stext[255];
    bool bFound;
    
    if ( (fin = fopen( fname.c_str(), "r" )) == NULL ) {
        return(-1);
    } else {
        while (!feof(fin)) {
            assert( fgets(line,254,fin) != NULL);
            bFound = false;
            
            if ( strstr(line, "FIELDNUM") != NULL ) {
              fi = addNew();
              sscanf(line,">FIELDNUM %d", &inum);
              fi->fieldNum = inum;              
			  //printf("inum = %d\n", inum);
              bFound = true;
            }
            
            if ( strstr(line, "HEADER") != NULL ) {
              sscanf(line,">HEADER %[ 0-9a-zA-Z_] ", stext);
			  //printf("stext = %s\n", stext);
              fi->name = stext;
              bFound = true;
            }

            if ( strstr(line, "TIME") != NULL ) {
              sscanf(line,">TIME %[ 0-9a-zA-Z.--] ", stext);
			  //printf("stext = %s\n", stext);
              fi->sTime = stext;
              bFound = true;
            }
            
            if ( strstr(line, "DESCRIPTION") != NULL ) {
              sscanf(line,">DESCRIPTION %[ 0-9a-zA-Z_] ", stext);
              fi->description = stext;
              bFound = true;
            }
            
            if ( strstr(line, "GRIDREFNUM") != NULL ) {
              sscanf(line,">GRIDREFNUM %d", &inum);
              fi->gridRefNum = inum;
              bFound = true;
            }
            
            if ( strstr(line, "FIELDTYPE") != NULL ) {
              sscanf(line,">FIELDTYPE %[ 0-9a-zA-Z_] ", stext);
              fi->fieldType = stext;
              bFound = true;
            }
            
            if ( strstr(line, "COMPONENT") != NULL ) {
              sscanf(line,">COMPONENT %d OF %d",&inum, &inum2);
              fi->component = inum;
              fi->dim = inum2;
              bFound = true;
            }
            
            if ( (fi != 0) && (bFound == false) ) {
              inum = 0;
              while ((inum != '[') && (!feof(fin)))
                inum = fgetc(fin);
                
              fi->position = ftell(fin) - 1;
                
              if ( fscanf(fin, "%d", &inum) != 1) {
                  printf("(%s,%d) fscanf failed\n", __FILE__, __LINE__);
              }
              fi->size = inum;
              
              fi = 0;              
            }
            
        }
        fclose(fin);
    }
    return(0);
}

DpFieldReader::DpFieldReader()
:DpIxFileNameContainer()
{
	fieldNum = 1;
	header ="undefined_header";
	description ="undefined_description";
	gridRefNum = 1;
	fieldType = "FieldSubDomains";
	component = 1;
	dim = 1;
	bSaveIxFile = false;
	fieldTypeLong = "FieldSubDomains, the elements are the subdomains";
}

DpFieldReader::~DpFieldReader()
{
}

int DpFieldReader::ReadFromFileName(SimpleField * f, string fname, FieldFileInfo *ffi, string fieldname, string stime)
{
    int i;
	
//	fprintf(stderr, "Reading fieldname = %s\n", fieldname.c_str());
	FieldInfo * finfo = ffi->getInfo( fieldname );
	if (finfo == 0) {
        fprintf(stderr,"Unable to find fieldname: %s in file %s\n", fieldname.c_str(), fname.c_str());
	} else {
		f->dim( finfo->dim );

		for (i = 1; i <= ffi->size(); i++) {
			bool bTime;
			if (stime != "")
				bTime = (stime == ffi->getInfo(i)->sTime);
			else
				bTime = true;

			if (  (fieldname == ffi->getInfo(i)->name) && ( bTime ) ) {
				component = ffi->getInfo(i)->component;
				ReadFromFile(f,fname, ffi->getInfo(i)->position);
				std::stringstream ss;
				ss << ffi->getInfo(i)->gridRefNum;
				f->setAttr("gridRefNum", ss.str());
				f->setAttr("name",ffi->getInfo(i)->name);
			}
		}
	}
	return (0);
}

int DpFieldReader::ReadFromFile( SimpleField * f, string fname, int fstartpos, FieldInfo *  )
{
  FILE * fin;
  int inum = 0;
  int icount = 0;
  
  if ( (fin = fopen(fname.c_str(),"r")) == NULL) {
    fprintf(stderr,"Unable to open field file: %s\n", fname.c_str());
    return(-1); 
  } else {
    //printf("Seeking %d\n",fstartpos);
    fseek(fin,fstartpos,SEEK_SET);
    
    if ( fscanf(fin,"[%d]",&icount) != 1) {
        printf("(%s,%d) fscanf failed\n", __FILE__, __LINE__);
    }
    //printf("%d values to read found.\n", icount);
    
    inum = fgetc(fin);
    if (inum == 'b') {
      //printf("Binary field format found.\n");
      ReadBinary(f,fin,icount);
    } else {
      //printf("Text field format found.\n");
      ReadText(f,fin,icount);
    }      
    fclose(fin);
  }
  return(0);
}

void DpFieldReader::ReadBinary( SimpleField * f, FILE * fin, int icount )
{
#ifdef Q_OS_UNIX
  XDR xdr;
  int i;
  int tmp;
  double dVal;
  double  fVal;
  
  i = 0;
  while ( (i != '@') && (!feof(fin)))
    i = fgetc(fin);
  
  //printf("f->dim() = %d\n", f->dim());
  if (f->dim() < 1) {
      f->dim(1);
      component = 1;
  }

  //printf("component = %d\n", component);
  f->val( component - 1, icount - 1, 0.0f );
  
  xdrstdio_create( &xdr, fin, XDR_DECODE );
  xdr_int(&xdr,&tmp);
  //printf("Ilosc danych do wczytania: %d\n", tmp);
  xdr_int(&xdr,&tmp);
  //printf("Format danych (8 lub 4): %d\n", tmp);
  
  switch (tmp) {
    case 8:
      for (i=0; i < icount; i++) {
        xdr_double(&xdr, &dVal);
        f->val(component - 1, i, dVal);
      }
      break;
    
    case 4:
      for (i=0; i < icount; i++) {
        xdr_double(&xdr, &fVal);
        f->val(component - 1, i,(double) fVal);
      }
      break;
    default:
      printf("Unsupported XDR data format for precision: %d\n", tmp);
  }
  
  xdr_destroy(&xdr);
#endif
}

void DpFieldReader::ReadText( SimpleField * f, FILE * fin, int icount )
{
  int inum = 0, i, ir;
  char line[255];
  float fval;
  
	setlocale( LC_ALL, "C");

  //printf("f->dim() = %d\n", f->dim());
  if (f->dim() < 1) {
      f->dim(1);
      component = 1;
  }
  f->val( component - 1, icount - 1, 0.0f );
  
  // Read the rest of line...
  assert( fgets(line,255,fin) != NULL );
  for (i = 0; (i < icount) && (!feof(fin)) ; i++) {
    ir = fscanf(fin," ( %d)= %f", &inum, &fval);
    //printf("(%d) Read from file: (%d) = %g\n",ir,inum, fval);
    f->val(component - 1, i, (double) fval);
  }
}

int DpFieldReader::SaveToFile( SimpleField * f, string fname, FieldInfo * fieldInfo )
{
	FILE *fout;
  
  if ( (fout = fopen(fname.c_str(),"wr")) == NULL ) {
    printf("Unable to open file for writing: %s\n", fname.c_str());
  } else {
    fclose(fout);
		bClearIxFile = true;
		AppendToFile(f,fname,fieldInfo);
		bClearIxFile = false;
  }
  return(0);
}

void DpFieldReader::GetFieldInfo( FieldInfo * fieldInfo )
{
	if (fieldInfo != 0) {
		printf("Getting FieldInfo\n");
		fieldNum = fieldInfo->fieldNum;
		header = fieldInfo->name;
		description = fieldInfo->description;
		gridRefNum = fieldInfo->gridRefNum;
		fieldType = fieldInfo->fieldType;
		component = fieldInfo->component;
		dim = fieldInfo->dim;
		//strncpy(fieldTypeLong, fieldInfo->name);
	}	
}


int DpFieldReader::AppendToFile( SimpleField * f, string fname, FieldInfo * fieldInfo )
{
  FILE *fout;
  int i,j;
  char format[500];
  char buf[20];

  if ( (fout = fopen(fname.c_str(),"a")) == NULL ) {
    printf("Unable to open file for writing: %s\n", fname.c_str());
  } else {

	// Checks internal variables!!!
	SaveIxToFILE(fout, fname.c_str());
	
	GetFieldInfo( fieldInfo );
	
    fprintf(fout,"\n>FIELDNUM  %d\n", fieldNum);
    fprintf(fout,">HEADER  %s$\n", header.c_str());
    fprintf(fout,">DESCRIPTION  %s$\n", description.c_str());
    fprintf(fout,">GRIDREFNUM  %d\n", gridRefNum);
    fprintf(fout,">FIELDTYPE  %s\n", fieldType.c_str());
    fprintf(fout,">COMPONENT  %d OF %d\n\n", component, dim);
                  
    fprintf(fout,"[%d] %s $ \n", f->size(), fieldTypeLong.c_str());
    j = 0;

    sprintf(format,"%d",f->size());
    sprintf(buf,"%d",strlen(format));
    strcpy(format,"  (%");
    strcat(format, buf);
    strcat(format,"d)= %g ");
    for (i = 0; i < f->size(); i++) {
      fprintf(fout,format, i+1,f->val(i));
      j++;
      if (j > 2) {
        j = 0;
        fprintf(fout,"\n");
      }
    }
    
	fprintf(fout,"\n\n");
    fclose(fout);

  }
  return(0);
}

void DpIxFile::clear()
{
	items.resize(0);
}

void DpIxFile::append( int _num, int _position ) 
{
	items.resize( items.size() + 1 );
	items[items.size()-1].num = _num;
	items[items.size()-1].position = _position;
}

void DpIxFile::AppendToFile(string fname, int _position)
{
	ReadFromFile(fname);
	append(count() + 1, _position);
	SaveToFile( fname );
}

void DpIxFile::SaveToFile( string fname )
{
	FILE *fin;
	unsigned int i;

	if ((fin = fopen(fname.c_str(),"w")) == NULL) {
		cerr << "Unable to open file for writing: " << fname << endl;
	} else {
		write_int( fin, items.size() );
		for (i=0; i < items.size(); i++) {
			write_int(fin,items[i].num);
			write_int(fin,items[i].position);
		}
		fclose(fin);
	}
}

void DpIxFile::ReadFromFile( string fname )
{
	FILE *fin;
	unsigned int i;

	if ((fin = fopen(fname.c_str(),"r")) == NULL) {
		cerr << "Unable to open file for reading: " << fname << endl;
	} else {
		i = read_int( fin );
		items.resize( i );
		for (i=0; i < items.size(); i++) {
			items[i].num = read_int(fin);
			items[i].position = read_int(fin);
		}
		fclose(fin);
	}
}

void DpIxFile::write_int( FILE *f, int v)
{
	unsigned int i;
	char * ptr = (char*) &v;
	for (i=0; i<4; i++)
                assert( fwrite( ptr + 3 - i,1,1,f) != 1 );
}

int DpIxFile::read_int( FILE * f )
{
	unsigned int i;
	int v;
	char * ptr = (char*) &v;
	for (i=0; i<4; i++)
                assert( fread( ptr + 3 - i,1,1,f) == 1);
	return(v);
}

void DpIxFile::debug()
{
	unsigned int i;
	cout << "Item count = " << items.size() << endl;
	for (i=0; i<items.size(); i++) {
		cout << "[" << i+1 << "] num      = " << items[i].num << endl;
		cout << "[" << i+1 << "] position = " << items[i].position << endl;
	}
		
}


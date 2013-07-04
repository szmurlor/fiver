#include "stlfile.h"

STLFile::STLFile()
{
}


STLFile::~STLFile()
{
}

//liczy liczbę trójkątów
int STLFile::countElements ( string fileName ){
    int amount=0;
    char line[1024];


    FILE* in= fopen( fileName.c_str(), "r" );
    if( in != NULL )
        cerr << "Counting elements in STLFile: " << fileName << "..." << endl;
    else {
        cerr << "Failed to open " << fileName << " for reading" << endl;
        return 0;
    }
    if (fgets(line,1024,in) == NULL)
        return 0;
    if (strstr(line,"solid") == NULL && strstr(line,"SOLID") == NULL)
        return 0;

    while ( fgets(line,1024,in) != NULL ){
        if ( strstr(line,"facet normal") != NULL || strstr(line,"FACET NORMAL") != NULL) {
            amount++;
        }
    }

    return amount;
}

//przeszukiwanie węzłów w poszukiwaniu podobnego
int STLFile::findEqual ( double x, double y, double z, Grid* grid, int nodesCounter){

    double eps = 0.000001;
//    cout << " mamy pkt " << x << " " <<y << " " <<z<< "porównujemy z: ";
    for (int i=0; i< nodesCounter; i++){
        Node* node = grid->getNode(i+1);
        double checkX = node->x();
        double checkY = node->y();
        double checkZ = node->z();
//        cout << endl << checkX  << " " << checkY  << " " << checkZ;

        if ( abs(checkX - x) < eps && abs(checkY - y) < eps && abs(checkZ - z) < eps ) {
//            cout << "znalazłem :)"<<endl;
            return i+1;
        }
    }

    return -1;
}


Grid * STLFile::loadFromFile( string fileName )
{
    setlocale( LC_ALL, "C");
    Grid *grid = new Grid();
    char line[1024];

    char* solidName;
    int numberOfElem;


    FILE* in= fopen( fileName.c_str(), "r" );   //.toStdString().c_str()
    if( in != NULL )
        cout << "Reading STLFile: " << fileName << "..." << endl;   //.toStdString()
    else {
        cout << "Failed to open " << fileName << " for reading" << endl;    //.toStdString()
        return NULL;
    }

    numberOfElem = this->countElements(fileName);


    if( fgets(line,1024,in) == NULL ){
        ERR: //qWarning() << "Error while reading STL file: " << fileName << "in line: " << endl << line;
        return NULL;
    }

    if ( strstr(line,"solid") != NULL ){
        sscanf ( strstr(line,"solid"), "solid %s", solidName);
    }
    if ( strstr(line,"SOLID") != NULL ){
        sscanf ( strstr(line,"SOLID"), "SOLID %s", solidName);
    }
    if ( solidName == NULL )
        solidName = "";

    grid->setGridName(solidName);
    grid->NumberOfSpaceDim = 3;
    grid->_nodes.resize( numberOfElem*3 );
    grid->_elems.resize( numberOfElem );
    grid->attr["etype"] = "T3n2D";
    double x,y,z;
    int nodesCounter=0;
    cerr << "Reading elements..."<<endl;

    double normx, normy, normz;
    char* normal;


    for( int i= 0; i < numberOfElem ; i++ ) {
        if( fgets(line,1024,in) == NULL || ( (normal = strstr(line,"facet normal")) == NULL &&  (normal = strstr(line,"FACET NORMAL")) == NULL)){
            goto ERR;

        } else {

            if ( sscanf( normal, "facet normal %lf %lf %lf", &normx, &normy, &normz ) == 3){


            } else if ( sscanf (normal, "FACET NORMAL %lf %lf %lf", &normx, &normy, &normz ) == 3){


            }

        }

        if( fgets(line,1024,in) == NULL || (strstr(line,"outer loop") == NULL && strstr(line,"OUTER LOOP") == NULL)) {
              goto ERR;
        }

        int tmp=0, j;
        x= y= z= 0;
        char* vertex;
        int  points[3];

        points[0] = 3*i+1;
        points[1] = 3*i+2;
        points[2] = 3*i+3;

        for ( j=1; j< 4; j++){
            if( fgets(line,1024,in) != NULL ) {
                if ((vertex = strstr(line,"vertex")) != NULL)
                    tmp=sscanf( vertex, "vertex %lf %lf %lf", &x, &y, &z );
                if ((vertex = strstr(line,"VERTEX")) != NULL)
                    tmp=sscanf( vertex, "VERTEX %lf %lf %lf", &x, &y, &z );
                if (tmp == 3){
                    int result = findEqual ( x,y,z,grid,nodesCounter);

                    if (result == -1 ){
                        grid->_nodes[nodesCounter].nr = nodesCounter+1;
                        grid->_nodes[nodesCounter]._parent = grid;
                        grid->_nodes[nodesCounter][0] = x;
                        grid->_nodes[nodesCounter][1] = y;
                        grid->_nodes[nodesCounter][2] = z;
                        points[j-1] = nodesCounter+1;
                        nodesCounter++;
//                        cout << " nowy punkt to: " << x << " " << y  << " " << z<<endl;
//                        cout << "ładujemy nowy punkt "<< nodesCounter << endl;
                    } else {
                        points[j-1] = result;
                    }
                }
            } else {
                cerr << "\"" << line << "\" -> " << tmp << ": x=" << x << " y=" << y <<" z=" << z << endl;
                goto ERR;
            }
        }

        double n[3];
        double eps = 0.1;
        //sprawdzenie czy wektor normalny jest prawidłowy??

        //odwrócony wektor normalny
        normalny4p(grid->_nodes[ points[0]-1 ]._Coords, grid->_nodes[ points[2]-1 ]._Coords, grid->_nodes[ points[1]-1 ]._Coords, grid->_nodes[ points[0]-1 ]._Coords, n);
//        cout << "counted: " << n[0] << ",\t" << n[1] << ",\t"<< n[2]<< endl;

        //czy odwrócony jest prawidłowy?
        if ( abs(n[0] - normx) < eps && abs(n[1] - normy) < eps && abs(n[2] - normz) < eps ){
            double tmp = points[1];
            points[0]= points[0];
            points[1]= points[2];
            points[2]= tmp;
            cout << "zmieniono kolejność. był " << n[0] << " " << n[1]  << " " << n[2]<< "a powinien być " << normx << " " << normy  << " " << normz <<endl;
        }

        Elem *e;
        e = new ElemT3n2D( grid );
        ((ElemT3n2D*)e)->setNodes( points[0], points[1], points[2] ); // FIVER starts numbering nodes with 1 and FENICS starts with 0
        grid->_elems[ i ] = e;
        e->nr = i+1;
        grid->addElemToSubdomain(i+1,1);

        if( fgets(line,1024,in) == NULL || (strstr(line,"endloop") == NULL && strstr(line,"ENDLOOP") == NULL)){
            goto ERR;
//            return NULL;
        }
        if( fgets(line,1024,in) == NULL || (strstr(line,"endfacet") == NULL && strstr(line,"ENDFACET") == NULL)){
            goto ERR;
//            return NULL;
        }
    }

    fclose(in);

    grid->_nodes.resize( nodesCounter );
    cout << "W pliku znaleziono " << nodesCounter << " unikalnych punktów !!!" << endl;
    return grid;
}


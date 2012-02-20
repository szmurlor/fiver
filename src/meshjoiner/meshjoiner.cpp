#include "meshjoiner.h"

#include <iostream>
#include <cstdlib>
#include <geom.h>
//#include <time.h>

MeshJoiner::MeshJoiner()
{
    exGrid = new Grid();
    flags = string("pq1.8a0.5Y");
}

Grid* MeshJoiner::joinMeshes(bool exSplitting)
{
    tetgenio in;
    tetgenio out;

cout << "MeshJoiner::joinMeshes" << endl;

#ifdef ZPALCA
    /////////////////////////////////////////z palca wywołanie/////////////////////////////////////////////////
cout << "Generujemy testowa siatkę w prostopadłościanie" << std::endl;
    tetgenio::facet *f;
    tetgenio::polygon *p;
    int i;

    // All indices start from 1.
    in.firstnumber = 1;

    in.numberofpoints = 8;
    in.pointlist = new REAL[in.numberofpoints * 3];
    in.pointlist[0]  = 0;  // node 1.
    in.pointlist[1]  = 0;
    in.pointlist[2]  = 0;
    in.pointlist[3]  = 2;  // node 2.
    in.pointlist[4]  = 0;
    in.pointlist[5]  = 0;
    in.pointlist[6]  = 2;  // node 3.
    in.pointlist[7]  = 2;
    in.pointlist[8]  = 0;
    in.pointlist[9]  = 0;  // node 4.
    in.pointlist[10] = 2;
    in.pointlist[11] = 0;
    // Set node 5, 6, 7, 8.
    for (i = 4; i < 8; i++) {
      in.pointlist[i * 3]     = in.pointlist[(i - 4) * 3];
      in.pointlist[i * 3 + 1] = in.pointlist[(i - 4) * 3 + 1];
      in.pointlist[i * 3 + 2] = 12;
    }

    in.numberoffacets = 6;
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    in.facetmarkerlist = new int[in.numberoffacets];

    // Facet 1. The leftmost facet.
    f = &in.facetlist[0];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 1;
    p->vertexlist[1] = 2;
    p->vertexlist[2] = 3;
    p->vertexlist[3] = 4;

    // Facet 2. The rightmost facet.
    f = &in.facetlist[1];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 5;
    p->vertexlist[1] = 6;
    p->vertexlist[2] = 7;
    p->vertexlist[3] = 8;

    // Facet 3. The bottom facet.
    f = &in.facetlist[2];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 1;
    p->vertexlist[1] = 5;
    p->vertexlist[2] = 6;
    p->vertexlist[3] = 2;

    // Facet 4. The back facet.
    f = &in.facetlist[3];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 2;
    p->vertexlist[1] = 6;
    p->vertexlist[2] = 7;
    p->vertexlist[3] = 3;

    // Facet 5. The top facet.
    f = &in.facetlist[4];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 3;
    p->vertexlist[1] = 7;
    p->vertexlist[2] = 8;
    p->vertexlist[3] = 4;

    // Facet 6. The front facet.
    f = &in.facetlist[5];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 4;
    p->vertexlist[1] = 8;
    p->vertexlist[2] = 5;
    p->vertexlist[3] = 1;

    // Set 'in.facetmarkerlist'

    in.facetmarkerlist[0] = 0;
    in.facetmarkerlist[1] = 0;
    in.facetmarkerlist[2] = 0;
    in.facetmarkerlist[3] = 0;
    in.facetmarkerlist[4] = 0;
    in.facetmarkerlist[5] = 0;
    ///////////////////////////////////////// koniec z palca /////////////////////////////////////////////////

//wywołania dla danych z palca
    tetrahedralize("pq",&in,&out);
//    tetrahedralize("pq1.414a0.1", &in, &out );
////////////////////////////zapisywanie kontrolne danych z palca//////////////////////////////////////////////
//    in.save_nodes("barin");
//    in.save_poly("barin");
//    out.save_nodes("barout");
//    out.save_elements("barout");
//    out.save_faces("barout");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
    //wywołanie dla wczytanych obiektów
    tetgenio external;
    char* charFlags = new char[flags.length()];;

    if ( exSplitting ){
        flags.copy(charFlags,flags.size(),0);
            cout<<"do tetgena 1 dajemy flagi \""<<charFlags<<"\"" <<endl;
        //wczytanie siatki zewnętrznej
        external = *getTetGenInput(exGrid);
        tetrahedralize("q1.4a0.1", &external, &in);

        flags.append("Y");
        flags.copy(charFlags,flags.size(),0);
            cout<<"do tetgena 2 dajemy flagi \""<<charFlags<<"\"" <<endl;
        external = *addHolesToTetInput( &in );
        tetrahedralize(charFlags, &external, &out);
    } else {
        flags.copy(charFlags,flags.size(),0);
            cout<<"do tetgena dajemy flagi \""<<charFlags<<"\"" <<endl;
        in = *getTetGenInput();
        tetrahedralize(charFlags, &in, &out);
    }

#endif
    //zwrócenie siatki przestrzennej z tetgena
    clearJoiner();
    return getGrid(&out);
}

// mapowanie wyjścia tetgena na Grid
Grid* MeshJoiner::getGrid(tetgenio *out)
{
    Grid* grid = new Grid();
    grid->setGridName("JoinedMesh");
    grid->NumberOfSpaceDim = 3;
    grid->_nodes.resize( out->numberofpoints );
    grid->attr["etype"] = "T4n3D";

    int i;

    //point from tetgenio to grid
    for ( i=0; i< out->numberofpoints; i++ ){
        grid->_nodes[i].nr = i+1;
        grid->_nodes[i]._parent = grid;
        grid->_nodes[i][0] = out->pointlist[i * 3];
        grid->_nodes[i][1] = out->pointlist[i * 3 + 1];
        grid->_nodes[i][2] = out->pointlist[i * 3 + 2];
    }

    //właściwe wyciąganie siatki czworościennej
    //tetrahedrons to grid
    grid->_elems.resize( out->numberoftetrahedra );
    for ( i = 0; i< out->numberoftetrahedra ; i++ ){
        Elem *e;
        e = new ElemT4n3D( grid );
        ((ElemT4n3D*)e)->setNodes( out->tetrahedronlist[i * 4],
                                   out->tetrahedronlist[i * 4 + 1],
                                   out->tetrahedronlist[i * 4 + 2],
                                   out->tetrahedronlist[i * 4 + 3]);
        grid->_elems[ i ] = e;
        e->nr = i+1;
        grid->addElemToSubdomain(i+1,1);
    }


    ////tylko testowe!!!!!!!! siatka powierzchniowa
    //facets to grid
//    grid->attr["etype"] = "T3n2D";
//    grid->_elems.resize( out->numberoffacets );
//    for ( i = 0; i< out->numberoffacets ; i++ ){
//        Elem *e;
//        e = new ElemT3n2D( grid );
//        ((ElemT3n2D*)e)->setNodes( out->facetlist[i].polygonlist->vertexlist[0],
//                                   out->facetlist[i].polygonlist->vertexlist[1],
//                                   out->facetlist[i].polygonlist->vertexlist[2]);
//        grid->_elems[ i ] = e;
//        e->nr = i+1;
//        grid->addElemToSubdomain(i+1,1);
//    }
    return grid;
}

//wyciąganie tylko zewnętrznego stla do "podziabania"
tetgenio* MeshJoiner::getTetGenInput( Grid * grid)
{
    tetgenio* in = new tetgenio();

    cout << "getTetGenInput for "<< grid <<endl;
    tetgenio::facet *f;
    tetgenio::polygon *p;
    int i;

    // All indices start from 1.
    in->firstnumber = 1;
/////////////////////////////////////////////// punkty //////////////////////////////////////////////////////////
    //tworzenie tablicy na wszystkie punkty
    in->numberofpoints = grid->getNoNodes();
    in->pointlist = new REAL[in->numberofpoints * 3];

    //przepisanie siatki zewnętrznej
    for ( i=0; i< grid->getNoNodes(); i++ ){
        in->pointlist[i * 3]     = grid->getNode(i+1)->x();
        in->pointlist[i * 3 + 1] = grid->getNode(i+1)->y();
        in->pointlist[i * 3 + 2] = grid->getNode(i+1)->z();
    }
////////////////////////////////////////////// elementy /////////////////////////////////////////////////////////
    in->numberoffacets = grid->getNoElems();
    in->facetlist = new tetgenio::facet[in->numberoffacets];
    in->facetmarkerlist = new int[in->numberoffacets];

    //przepisanie siatki zewnętrznej
    for ( i = 0; i< grid->getNoElems(); i++ ){
        f = &in->facetlist[i];
        f->numberofpolygons = 1;
        f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
        f->numberofholes = 0;
        f->holelist = NULL;
        p = &f->polygonlist[0];
        p->numberofvertices = 3;
        p->vertexlist = new int[p->numberofvertices];
        p->vertexlist[0] = grid->getElem(i+1)->_nodes[0];
        p->vertexlist[1] = grid->getElem(i+1)->_nodes[1];
        p->vertexlist[2] = grid->getElem(i+1)->_nodes[2];

        in->facetmarkerlist[i]=0;
    }

    return in;
}

//dodawanie dziur do bardziej gęstej siatki zewnętrznej
tetgenio* MeshJoiner::addHolesToTetInput( tetgenio* oldIn )
{
    tetgenio* in = new tetgenio();

    cout << "adding holes to TetGenInput "<< oldIn->numberofpoints <<" punktów i "<<oldIn->numberoftrifaces << endl;
    tetgenio::facet *f;
    tetgenio::polygon *p;
    int i;

    // All indices start from 1.
    in->firstnumber = 1;
/////////////////////////////////////////////// punkty //////////////////////////////////////////////////////////
    //tworzenie tablicy na wszystkie punkty
    in->numberofpoints = getNoOfAllNodes() - exGrid->getNoNodes() + oldIn->numberofpoints;
    in->pointlist = new REAL[in->numberofpoints * 3];

    //przepisanie siatki zewnętrznej z oldIn
    for ( i=0; i< oldIn->numberofpoints; i++ ){
        in->pointlist[i * 3]     = oldIn->pointlist[i * 3];
        in->pointlist[i * 3 + 1] = oldIn->pointlist[i * 3 + 1];
        in->pointlist[i * 3 + 2] = oldIn->pointlist[i * 3 + 2];
    }
    int filledPoints = oldIn->numberofpoints;

    //dopisanie dziur
    in->numberofholes = holes.size();
    in->holelist = new double[in->numberofholes*3];
    //iterowanie po dziurach
    for ( i=0; i < holes.size() ; i++ ){
        //iterowanie po punktach dziury
        for ( int j=0; j<((Grid*)holes.at(i))->getNoNodes(); j++ ){
            in->pointlist[3*filledPoints + j * 3]     = ((Grid*)holes.at(i))->getNode(j+1)->x();
            in->pointlist[3*filledPoints + j * 3 + 1] = ((Grid*)holes.at(i))->getNode(j+1)->y();
            in->pointlist[3*filledPoints + j * 3 + 2] = ((Grid*)holes.at(i))->getNode(j+1)->z();
        }
        filledPoints += ((Grid*)holes.at(i))->getNoNodes();

        //zapisywanie jako dziura
        Grid* currentHole = (Grid*)holes.at(i);
        double p[3];
        getPointInHole(currentHole, p);

        in->holelist[i*3] = p[0];
        in->holelist[i*3+1] = p[1];
        in->holelist[i*3+2] = p[2];
    }

////////////////////////////////////////////// elementy /////////////////////////////////////////////////////////
    in->numberoffacets = getNoOfAllElems() - exGrid->getNoElems() + oldIn->numberoftrifaces;
    in->facetlist = new tetgenio::facet[in->numberoffacets];
    in->facetmarkerlist = new int[in->numberoffacets];

    //przepisanie siatki zewnętrznej
    for ( i = 0; i< oldIn->numberoftrifaces; i++ ){
        f = &in->facetlist[i];
        f->numberofpolygons = 1;
        f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
        f->numberofholes = 0;
        f->holelist = NULL;
        p = &f->polygonlist[0];
        p->numberofvertices = 3;
        p->vertexlist = new int[p->numberofvertices];
        p->vertexlist[0] = oldIn->trifacelist[3*i];
        p->vertexlist[1] = oldIn->trifacelist[3*i+1];
        p->vertexlist[2] = oldIn->trifacelist[3*i+2];

        in->facetmarkerlist[i]=0;
    }

    filledPoints = oldIn->numberofpoints;
    int filledFacet = oldIn->numberoftrifaces;
    //dopisanie dziur
    //iterowanie po dziurach
    for ( i=0; i < holes.size() ; i++ ){
        //iterowanie po elementach dziury
        for ( int j=0; j<((Grid*)holes.at(i))->getNoElems(); j++ ){
            f = &in->facetlist[filledFacet + j];
            f->numberofpolygons = 1;
            f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
            f->numberofholes = 0;
            f->holelist = NULL;
            p = &f->polygonlist[0];
            p->numberofvertices = 3;
            p->vertexlist = new int[p->numberofvertices];
            p->vertexlist[0] = ((Grid*)holes.at(i))->getElem(j+1)->_nodes[0]+filledPoints;
            p->vertexlist[1] = ((Grid*)holes.at(i))->getElem(j+1)->_nodes[1]+filledPoints;
            p->vertexlist[2] = ((Grid*)holes.at(i))->getElem(j+1)->_nodes[2]+filledPoints;

            in->facetmarkerlist[i]=0;
        }
        filledFacet += ((Grid*)holes.at(i))->getNoElems();
        filledPoints += ((Grid*)holes.at(i))->getNoNodes();
    }

    return in;
}

//sklejanie "skorupek" w jedną siatkę, zwracanie wejścia do tetgena
tetgenio* MeshJoiner::getTetGenInput()
{
    tetgenio* in = new tetgenio();

    cout << "getTetGenInput"<<endl;
    tetgenio::facet *f;
    tetgenio::polygon *p;
    int i;

    // All indices start from 1.
    in->firstnumber = 1;
/////////////////////////////////////////////// punkty //////////////////////////////////////////////////////////
    //tworzenie tablicy na wszystkie punkty
    in->numberofpoints = getNoOfAllNodes();
    in->pointlist = new REAL[in->numberofpoints * 3];

    //przepisanie siatki zewnętrznej
    for ( i=0; i< exGrid->getNoNodes(); i++ ){
        in->pointlist[i * 3]     = exGrid->getNode(i+1)->x();
        in->pointlist[i * 3 + 1] = exGrid->getNode(i+1)->y();
        in->pointlist[i * 3 + 2] = exGrid->getNode(i+1)->z();
    }
    int filledPoints = exGrid->getNoNodes();

    //dopisanie dziur
    in->numberofholes = holes.size();
    in->holelist = new double[in->numberofholes*3];
    //iterowanie po dziurach
    for ( i=0; i < holes.size() ; i++ ){
        //iterowanie po punktach dziury
        for ( int j=0; j<((Grid*)holes.at(i))->getNoNodes(); j++ ){
            in->pointlist[3*filledPoints + j * 3]     = ((Grid*)holes.at(i))->getNode(j+1)->x();
            in->pointlist[3*filledPoints + j * 3 + 1] = ((Grid*)holes.at(i))->getNode(j+1)->y();
            in->pointlist[3*filledPoints + j * 3 + 2] = ((Grid*)holes.at(i))->getNode(j+1)->z();
        }
        filledPoints += ((Grid*)holes.at(i))->getNoNodes();

        //zapisywanie jako dziura
        Grid* currentHole = (Grid*)holes.at(i);
        double p[3];
        getPointInHole(currentHole, p);

        in->holelist[i*3] = p[0];
        in->holelist[i*3+1] = p[1];
        in->holelist[i*3+2] = p[2];
    }

////////////////////////////////////////////// elementy /////////////////////////////////////////////////////////
    in->numberoffacets = getNoOfAllElems();
    in->facetlist = new tetgenio::facet[in->numberoffacets];
    in->facetmarkerlist = new int[in->numberoffacets];

    //przepisanie siatki zewnętrznej
    for ( i = 0; i< exGrid->getNoElems(); i++ ){
        f = &in->facetlist[i];
        f->numberofpolygons = 1;
        f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
        f->numberofholes = 0;
        f->holelist = NULL;
        p = &f->polygonlist[0];
        p->numberofvertices = 3;
        p->vertexlist = new int[p->numberofvertices];
        p->vertexlist[0] = exGrid->getElem(i+1)->_nodes[0];
        p->vertexlist[1] = exGrid->getElem(i+1)->_nodes[1];
        p->vertexlist[2] = exGrid->getElem(i+1)->_nodes[2];

        in->facetmarkerlist[i]=0;
    }

    filledPoints = exGrid->getNoNodes();
    int filledFacets = exGrid->getNoElems();
    //dopisanie dziur
    //iterowanie po dziurach
    for ( i=0; i < holes.size() ; i++ ){
        //iterowanie po elementach dziury
        for ( int j=0; j<((Grid*)holes.at(i))->getNoElems(); j++ ){
            f = &in->facetlist[filledFacets + j];
            f->numberofpolygons = 1;
            f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
            f->numberofholes = 0;
            f->holelist = NULL;
            p = &f->polygonlist[0];
            p->numberofvertices = 3;
            p->vertexlist = new int[p->numberofvertices];
            p->vertexlist[0] = ((Grid*)holes.at(i))->getElem(j+1)->_nodes[0]+filledPoints;
            p->vertexlist[1] = ((Grid*)holes.at(i))->getElem(j+1)->_nodes[1]+filledPoints;
            p->vertexlist[2] = ((Grid*)holes.at(i))->getElem(j+1)->_nodes[2]+filledPoints;

            in->facetmarkerlist[i]=0;
        }
        filledFacets += ((Grid*)holes.at(i))->getNoElems();
        filledPoints += ((Grid*)holes.at(i))->getNoNodes();
    }

    return in;
}

void MeshJoiner::getPointInHole( Grid* currentHole , double p[3]){
    double divider = 0.01;
    int iterations = 0;
    srand ( time(NULL) );

//    int number=12;
    int number = rand() % currentHole->getNoElems();

//    cout << "\nwylosowany numerek trójkąta to: " << number << endl << endl;

    while ( 1 ) {
        if ( iterations > 5){
            divider = 0.01;
            iterations=0;
            number = rand() % currentHole->getNoElems();
//            cout << "\nnowy wylosowany numerek trójkąta to: " << number << endl;
        }
        ElemT3n2D* facet = ((ElemT3n2D*)currentHole->_elems[number]);
        double n[3];
        double minlength=9999999;
        for(int j=0; j < 3 ; j++){
            double len = DIST_Point_Point(((Node)currentHole->_nodes[facet->_nodes[j]])._Coords,((Node)currentHole->_nodes[facet->_nodes[j+1<3?j+1:0]])._Coords);
            if (len < minlength)
                minlength = len;
        }
//        cout << "najmniejsza długość boku to: " << minlength << endl;
        vector<double> trCenter = facet->center();
        facet->getNormalOfFace(0,n);

        p[0] = trCenter.at(0) - divider * minlength * n[0];
        p[1] = trCenter.at(1) - divider * minlength * n[1];
        p[2] = trCenter.at(2) - divider * minlength * n[2];

        double inf[3];
        inf[0] = trCenter.at(0) + 9999999 * n[0];
        inf[1] = trCenter.at(1) + 9999999 * n[1];
        inf[2] = trCenter.at(2) + 9999999 * n[2];

        iterations++;

//        if (countCuts(currentHole, trCenter.data(),p) % 2 == 0 )
        if (countCuts(currentHole, inf,p) % 2 == 1 ) {
            break;
        } else {
            divider/=2;
        }
    }
}


int MeshJoiner::countCuts(Grid *currentHole, double *point1, double* point2)
{
    double pp[3];
    double t;
    int cuts=0;
    for ( int i=0; i< currentHole->getNoElems(); i++){
        ElemT3n2D* element = ((ElemT3n2D*)currentHole->_elems[i]);
        double normal[3];

        element->getNormalOfFace(0,normal);
//        if (ISEC_Line_Plane( element->center().data(), normal, point1, point2, pp) == 1) {
        if (ISEC_Segment_Plane( element->center().data(), normal, point1, point2, pp, t) == 1) {
            if (IS_Point_In_Tri(pp, currentHole->getNode(element->_nodes[0])->_Coords,
                                    currentHole->getNode(element->_nodes[1])->_Coords,
                                    currentHole->getNode(element->_nodes[2])->_Coords)) {
                cuts++;
            }
        }

    }
    return cuts;
}

void MeshJoiner::addGrid(Grid* g){
    cout << "new grid base " << g->gridName() << endl;
    exGrid->copyFrom( *g );
}

void MeshJoiner::addHole(Grid* g ){
    cout<< "adding hole " << g->gridName() <<endl;
    holes.push_back(g);
}

int MeshJoiner::getNoOfAllNodes()
{
    int result=0;
    Grid* gr;

    result += exGrid->getNoNodes();

    for ( int i=0 ; i < holes.size() ; i++ ){
        gr = (Grid*) holes.at(i);
        result += gr->getNoNodes();
    }
    return result;
}

int MeshJoiner::getNoOfAllElems()
{
    int result=0;
    Grid* gr;

    result += exGrid->getNoElems();
    for ( int i=0 ; i < holes.size() ; i++ ){
        gr = (Grid*) holes.at(i);
        result += gr->getNoElems();
    }

    return result;
}

void MeshJoiner::clearJoiner()
{
    this->exGrid = new Grid();
    this->holes.clear();
}

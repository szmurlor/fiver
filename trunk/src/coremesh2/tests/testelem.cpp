#include "../src/elemt4n3d.h"
#include "../src/elem.h"
#include "../src/grid.h"
#include "../src/node.h"


int main( int argc, char ** argv )
{
    double nv[3];
    Grid * g;
    
    Node * n;
    ElemT4n3D * e;
    
    g = new Grid();
    g->attr["Name"] = "Pierwsza siatka";
    g->attr["GridNum"] = "1";
    g->_nodes.resize(6);
    printf("Resized nodes...\n");
    g->setNodeCoords(1,0,0,0);
    g->setNodeCoords(2,1,0,0);
    g->setNodeCoords(3,0,1,0);
    g->setNodeCoords(4,0,0,1);
    g->setNodeCoords(5,0,0,-1);
    g->setNodeCoords(6,0,0,-2);
    g->addNewNode(-1,-1,-1);
    g->addNewNode(-1,-1,-1);
    g->addNewNode(-1,-1,-1);
    g->addNewNode(-1,-1,-1);
    printf("Finished adding nodes\n");
    g->debug();
    
    
    e = new ElemT4n3D( g );
    printf("e = %p\n", e );
    printf("-1\n");
    printf("e->_nodes.size() = %d\n",e->_nodes.size());
    e->setNode(0,1);
    printf("0\n");
    e->setNode(1,2);
    e->setNode(2,3);
    e->setNode(3,4);
    //e->setNodes(1,2,3,4);
    g->debug();
    g->addElem( e );
    g->debug();
    
    g->debug();
    
    e = new ElemT4n3D( g );
    e->setNodes(1,2,3,5);
    g->addElem( e );
    
    
    g->debug();
    
    g->neighbourElements();
    
    Elem * e2;
    e2 = g->getElem( 1 );
    int ie;
    CFace face(3);
    face.nodes[0] = 0;
    face.nodes[1] = 2;
    face.nodes[2] = 3;
    if ( (ie = e2->getNeighbourWithFace(face)) == 0 ) 
        printf("face 1,2,3 is external\n");
    else
        printf("face 1,2,3 is internal (neighbour = %d)\n", ie);
    
    return 0;
}
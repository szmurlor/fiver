#include <grid.h>
#include <elem.h>
#include <node.h>

#include <diffreader.h>


int main(int argc, char **argv)
{
    Grid g;
    DpGridFileInfo dgfi;
    DpReader dr;

    Node *n;
    int i;
    int size = 10;
    int nc = 0;


    dgfi.ReadFromFile(argv[1]);
    dr.ReadFromFile(&g, argv[1], dgfi.getInfo(0).position);

	g.debug();

	Grid g2;

	g2.copyFrom( g );

	g2.debug();

	return 0;
}

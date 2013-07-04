#include <stdio.h>
#include <grid.h>
#include <diffreader.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;
#define O_SWAP 0
#define O_CUT 1
#define O_LIST 2
#define O_BND 3

int swapSubdomains( char * sFileIn, char * sFileOut, char * sSubNr1, char * sSubNr2)
{
	Grid g;
	DpGridFileInfo dgfi;
	DpReader dr;
	int i;

	dgfi.ReadFromFile(sFileIn);
	dr.ReadFromFile(&g,sFileIn, dgfi.getInfo(0)->position);

	int old_sub, new_sub;
	old_sub = atoi(sSubNr1);
	new_sub = atoi(sSubNr2);
	for (i=0; i<g._elems.size(); i++) {
		if (g._elems[i]->subdomain() == old_sub)
			g._elems[i]->setSubdomain( new_sub, false );
	}
	dr.SaveToFile(&g,sFileOut);	

	return (1);
}

int addBndToSub(char * sFileIn, char * sFileOut, int optind, char ** argv)
{
	Grid g;
	DpGridFileInfo dgfi;
	DpReader dr;
	int i;
	std::vector<int> subs;

	dgfi.ReadFromFile(sFileIn);
	dr.ReadFromFile(&g,sFileIn, dgfi.getInfo(0)->position);

	int isubno, ibnd, ii = 3;
	isubno = atoi(argv[optind+2]);
	while (isubno-- > 0) 
		subs.push_back( atoi( argv[optind + ii++] ) );
	ibnd = atoi( argv[optind + ii] );	
	//ibnd = atoi(sBndNr);
	
	for (i=0; i < g._nodes.size(); i++) {
		g._nodes[i].Tag = 0;
	}

	for (i=0; i<g._elems.size(); i++) {
		for (ii = 0; ii < subs.size(); ii++) {
			if (g._elems[i]->subdomain() == subs[ii]) {
				for (int j=0; j < g._elems[i]->_nodes.size(); j++) {
					int n = g._elems[i]->_nodes[j] - 1;
					g._nodes[ n ].Tag = 1;
				}
			}
		}
	}

	g.addBoundary( ibnd );
	for (i=0; i < g._nodes.size(); i++) {
		if ( g._nodes[i].Tag == 0 ) {
			g._nodes[i].addBoundary( ibnd, false ); // do not register boundary.
		}
	}

	g.sortBoundaries();
	dr.SaveToFile(&g,sFileOut);	

	
	return (1);
}

int listSubdomains(char * sFileIn)
{
	Grid g;
	DpGridFileInfo dgfi;
	DpReader dr;
	int i;

	dgfi.ReadFromFile(sFileIn);
	dr.ReadFromFile(&g,sFileIn, dgfi.getInfo(0)->position);
		
	for (i=0; i<g._subdomains.size(); i++) {
		printf("Subdomain nr: %d\n", g._subdomains[i].nr);
	}

	return(1);
}

int cutSubdomain(char * sFileIn, char * sFileOut, char * sSubNr)
{
	Grid g, g2;
	DpGridFileInfo dgfi;
	DpReader dr;
	int i,j;
	int iSubNr;

	iSubNr = atoi(sSubNr);
	g2.NumberOfSpaceDim = 3;
		
	printf("Scanning grid in file: %s\n",sFileIn);
	dgfi.ReadFromFile(sFileIn);
	printf("Reading grid 0 from file: %s\n",sFileIn);
	dr.ReadFromFile(&g,sFileIn, dgfi.getInfo(0)->position);
	
	printf("Initializing Tag = 0 in original grid\n");
	for (i=0; i<g._nodes.size();i++)
		g._nodes[i].Tag = 0;

	printf("Inserting elements from SubNr = %d into new grid.(Marking nodes to copy with Tag = 1\n",iSubNr);
	for (i=0; i<g._elems.size();i++){
		if (g._elems[i]->subdomain() == iSubNr) {
			g2._elems.resize( g2._elems.size() + 1 );
			g2._elems[ g2._elems.size() - 1 ] = g._elems[i];
			g2._elems[ g2._elems.size() - 1 ]->nr = g2._elems.size();
			for (j=0;j<4;j++) 
				g._nodes[ g._elems[i]->_nodes[j] - 1 ].Tag = 1;
		}
	}


	printf("Copying nodes into new grid...\n");
	for (i=0; i <g._nodes.size();i++){
		if (g._nodes[i].Tag == 1) {
			g2._nodes.resize( g2._nodes.size() + 1);
			g2._nodes[ g2._nodes.size() - 1] = g._nodes[i];
			g2._nodes[ g2._nodes.size() - 1].nr =   g2._nodes.size();
			g._nodes[i].Tag = g2._nodes.size();
		}
	}

	printf("Renumerating references to nodes in new grid elements.\n");
	for (i=0;i<g2._elems.size();i++) {
		for (j=0;j<4;j++)
			g2._elems[i]->_nodes[j] = g._nodes[ g2._elems[i]->_nodes[j] - 1 ].Tag;
	}

	printf("Scanning for available boudaries...\n");
	g2.scanBoundaries();
	printf("Scanning for available subdomains...\n");
	g2.scanSubdomains();


	printf("Saving new grid into file: %s.\n",sFileOut);
	dr.SaveToFile(&g2,sFileOut);	

	printf("Finished.\n");
	return(1);
}

int main(int argc, char **argv)
{
	int iTryb;
	int o;
	
	if (argc<3) {
		printf("Wywo³anie:\n");
		printf("\tgrid_sub input.grid output.grid old_sub new_sub\n");
		printf("\t\tZamienia numer wybranego podobszaru na inny.\n");
		printf("\tgrid_sub -l input.grid\n");
		printf("\t\t  (listuje wszystkie podobszary)\n");
		printf("\tgrid_sub -c input.grid output.grid sub_nr\n");
		printf("\t\tWycina wybrany podobszar i zapisuje go jako inn± siatkê.\n");
		printf("\tgrid_sub -b input.grid output.grid subno sub1 .. subn bnd_nr\n");
		printf("\t\tDodaj warunek brzegowy do wszystkich wezlow, ktore nie naleza\n");
		printf("\t\tdo podbszaru o zadanym numerze.\n");
		printf("argc = %d\n", argc);
		exit(0);
	}
	iTryb = O_SWAP;
	while ((o=getopt(argc,argv,"clb")) != -1) {
		switch (o) {
			case 'c': iTryb = O_CUT;break;
			case 'l': iTryb = O_LIST;break;
			case 'b': iTryb = O_BND;break;
		}
	}

	switch (iTryb) {
		case O_SWAP : return swapSubdomains(argv[optind], argv[optind+1], argv[optind+2], argv[optind+3]);break;
		case O_LIST : return listSubdomains(argv[optind]);
		case O_CUT : return cutSubdomain(argv[optind],argv[optind+1],argv[optind+2]);
		case O_BND : return addBndToSub(argv[optind], argv[optind+1], optind, argv);
	}

	return (0);
}

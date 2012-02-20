#ifndef __GRID_H
#define __GRID_H

#include "meshobject.h"
#include "elem.h"
#include "geom.h"
#include "facelist.h"
#include "voxelcont.h"
#include "elemt4n3d.h"
#include "elemt3n2d.h"

#include <list>
#include <vector>
#include <set>
#include <map>

using namespace std;

struct IPoint 
{
	int x,y,z;
};

class Subdomain 
{
	protected:
		vector<int> _elems;

	public:
		int nr;
		char name[50];

		Subdomain() { name[0] = 0; nr = 0; };
		Subdomain(int nr);

		void addElem(int ie);	
		void setElemsCount(int icount);

		void compactGrid();

		int elemsCount() { return(_elems.size()); };
		void debug();
		Subdomain & operator=(const Subdomain & src);
};

class Boundary {
	public:
		vector<int> _nodes;
		int nr;								
		char name[50];

		void debug();

		Boundary & operator=(const Boundary & src);
};

class Grid : public MeshObject
{
	protected:
		string _gridname;
		// bounding box minial, bounding box maximal
		double bb_min[3], bb_max[3];
		VoxelCont *voxc;

		// rows: nodes, columns: elements to which each node belongs
		vector< vector<int> > _nodeNbrs;

		// dim: noElems * getFacesCount() (eg. for tetras: noElems * 4)
		// contains information for each element about its neighbours.
		// simply for tetras these are four neighbour element numbers,
		// one for each face of the element
		int noFacesInElem;
		vector< int > _elemNbrs;
		vector< int > _elemFaceHashes;
		
	public:
		map<string,string> attr;
		vector<Elem*> _elems;
		vector<Node> _nodes;
		vector<Boundary> _bnds;
		vector<Subdomain> _subdomains;
		int NumberOfSpaceDim;
		
		Grid();
		virtual ~Grid();

		virtual void copyFrom( Grid & src );
		virtual void clear();

		void setGridName(string aname);
		string gridName() { return _gridname; };

		int getNoNodes() { return _nodes.size(); };
		int getNoElems() { return _elems.size(); };

		void addElemToSubdomain(int elemnr, int subdomain_nr);
		void addNodeToBoundary(int nodenr, int boundary_nr);
		void addNodeToBoundaryIndex(int nodenr, int boundary_index);
		int indexOfBoundary( int boundary_nr );
		int indexOfSubdomain( int subdomain_nr );

		void compactGrid();

		/* build information about element's neighbourhood over faces,
			function is usefull only for 3D grids.
			does nothing for 2D grids. */
		void nbrElems();
		void nbrNodes() { nodes2Elems(); };
                void checkNbrs();
		/* build information which elems are touching each node. */
		void nodes2Elems();

		void boundaryNodes(int _bnd); // mark boundary nodes
		void boundaryNodesSub( int _bnd, int _sub );

		/* Skanuj wezly w poszukiwaniu niezarejestrowanych w glownej siatce 
		 * warunkow brzegowych */
		void scanBoundaries();

		/* Skanuj elementy w poszukiwaniu niezarejestrowanych w glownej
		 * siatce podobszarow*/
		void scanSubdomains();

		/* Sprawdz czy dany podobszar lub warunek brzegowy 
		 * znajduje sie na liscie. Jezeli nie znajduje sie dodaj 
		 * do listy.*/
		void addSubdomain(int _snr);
		void addBoundary(int _bnr);
		void sortBoundaries();
		
		Node * addNewNode(double coords[3]);
        Node * addNewNode( double x, double y, double z );
        Node * setNodeCoords( int in, double x, double y, double z );
		Elem * addNewElemT4n3D(int SubNr, int n1, int n2, int n3, int n4);
		Elem * addNewElemT3n2D(int SubNr, int n1, int n2, int n3);
        Elem * addElem( Elem * e );
		
		// 1 - oznacza pierwszy element.
		Elem * getElem(int nr) 
		   { return( _elems[ nr - 1]); }
		Node * getNode(int nr)
		   { return( &_nodes[ nr - 1]); }
		
		/** Divide volume on size^3 cubes. This will speed up
		    the search procedure for getElemPtv		*/
		void prepareElemPtv(int size);
		Elem * getElemPtv( double P[3] );

		/** bounding box of grid */
		void getBBox(double minP[3], double maxP[3]);
                void getCenter( double P[3] );
			 
		/** 	Dodaj wezel do siatki.
				Pobierz element sasiedni.
				Uwotrz nowe elemnty w ElemNr
				Utworz nowe elementy w siasiednim
				Usun Element ElemNr i element sasiedni.
		 */
		Node * addNodeOnFace(int ElemNr, int iNode1, int iNode2, int iNode3, double Coords[3] );
		Node * addNodeOnEdge(int ElemNr, int iNode1, int iNode2, double Coords[3]);
		void STLSaveFacesOnBnd(int _bnr, string FName, bool OnlyExternal = false);
		
		//bool isFaceExternal(CFace *face);
  
                void setElemsTag( int  );
                void setElemsTagForNbr( int  );

		virtual void debug();
		VoxelCont * _getVoxelPtv() { return voxc; };

		void addNodeNbr( int nr, int elem_nr );
		bool hasNodeNbr( int nr, int elem_nr );
		void removeNodeNbr( int nr, int elem_nr );
		vector<int> * getNodeNbrs( int nr );

		int getElemNbr( int nr, int iface );
		void setElemNbr( int nr, int iface, int inbr );
		
		int getElemFaceHash( int enr, int facenr);
		void setElemFaceHash( int enr, int facenr, int hash);

                void translate(double tr[3]);
                void rotate(double center[3] , double axis[3], double angle);

                //jstar:
                int elemSubdomain( int e ) { Elem * el = _elems[ e ]; return el->subdomain(); } // zwraca nr podobszaru dla el. nr e (e in <0,nElems-1>)
                void nodeSubdomains( int n, set<int>& ); // zwraca podobszary przylegające do wezla n (n in <0,nNodes-1>)

};

#endif 

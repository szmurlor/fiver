#ifndef _EDGE_LIST
#define _EDGE_LIST

#include "node.h"
#include "geom.h"
#include <stdio.h>

class CEdge 
{
	public:
		int n1;
		int n2;
		int Tag;
		double t0, t1;
		double p0[3], p1[3];
		
		CEdge *next;
		
		int cmp(int _n1, int _n2);
        CEdge & operator=( const CEdge & src );
};


class EdgeList
{
	public:		
		bool bAutoDelete; // = true, by default
		bool bDebug;
	
		EdgeList();
		~EdgeList();
		
		CEdge *find(int n1, int n2);
		CEdge *addNew();
		CEdge *addNew(int _n1, int _n2);
		CEdge *add( CEdge* pe);
		CEdge *getFirst() { return first; };
		void clear();
		int count();
		void sortT0T1();
		void debug();

		void removeEdgesWithNode( int n );

		/* Be careful, function adds to the el list pointers to original
		 * CEdge objects. (They are not copied, they are referenced by
		 * pointer.*/
		void buildListWithNode(int n1, EdgeList * el);

	
	protected:
		CEdge *first;
		CEdge *last;
		int iCount;
};


#endif

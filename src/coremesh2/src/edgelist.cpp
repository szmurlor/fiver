

#include "edgelist.h"
#include <stdlib.h>

EdgeList::EdgeList()
{
	iCount = 0;
	first = 0;
	last = 0;
	bAutoDelete = true;
	bDebug = false;
}

/*!
    \fn EdgeList::~EdgeList()
 */
EdgeList::~EdgeList()
{
    clear();
}
	

/*!
    \fn EdgeList::find(Node *n1, Node * n2)
 */
CEdge *EdgeList::find(int n1, int n2)
{	
	CEdge *tmp;
	
	tmp = first;
	while (tmp != 0) {
		if ((tmp->n1 == n1) && (tmp->n2 == n2))
			return tmp;
		if ((tmp->n2 == n1) && (tmp->n1 == n2))
			return tmp;
			
		tmp = tmp->next;
	}   
	
	return 0; 
}


/*!
    \fn EdgeList::addNew()
 */
CEdge *EdgeList::addNew()
{
	CEdge *tmp;
	tmp = new CEdge();
	tmp->next = 0;
	return( add( tmp ) );
}


/*!
    \fn EdgeList::add( PEdge pe)
 */
CEdge *EdgeList::add( CEdge *pe)
{
	if (last != 0)
		last->next = pe;
	if (first == 0)
		first = pe;
	last = pe;

	iCount++;
	return( pe );

}


/*!
    \fn EdgeList::clear()
 */
void EdgeList::clear()
{
	CEdge *tmp;
	tmp = first;
	while (tmp != 0) {		
		first = tmp->next;
		if (bAutoDelete == true) {
			delete(tmp);
		}
		tmp = first;
	}
	iCount = 0;
	first = 0;
	last = 0;
}




/*!
    \fn EdgeList::CEdge *addNew(Node * _n1, Node * _n2)
 */
CEdge * EdgeList::addNew(int _n1, int _n2)
{
    CEdge *t;
		t = addNew();
		t->n1 = _n1;
		t->n2 = _n2;
		t->Tag = 0;
		
		return(t);
}

void EdgeList::buildListWithNode(int n1, EdgeList * el)
{
	CEdge *e;
	el->clear();
	e = getFirst();
	while (e != 0) {
		if ((e->n1 == n1) || (e->n2 == n1))  
			el->addNew(e->n1, e->n2);
		e = e->next;
	}
}

void EdgeList::debug()
{
	int i = 1;
	CEdge *t = first;
	while (t!= 0) {
		printf("Edge[%d]: \t%d\t%d \t(%d)\n",i,t->n1, t->n2, t->Tag);
		printf("Edge[%d]: \tt0=%g\tt1=%g\n",i,t->t0, t->t1);
		t = t->next;
		i++;
	}
}

int CEdge::cmp(int _n1, int _n2)
{
	if ((_n1 == n1) && (_n2==n2))
		return 0;
	if ((_n1 == n2) && (_n2==n1))
		return 1;
	return -1;
}

#define MIN(a,b) ((a)<(b)?(a):(b));

int compare_edge_t0t1( const void * p1, const void * p2)
{
	CEdge **e1 = (CEdge**) p1;
	CEdge **e2 = (CEdge**) p2;
	double t1, t2;
	t1 = MIN((*e1)->t0, (*e1)->t1);
	t2 = MIN((*e2)->t0, (*e2)->t1);

	if (t1 == t2) return 0;
	if (t1 > t2) return 1;
	return -1;
	
}

void EdgeList::removeEdgesWithNode( int n )
{
	CEdge *t, *tn;

	t = 0;
	tn = first;
	while (tn != 0) {
		if (( tn->n1 == n ) || ( tn->n2 == n )) {
			if (t != 0) {
				t->next = tn->next;
				delete(tn);
				tn= t->next;
			} else {
				first = tn->next;
				delete(tn);
				tn = first;
			}
		} else {
			t = tn;
			tn = tn->next;
		}
	}
}

int EdgeList::count() 
{
	CEdge * te;
	int c = 0;
	te = getFirst();
	while (te != 0) {
		te = te->next;
		c++;
	}
	return(c);
	
}

void EdgeList::sortT0T1()
{
	CEdge ** ve;
	int i;
	CEdge * te;
	int mcount = count();

	if (bDebug) printf("Allocating %d bytes\n",mcount*sizeof(CEdge*));
	if ((ve = (CEdge**) malloc(mcount*sizeof(CEdge*)) ) == NULL) {
		printf("Error! Unable to allocate memory for qsort.\n");
		return;
	}

	if (bDebug) printf("Filling array of pointers for qsort...\n");
	te = getFirst();
	for (i=0; i< mcount; i++) {
		ve[i] = te;
		te = te->next;
	}

	if (bDebug) printf("Starting qsort procedure...\n");
	qsort( ve, mcount, sizeof(CEdge*), compare_edge_t0t1);
	
	if (bDebug) printf("Reconstructing sorted list...\n");
	first = ve[0];
	last = ve[mcount - 1];
	te = first;
	last->next = 0;
	for (i=1; i< mcount; i++) {
		te->next = ve[i];
		te = te->next;
	}
	
	if (bDebug) printf("releasing memory allocated for array...\n");
	free( ve );

	if (bDebug) printf("Finished sortT0T1.\n");
}

CEdge & CEdge::operator =( const CEdge & src )
{
    n1 = src.n1;
    n2 = src.n2;
    Tag = src.Tag;
    t0 = src.t0;
    t1 = src.t1;
    next = src.next;
    SET(p0, src.p0);
    SET(p1, src.p1);

	return *this;
}

#include "nodelist.h"

CNode::CNode()
{
	elist = 0;
	next = 0;
	Tag = 0;
	nr = 0;
}

CNode::~CNode()
{
	if (elist != 0) {
		elist->bAutoDelete = true;
		elist->clear();
		delete(elist);
	}
}

CEdge * CNode::addEdge(int n1, int n2)
{
	CEdge * e;
	if (elist==0) elist = new EdgeList;
	
	e = elist->find(n1,n2);
	if (e==0)
		e = elist->addNew(n1,n2);
	return (e);
}

NodeList::NodeList()
{
	iCount = 0;
	first = 0;
	last = 0;
	bAutoDelete = true;
}

/*!
    \fn NodeList::~NodeList()
 */
NodeList::~NodeList()
{
    clear();
}

/*!
    \fn NodeList::find(Node *n1, Node * n2)
 */
CNode *NodeList::find(int n)
{	
	CNode *tmp;
	
	tmp = first;
	while (tmp != 0) {
		if (tmp->nr == n)
			return tmp;
			
		tmp = tmp->next;
	}   
	
	return 0; 
}


/*!
    \fn NodeList::addNew()
 */
CNode *NodeList::addNew()
{
	CNode *tmp;
	tmp = new CNode();
	return( add( tmp ) );
}


/*!
    \fn NodeList::add( PNode pe)
 */
CNode *NodeList::add( CNode *pn)
{
	if (last != 0)
		last->next = pn;
	if (first == 0)
		first = pn;
	last = pn;

	return( pn );

	iCount++;
}


/*!
    \fn NodeList::clear()
 */
void NodeList::clear()
{
	CNode *tmp;
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
    \fn NodeList::CNode *addNew(Node * _n1, Node * _n2)
 */
CNode * NodeList::addNew(int n)
{
	CNode *t;
	t = addNew();
	t->nr = n;
	
	return(t);
}

void NodeList::debug()
{
	int i = 1;
	CNode *t = first;
	while (t!= 0) {
		printf("Node[%d].\n",t->nr);
		if (t->elist != 0) {
			CEdge *e;
			e = t->elist->getFirst();
			while (e != 0) {
				printf("\tEdge:\t%d\t%d\n",e->n1,e->n2);
				e = e->next;
			}
		}
		t = t->next;
		i++;
	}
}

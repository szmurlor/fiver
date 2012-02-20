//
// C++ Implementation: elemlist
//
// Description: 
//
//
// Author: Robert Szmurlo <szmurlor@iem.pw.edu.pl>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "elemlist.h"

ElemList::ElemList()
{
	iCount = 0;
	_last = 0;
	_first = 0;
	bAutoDelete = true;
}


ElemList::~ElemList()
{
	clear();
	_first = 0;
}


/*!
    \fn ElemList::find(int n1, int n2, int n3)
 */
CElem *ElemList::find(int n1, int n2, int n3, int n4)
{
	CElem *tmp;
	
	tmp = _first;
	while (tmp != 0) {
		if (tmp->cmp(n1,n2,n3,n4) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}
	
	return 0; 
}

CElem *ElemList::find(int nr)
{	
	CElem *tmp;
	
	tmp = _first;
	while (tmp != 0) {
		if (tmp->cmp(nr) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}   
	
	return 0; 
}

/*!
    \fn NodeList::addNew()
 */
CElem *ElemList::addNew()
{
	CElem *tmp;
	tmp = new CElem();
	tmp->next = 0;
	tmp->Tag = 0;
	return( add( tmp ) );
}


/*!
    \fn NodeList::add( PNode pe)
 */
CElem *ElemList::add( CElem *pn)
{
	if (_last != 0)
		_last->next = pn;
	if (_first == 0)
		_first = pn;
	_last = pn;

	return( pn );

	iCount++;
}


/*!
    \fn NodeList::clear()
 */
void ElemList::clear()
{
	CElem *tmp;
	tmp = _first;
	while (tmp != 0) {		
		_first = tmp->next;
		if (bAutoDelete == true) {
			delete(tmp);
		}
		tmp = _first;
	}
	iCount = 0;
	_first = 0;
	_last = 0;
}




/*!
    \fn NodeList::CNode *addNew(Node * _n1, Node * _n2)
 */
CElem * ElemList::addNew(int nr, int n1, int n2, int n3, int n4)
{
	CElem *t;
	t = addNew();
	t->n[0] = n1;
	t->n[1] = n2;
	t->n[2] = n3;
	t->n[3] = n4;
	t->nr = nr;
	
	return(t);
}

CElem * ElemList::addNew(int nr)
{
	return(addNew(nr,0,0,0,0));
}

void ElemList::debug()
{
	int i = 1;
	CElem *t = _first;
	while (t!= 0) {
		t->debug();		
		t = t->next;
		i++;
	}
}

int ElemList::length()
{
	int i = 0;
	CElem *t = _first;
	while (t!=0) {
		t = t->next;
		i++;
	}
	return (i);
}

void ElemList::compact(int Tag)
{
	CElem *t = _first;
	CElem *p = 0,*n = 0;
	while (t!= 0) {
		n = t->next;
		if (t->Tag == Tag) {
			if (p != 0) p->next = n;
			else _first = n;
			if (n==0) _last = p;
			if (bAutoDelete) delete t;
			t = n;
			iCount--;
			
		} else {
			p = t;
			t = n;
		}
		if (t != 0) n = t->next;
		else n = 0;
	}
}

int CElem::cmp(int n1, int n2, int n3, int n4)
{
	bool b1,b2,b3,b4;
	int i;
	b1 = b2 = b3 = false;
	for (i=0;i<4;i++) if (n[i] == n1) b1 = true;
	for (i=0;i<4;i++) if (n[i] == n2) b2 = true;
	for (i=0;i<4;i++) if (n[i] == n3) b3 = true;
	for (i=0;i<4;i++) if (n[i] == n4) b4 = true;
	
	if ((b1==true) && (b2==true) && (b3==true))
		return 0;
	else
		return -1;
}

int CElem::cmp(int nr)
{
	if (this->nr == nr)
		return 0;
	else
		return -1;
}

void CElem::debug()
{
	printf("Elem[%d]: %d, %d, %d, %d\n",nr,n[0], n[1], n[2], n[3]);
}


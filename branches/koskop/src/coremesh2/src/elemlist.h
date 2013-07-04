//
// C++ Interface: elemlist
//
// Description: 
//
//
// Author: Robert Szmurlo <szmurlor@iem.pw.edu.pl>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ELEMLIST_H
#define ELEMLIST_H

#include <stdio.h>

class CElem{
public:
	int nr;
	int n[4];
	CElem * next;
	int Tag;
	
	int cmp(int nr);
	int cmp(int n1, int n2, int n3, int n4);
	void debug();
};

/**
@author Robert Szmurlo
*/
class ElemList{

protected:
	CElem * _first;
	CElem * _last;
	int iCount;	
	
public:
	bool bAutoDelete;
	ElemList();

  	~ElemList();
	
	CElem * getFirst() { return(_first); };
	
	CElem * find(int n1, int n2, int n3, int n4);
	CElem * find(int nr);
	CElem * addNew();
	CElem * addNew(int nr, int n1, int n2, int n3, int n4);
	CElem * addNew(int nr); 
	CElem * add(CElem * pn);
	void clear();
	void compact(int Tag);
	
	int length();
	int count() { return(length()); };
	
	void debug();
};

#endif

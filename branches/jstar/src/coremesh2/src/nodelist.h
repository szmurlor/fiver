#ifndef _NODE_LIST_H
#define _NODE_LIST_H

#include "edgelist.h"

class CNode {
public:
	int nr;
	int Tag;
	EdgeList *elist;
	CNode *next;
	
	CNode();
	~CNode();
	
	CEdge * addEdge(int n1, int n2);
	
};

class NodeList {
public:
	bool bAutoDelete;
	
	NodeList();
	~NodeList();
	
	CNode * find(int n);
	CNode * addNew();
	CNode * addNew(int n);
	CNode * add(CNode * pn);
	CNode * getFirst() { return first; };
	void clear();
	
	void debug();
protected:
	CNode *first;
	CNode *last;
	int iCount;
};
#endif

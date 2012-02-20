#include "gridreader.h"

GridReader::GridReader()
{
	FReaderName = "Undefined.";
}

string GridReader::ReaderName()
{
	return(FReaderName);
}

GridFileInfo::GridFileInfo()
{
    _first = 0;
    _gridCount = 0;
}

GridFileInfo::~GridFileInfo()
{
    clear();
}

void GridFileInfo::clear()
{
    gridInfo *tmp = _first;
    
    while (tmp != 0) {
        _first = ((gridInfo*) tmp)->next;
        delete(tmp);
        tmp = _first;
    }
}

gridInfo * GridFileInfo::add()
{
    gridInfo * tmp;
    tmp = last();
    if (tmp == 0) {
        _first = new gridInfo;
        _first->next = 0;
        tmp = _first;
    } else {
        tmp->next = new gridInfo;
        tmp->next->next = 0;
        tmp = tmp->next;
    }
    _gridCount++;

    return(tmp);
}

gridInfo * GridFileInfo::first()
{
    return (_first);
}

gridInfo * GridFileInfo::last()
{
    gridInfo * tmp;
    tmp = _first;
    if (tmp == 0) return(0);
    
    while (tmp->next != 0)
        tmp = tmp->next;
        
    return (tmp);
}

void GridFileInfo::ReadFromFile(string )
{
    
}

int GridFileInfo::gridCount()
{
    return (_gridCount);
}

void GridFileInfo::debug()
{
    gridInfo *tmp;
    int i = 1;
    
    tmp = first();
    while (tmp != 0) {
        printf("[%d] ----------- \n",i);
        printf("[%d] Name = %s \n",i,tmp->name);
        printf("[%d] Position = %d \n",i,tmp->position);
        printf("[%d] NoOfSpaceDim = %d \n",i,tmp->NoOfSpaceDim);
        printf("[%d] NoOfNodes = %d \n",i,tmp->NoOfNodes);
        printf("[%d] NoOfElements = %d \n",i,tmp->NoOfElements);
        printf("[%d] ----------- \n",i);
        i++;
        tmp = tmp->next;
    }
}

gridInfo * GridFileInfo::getInfo(int i)
{
    int _i = 1;
    gridInfo * tmp;
    tmp = _first;
    while (_i<i) {
        if (tmp == 0) return 0;
        tmp = tmp->next;
        _i++;
    }
    return tmp;
}


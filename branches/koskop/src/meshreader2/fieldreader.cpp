#include <cstdio>
#include "fieldreader.h"

FieldReader::FieldReader()
{
}

FieldFileInfo::FieldFileInfo()
{
    _head = 0;
}

FieldFileInfo::~FieldFileInfo()
{
    clear();
}

void FieldFileInfo::clear()
{
    FieldInfo * t = _head;
    while (t != 0){
        _head = t->next;
        delete (t);
        t = _head;
    }
    _head = 0;
}

int FieldFileInfo::count( string fieldname )
{
	int ic = 0;

	FieldInfo * t = _head;
	while (t != 0){
		if (t->name == fieldname)
			ic++;
		t = t->next;
	}
	return ic;
}

FieldInfo * FieldFileInfo::addNew()
{
	FieldInfo * n = new FieldInfo;
	
	if (_head == 0) {
		_head = n;
	} else {
		FieldInfo * t = _head;
		while (t->next != 0) t=t->next;
		t->next = n;
	}
	return(n);
}

void FieldFileInfo::debug()
{
    int i = 1;
    FieldInfo * t = _head;
    while (t != 0){
        cout << "[" << i << "] fieldNum = " << t->fieldNum << endl;
        cout << "[" << i << "] name = " << t->name << endl;
        cout << "[" << i << "] description = " << t->description << endl;
        cout << "[" << i << "] fieldType = " << t->fieldType << endl;
        cout << "[" << i << "] sTime = " << t->sTime << endl;
        cout << "[" << i << "] component = " << t->component << endl;
        cout << "[" << i << "] dim = " << t->dim << endl;
        cout << "[" << i << "] position = " << t->position << endl;
        cout << "[" << i << "] size = " << t->size << endl;
        cout << "[" << i << "] gridRefNum = " << t->gridRefNum << endl;
        t = t->next;
        i++;
    }
}

FieldInfo * FieldFileInfo::getInfo(int i)
{
    int _i = 1;
    FieldInfo * tmp;
    tmp = _head;
    while (_i<i) {
        if (tmp == 0) return 0;
        tmp = tmp->next;
        _i++;
    }
    return tmp;
}

FieldInfo * FieldFileInfo::getInfo( string fieldname, int component )
{
	FieldInfo * t = _head;
	while (t != 0){
		if (t->name == fieldname) {
			component--;
			if (component == 0)
				return t;
		}
		t = t->next;
	}
    return 0;
}

vector<string> FieldFileInfo::getFieldNames()
{
	vector<string> tmp;
	
	FieldInfo * t = _head;
	while (t != 0){
		if (find(tmp,t->name) == tmp.end()) {
			printf("Found: %s\n", (t->name).c_str());
			tmp.push_back( t->name );
		}
		t = t->next;
	}
	
	return tmp;
}


vector<string>::iterator FieldFileInfo::find( vector<string> & tmp , string name)
{
	vector<string>::iterator it;
	for (it = tmp.begin(); it != tmp.end(); it++) {
		if ( (*it) == name ) return it;
	}
	return it;
}


vector<string> FieldFileInfo::getFieldTimes( string fname )
{
	vector<string> tmp;
	
	FieldInfo * t = _head;
	while (t != 0){
		if (t->name == fname ) {
			if (find(tmp, t->sTime) == tmp.end() )
				tmp.push_back( t->sTime );
		}
		t = t->next;
	}
	
	return tmp;
}

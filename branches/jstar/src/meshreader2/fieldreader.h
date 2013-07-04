#ifndef _FIELDREADER_H
#define _FIELDREADER_H

#include <simplefield.h>
#include <vector>
#include <string>

using namespace std;

class FieldInfo {
public:
	FieldInfo() {
		fieldNum = 0;
		component = 0;
		position = 0;
		size = 0;
		next = 0;
	};
	~FieldInfo() {};

	int fieldNum;
	string name;
	string description;
	string fieldType;
	string sTime;
	int component;
	int dim;
	int size;
	int position;
	int gridRefNum;

	FieldInfo * next;
};


class FieldFileInfo {
protected:
	FieldInfo * _head;
	vector<string>::iterator find (vector<string> & tmp, string name);
public:
	FieldFileInfo();
	virtual ~FieldFileInfo();

        virtual int ReadFromFile(string fname) = 0;
	void debug();
	void clear();

	FieldInfo * getInfo(int i);
	FieldInfo * getInfo( string fieldname, int component = 1 );
		
	int size() {
		FieldInfo * t = _head; 
		int i = 0;
		while (t!=0) {
			i++;
			t = t->next;
		}
		return i;
	}

	int count( string fieldname );
	int count() { return size(); };

	FieldInfo * addNew();

	vector<string> getFieldNames();
	vector<string> getFieldTimes( string fname );
};

class FieldReader {
public:
	FieldReader();
	virtual ~FieldReader() {};

	virtual int ReadFromFile(SimpleField * f, string fname, int fstartpos, FieldInfo * fieldInfo = 0) = 0;
	virtual int ReadFromFileName(SimpleField * f, string fname, FieldFileInfo *ffi, string fieldname, string stime = "") = 0;
	virtual int SaveToFile(SimpleField * f, string fname, FieldInfo * fieldInfo = 0) = 0;
};
#endif


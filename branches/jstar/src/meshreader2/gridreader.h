#ifndef _GRIDREADER_H
#define _GRIDREADER_H

#include <grid.h>
#include <string>

using namespace std;

typedef struct stGridInfo {
		char fname[255];
		int position;
		char name[255];
		int NoOfSpaceDim;
		int NoOfNodes;
		int NoOfElements;
		char gridType[50];
		int checkNum;
		int gridNum;
		
		struct stGridInfo * next;
}gridInfo;

class GridFileInfo {
	protected:
		gridInfo *_first;
		int _gridCount;
		
		void clear();
		gridInfo * add();
	public:
		GridFileInfo();
		virtual ~GridFileInfo();
		
		int gridCount();
		int count() { return gridCount(); }
		gridInfo * getInfo(int i);
		
		gridInfo *first();
		gridInfo *last();
		
		virtual void ReadFromFile(string fname); 
		
		virtual void debug();
};

class GridReader
{
	protected:
		string FReaderName;
		Grid *_g;

	public:
		GridReader();
		virtual ~GridReader() {};

		string ReaderName();

		virtual int ReadFromFile(Grid *g, string fname, int fstartpos = 0) = 0;
		virtual int SaveToFile(Grid *g, string fname, stGridInfo * gridInfo = 0) = 0;
				
};
#endif

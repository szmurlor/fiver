#ifndef _DIFFREADER_H
#define _DIFFREADER_H

#include "gridreader.h"
#include "fieldreader.h"
#include <elemt4n3d.h>
#include <elemt3n2d.h>
#include <grid.h>
#include <stdio.h>
#include <vector>

#ifdef Q_OS_UNIX
  #include <rpc/types.h>
  #include <rpc/xdr.h>
#endif

#include <errno.h>
#include <ctype.h>

using namespace std;

#define GRID_TEXT 1
#define GRID_BINARY 2

typedef struct _stIxInfo {
	int num;
	int position;
} stIxInfo;

class DpIxFile
{
	protected:
		vector< stIxInfo > items;

		void write_int(FILE * f, int v);
		int read_int(FILE * f);
	public:
		DpIxFile() { items.resize(0); };
		~DpIxFile() {};

		void clear();
		void append( int _num, int _position );

		void ReadFromFile( string fname );
		void SaveToFile( string fname );
		/** Num of the item is: count() + 1 */
		void AppendToFile(string fname, int _position);

		void debug();

		int count() { return items.size(); };
		int size() { return count(); };
};



class DpGridFileInfo : public GridFileInfo {
    public:
        DpGridFileInfo();
        ~DpGridFileInfo();
        
        virtual void ReadFromFile(string fname); 
};

class DpIxFileNameContainer {
	protected:
		DpIxFile dix;
		string _ixFileName;
		bool bClearIxFile;

		string ixFileName(string fname);
		string buildIxFileName(string fname);
		void SaveIxToFILE( FILE * fout, string fname);
	public:
		bool bSaveIxFile;

		DpIxFileNameContainer() { 
			_ixFileName[0] = 0; 
			bClearIxFile = 	false;
			bSaveIxFile = false;
		};
		~DpIxFileNameContainer() {};

		/** Copies memory of string to local area! */
		void setIxFileName( string fname );
		void clearIxName() { _ixFileName[0] = 0; };
								
};

class DpReader: public GridReader, public DpIxFileNameContainer
{
    protected:
        FILE *fin;

        int ReadGridHeader(char *line);
        void ReadBinary();
        void ReadText();
	void GetGridInfo( stGridInfo * gridInfo );

    public:
        int gridNum;
        int checkNum;
        string gridType;
        int dim;

        DpReader();

        virtual int ReadFromFile(Grid *g, string fname, int fstartpos = 0);
		virtual int SaveToFile(Grid *g, string fname, stGridInfo * gridInfo = 0);
		virtual int AppendToFile(Grid *g, string fname, stGridInfo * gridInfo = 0);

};

class DpFieldFileInfo : public FieldFileInfo
{
    public:
        DpFieldFileInfo();
        ~DpFieldFileInfo();

        virtual int ReadFromFile( string fname );
};

class DpFieldReader : public FieldReader, public DpIxFileNameContainer
{
protected:
	void ReadBinary( SimpleField * f, FILE * fin, int icount );
	void ReadText( SimpleField * f, FILE * fin, int icount );
	void GetFieldInfo( FieldInfo * fieldInfo );
    
  public:
    int fieldNum;
    string header;
    string description;
    int gridRefNum;
    string fieldType;
    int component, dim;
    string fieldTypeLong;

    DpFieldReader();
    ~DpFieldReader();
    
	virtual int ReadFromFile( SimpleField * f, string fname, int fstartpos, FieldInfo * fieldInfo = 0 );
    virtual int ReadFromFileName(SimpleField * f, string fname, FieldFileInfo *ffi, string fieldname, string stime = 0);
	virtual int SaveToFile( SimpleField * f, string fname, FieldInfo * fieldInfo = 0 );
	virtual int AppendToFile( SimpleField *g, string fname, FieldInfo * fieldInfo = 0 );

};

#endif

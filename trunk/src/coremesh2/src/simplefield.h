#ifndef _SIMPLEFIELD_H
#define _SIMPLEFIELD_H

#include <vector>
#include <iostream>
#include <string>
#include <map>

class Grid;

using namespace std;

/**
    val(i) - return i-th element of field.
    First element has index = 0!
    First dimension has index = 0!
*/
class SimpleField
{
protected:
	vector < vector < double > >_fvals;
	string _name;

public:
	map < string, string > attr;

        SimpleField ();
	~SimpleField ();
	void clear ();
	void copyFrom (SimpleField & newf);
	
	int dim ()
	{
		return _fvals.size ();
	};

	void dim (int i)
	{
		_fvals.resize (i);
	};

	int size ()
	{
		if (_fvals.size () > 0)
			return _fvals[0].size ();
		else
		return 0;
	};
	int size (int i)
	{
		return _fvals[i].size ();
	};

	void name (string fieldname);
	string name ();

	double val (int i);
	double val (int dim, int i);
	
	void val (int i, double v);
	void val (int dim, int i, double v);
	
	void set (int i, double val);
	void set (int dim, int i, double v);
	
	double operator[] (int i);
	
	void debug ();
	
	double min ();
	double max ();

        double min (Grid* grid, int subDomain);
        double max (Grid* grid, int subDomain);

	void setAttr( string name, string value );
	string getAttr( string name );
};

#endif

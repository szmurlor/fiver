#ifndef _FIELD_H
#define _FIELD_H

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <simplefield.h>

class Grid;

using namespace std;

/**
    val(i) - return i-th element of field.
    First element has index = 0!
    First dimension has index = 0!
*/
class Field
{
protected:
	vector < SimpleField * >     _fields;
	vector < double >            _timesteps;
	unsigned int                 _curtime;
	map < string, string >       _attr;
	string                       _name;


public:

	Field ();
	~Field ();

	void clear ();
	void copyFrom (Field & newf);
	
	int dim ()
	{
		if (_fields.size() > _curtime)
			return _fields[_curtime]->dim();
		else 
			return 0;
	};
	void dim (int i)
	{
		if (_fields.size() > _curtime)
			_fields[_curtime]->dim(i);
	};

	int size ()
	{
		if (_fields.size() > _curtime)
			return _fields[_curtime]->size ();
		else
			return 0;
	};
	int size (int i)
	{
		if (_fields.size() > _curtime)
			return _fields[_curtime]->size (i);
		else
			return 0;
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
        double min (Grid* grid, int subDomain);
	double max ();
        double max (Grid* grid, int subDomain);

	SimpleField * add( double timestep = 0);
	SimpleField * add(SimpleField * f, double timestep = 0);
	SimpleField * getSimpleField();

	double isStatic();
	double isDynamic();
	double getMinTime();
	double getMaxTime();
	double getTimeStep();
	double getCurTime();
	void setCurTime(double timestep);

	void setAttr( string name, string value );
	string getAttr( string name );
};

#endif

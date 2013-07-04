#include "field.h"
#include "grid.h"
#include <string>

Field::Field()
{
	_curtime = 0;
	dim(0);
}

Field::~Field()
{
	dim(0);
}

void Field::clear()
{
	for (unsigned int d = 0; d < _fields.size(); d++) {
		_fields[d]->clear();
		delete _fields[d];
	}
	_fields.resize(0);
	_timesteps.resize(0);
}

double Field::val (int i)
{
	if (_curtime < _fields.size())
		return _fields[_curtime]->val(0,i);
	else
		return 0;
}

double Field::val (int dim, int i)
{
	if (_curtime < _fields.size())
		return _fields[_curtime]->val(dim,i);
	else
		return 0;
}

void Field::val(int i, double v )
{
	if (_curtime < _fields.size())
		_fields[_curtime]->val(i,v);
}

void Field::set(int i, double v )
{
	if (_curtime < _fields.size())
		_fields[_curtime]->val(i,v);
}

void Field::set(int dim, int i, double v )
{
	if (_curtime < _fields.size())
		_fields[_curtime]->val(dim,i,v);
}

void Field::val(int dim, int i, double v )
{
	if (_curtime < _fields.size())
		_fields[_curtime]->val(dim,i,v);
}

void Field::name( string fieldname )
{
	_name = fieldname;
}

string Field::name()
{
	return _name;
}

double Field::operator[](int i)
{
	if (_curtime < _fields.size())
		return _fields[_curtime]->val(i);

	return 0;
}

void Field::debug()
{
	for (unsigned int i=0; i < _fields.size(); i++) {
		cout << "Dumping field ["<< _timesteps[i] << "]:" << endl;
		_fields[i]->debug();
	}
}

double Field::min()
{
	unsigned int i;
	double vmin = 1e78;
	for (i = 0;  i < _fields.size(); i++) {
		double t = _fields[i]->min();
		if (t < vmin)  { vmin = t;}
	}
	return vmin;
}

double Field::max()
{
	unsigned int i;
	double vmax = -1e78;
	for (i = 0;  i < _fields.size(); i++) {
		double t = _fields[i]->max();
		if (t>vmax) vmax = t;
	}
	return vmax;
}

double Field::min(Grid* grid, int subDomain)
{
    unsigned int i;
    double vmin = 1e78;
    std::cerr << "Field::min subDomain(" << subDomain << ")";
    for (i = 0;  i < _fields.size(); i++) {
        double t = _fields[i]->min(grid, subDomain);
        if (t < vmin)  { vmin = t;}
    }
    std::cerr << "=" << vmin << std::endl;
    return vmin;
}

double Field::max(Grid* grid, int subDomain)
{
    unsigned int i;
    double vmax = -1e78;
    for (i = 0;  i < _fields.size(); i++) {
        double t = _fields[i]->max(grid, subDomain);
        if (t>vmax) vmax = t;
    }
    //std::cerr << "Field::max subDomain(" << subDomain << ")=" << vmax << std::endl;
    return vmax;
}

void Field::copyFrom( Field &  )
{
	printf("CopyFrom in Field NOT implemented!");
}

SimpleField * Field::add(double timestep)
{
        if( _fields.size() == 0 )
            throw string( "Field::add(double timestep) - can not add field without grid");
        SimpleField *f = new SimpleField();
	_fields.push_back( f );
	_timesteps.push_back( timestep );
	return f;
}

SimpleField * Field::add(SimpleField * f, double timestep)
{

        _fields.push_back(f);
	_timesteps.push_back( timestep );        

	return f;
}

SimpleField * Field::getSimpleField()
{
	if (_curtime < _fields.size()) 
		return _fields[_curtime];
	else
		return 0;
}

double Field::isStatic()
{ 
	return (_fields.size() == 1); 
};

double Field::isDynamic()
{
	return (_fields.size() > 1);
}

double Field::getMinTime() {
	if (_timesteps.size() > 0)
		return _timesteps[0];
	else
		return 0;
}

double Field::getMaxTime() {
	if (_timesteps.size() > 0) {                
		return (_timesteps.back());
	} else {
		return 0;
	}
}

double Field::getCurTime() 
{
	return _timesteps[ _curtime ];
}

double Field::getTimeStep() 
{
	if (_timesteps.size() > 1) {
		return (_timesteps[1] - _timesteps[0]);
	} else {
		return 0;
	}
}

void Field::setCurTime(double timestep) 
{
	for (unsigned int i = 0; i < _timesteps.size(); i++) {
		if (timestep == _timesteps[i]) {
			_curtime = i;
			return;
		}
	}
}

void Field::setAttr( string name, string value )
{
	_attr[name] = value;
}

string Field::getAttr( string name )
{
	return _attr[name];
}


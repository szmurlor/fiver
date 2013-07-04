#include "simplefield.h"
#include "grid.h"
#include <set>

SimpleField::SimpleField()
{
    dim(0);
}

SimpleField::~SimpleField()
{
    dim(0);
}

void SimpleField::clear()
{
	for (unsigned int d = 0; d < _fvals.size(); d++)
		_fvals[d].resize(0);
	
	_fvals.resize(0);
}

double SimpleField::val (int i)
{
    return val(0,i);
}

double SimpleField::val (int dim, int i)
{
    if (_fvals.size() > 0)
        return ( (_fvals[ dim ])[ i ]);
    else
        return 0;
}

void SimpleField::val(int i, double v )
{
    val(0,i,v);
}

void SimpleField::set(int i, double v )
{
    val(0,i,v);
}

void SimpleField::set(int dim, int i, double v )
{
    val(dim, i, v);
}

void SimpleField::val(int dim, int i, double v )
{
	if (_fvals.size() > (unsigned int) dim) {
		if (_fvals[dim].size() <= (unsigned int) i) {
			_fvals[dim].resize( i + 1 );
				}
		(_fvals[dim])[ i ] = v; 
	} else {
		cerr << "Warning! Setting field value for dim = " << dim << " while fields.size() = "<< _fvals.size() << endl;
	}
}

void SimpleField::name( string fieldname )
{
	_name = fieldname;
}

string SimpleField::name()
{
	return _name;
}

double SimpleField::operator[](int i)
{
    return val(i);
}

void SimpleField::debug()
{
  cout << "Dumping field..."<<endl;
  for (unsigned int i = 0; i < _fvals.size(); i++) {
    for (unsigned int j = 0; j < _fvals[i].size(); j++)
      cout << "[" << i << "," << j << "] = " << (_fvals[i])[j] << endl;
  }
  cout << "Dumping field finished."<<endl;
}

/* oryginalna wersja Roberta
double SimpleField::min()
{
    unsigned int i,j;
    double vmin = 1e78;
    for (i = 0;  i < _fvals.size(); i++) { // petla po skladowych
        for (j = 0; j < _fvals[i].size(); j++) { //petla po elementach
            if ((_fvals[i])[j] < vmin)
                vmin = (_fvals[i])[j];
        }
    }
    return vmin;
}

double SimpleField::max()
{
    unsigned int i,j;
    double vmax = -1e78;
    for (i = 0;  i < _fvals.size(); i++) {
        for (j = 0; j < _fvals[i].size(); j++) {
            if ((_fvals[i])[j] > vmax)
                vmax = (_fvals[i])[j];
        }
    }
    return vmax;
}
*/

double SimpleField::min()
{
    unsigned int i,j;
    double vmin = 1e78;
    if( _fvals.size() == 1 ) { // scalar
        for (j = 0; j < _fvals[0].size(); j++) { //petla po elementach
            if ((_fvals[0])[j] < vmin)
                vmin = (_fvals[0])[j];
        }
    } else { // vector - take magnitude
        for (j = 0; j < _fvals[0].size(); j++) { //petla po elementach
            double m= 0;
            for (i = 0;  i < _fvals.size(); i++) { // petla po skladowych
                m+= (_fvals[i])[j]*(_fvals[i])[j];
            }
            m= sqrt( m );
            if (m < vmin)
                vmin = m;
        }
    }
    return vmin;
}

double SimpleField::max()
{
    unsigned int i,j;
    double vmax = -1e78;
    if( _fvals.size() == 1 ) { // scalar
        for (j = 0; j < _fvals[0].size(); j++) { //petla po elementach
            if ((_fvals[0])[j] > vmax )
                vmax = (_fvals[0])[j];
        }
    } else { // vector - take magnitude
        for (j = 0; j < _fvals[0].size(); j++) { //petla po elementach
            double m= 0;
            for (i = 0;  i < _fvals.size(); i++) { // petla po skladowych
                m+= (_fvals[i])[j]*(_fvals[i])[j];
            }
            m= sqrt( m );
            if (m > vmax)
                vmax = m;
        }
    }
    return vmax;
}

double SimpleField::min(Grid* grid, int subDomain)
{
    unsigned int i,j;
    double vmin = 1e78;
    std::set<int> sj;

    bool fieldAtNodes= false;
    if( _fvals[0].size() == grid->getNoNodes() )
        fieldAtNodes= true;

    if( _fvals.size() == 1 ) { // scalar
        for (j = 0; j < _fvals[0].size(); j++) { //petla po wartosciach
            if( fieldAtNodes ) {
                grid->nodeSubdomains(j,sj);
                if( sj.size() == 0 || sj.find(subDomain) == sj.end() ) {
                    continue;
                }
            } else if( grid->elemSubdomain(j) != subDomain )
                continue;

            if ((_fvals[0])[j] < vmin)
                vmin = (_fvals[0])[j];
        }
    } else { // vector - take magnitude
        for (j = 0; j < _fvals[0].size(); j++) { //petla po wartosciach
            if( fieldAtNodes ) {
                grid->nodeSubdomains(j,sj);
                if( sj.size() == 0 || sj.find(subDomain) == sj.end() ) {
                    continue;
                }
            } else if( grid->elemSubdomain(j) != subDomain )
                continue;

            double m= 0;
            for (i = 0;  i < _fvals.size(); i++) { // petla po skladowych
                m+= (_fvals[i])[j]*(_fvals[i])[j];
            }
            m= sqrt( m );
            if (m < vmin)
                vmin = m;
        }
    }
    //std::cerr << "SimpleField::min subDomain(" << subDomain  << ")";
    //std::cerr << "=" << vmin << std::endl;
    return vmin;
}

double SimpleField::max(Grid *grid, int subDomain)
{
    unsigned int i,j;
    double vmax = -1e78;
    std::set<int> sj;

    bool fieldAtNodes= false;
    if( _fvals[0].size() == grid->getNoNodes() )
        fieldAtNodes= true;

    if( _fvals.size() == 1 ) { // scalar
        for (j = 0; j < _fvals[0].size(); j++) { //petla po wartosciach
            if( fieldAtNodes ) {
                grid->nodeSubdomains(j,sj);
                if( sj.size() == 0 || sj.find(subDomain) == sj.end() ) {
                    continue;
                }
            } else if( grid->elemSubdomain(j) != subDomain )
                continue;

            if ((_fvals[0])[j] > vmax )
                vmax = (_fvals[0])[j];
        }
    } else { // vector - take magnitude
        for (j = 0; j < _fvals[0].size(); j++) { //petla po wartosciach
            if( fieldAtNodes ) {
                grid->nodeSubdomains(j,sj);
                if( sj.size() == 0 || sj.find(subDomain) == sj.end() ) {
                    continue;
                }
            } else if( grid->elemSubdomain(j) != subDomain )
                continue;

            double m= 0;
            for (i = 0;  i < _fvals.size(); i++) { // petla po skladowych
                m+= (_fvals[i])[j]*(_fvals[i])[j];
            }
            m= sqrt( m );
            if (m > vmax)
                vmax = m;
        }
    }
    //std::cerr << "SimpleField::max subDomain(" << subDomain  << ")=" << vmax << std::endl;
    return vmax;
}

void SimpleField::copyFrom( SimpleField & newf )
{
	clear();
	_fvals = newf._fvals;
	_name = newf._name;
	attr = newf.attr;
}

void SimpleField::setAttr( string name, string value )
{
	attr[name] = value;
}

string SimpleField::getAttr( string name )
{
	return attr[name];
}


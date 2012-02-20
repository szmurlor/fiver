/* Author: Robert Szmurlo
 * created: 2005.04... 
 */ 


#include <simplefield.h>
#include <grid.h>
#include <iostream>
#include <vector>

#define ENERGY 1
#define POWER 2
#define FLUXDENSITY 1
#define FIELDINTENSITY 2

using namespace std;

class CFieldEnergy {

protected:
	double max_value;
	vector<SimpleField*> fields;
	Grid *grid;

	double equation( double mat_val, double v );
	
public:
	int calcType;
	int fieldType;
	bool DEBUG;
	double fNormVal;
	int iNormSub;
	bool bNorm;
	double dOver;
	bool bEX;
 	vector<int> sub_idx;
	vector <double> mat;
	bool bOver;

	CFieldEnergy();
	~CFieldEnergy();

	double calcMax(int isub);
	double val(int ie);

	void calculateScalar(Grid *g, SimpleField *f);

	void calculate3D( Grid *g, SimpleField *f1, SimpleField *f2, SimpleField *f3 );
};


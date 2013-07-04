#include <grid.h>
#include <field.h>
#include <elem.h>
#include <diffreader.h>
#include <diffsimu.h>
#include <stdio.h>
#include <vector>
#include <geom.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void printHelp()
{
	printf("usage: fieldinfo [-g grid_name] [-gn grid_nr] [-P px py pz] [-L L1x L1y L1z L2x L2y L2z ndL] \n");
	printf("    -l               : list fields in file\n");
	printf("    -gn grid_name    : name of file with grid definition.\n");
	printf("    -g grid_nr       : index of grid inside the file to be read (first index = 1)\n");
	printf("    -P px py pz      : find element and field value in point px, py, pz\n");
	printf("    -L L1x.... ndL   : find elements and field values in ndL+1 points located on the line\n");
	printf("                       between points L1 and L2\n");
	printf("    -s px py pz r np : calculate average value unside sphere with center px,py,pz, and radius r,\n");
	printf("                       and number of points on radius np (satisfactory results are for np=3)\n");
	printf("    -f fldfname fc f1 f2 ..   : read fields from specified indexes in file. fc - number of indexes\n");
	printf("    -mmax iSub N   : show N top most maximum field values in subdomain iSub. \n");
	

}

bool calc_val( Field * f, Grid * g, Elem * e, double *v, double P[3])
{
	vector<double> vals(4);
	if ( g->_nodes.size() == f->size() ) {
		for (int j=0; j<4; j++) {
			vals[j] = f->val(e->_nodes[j] - 1);
		}
		*v = e->calcValPtv( P, vals);
		return true;
	} else {
		if (g->_elems.size() == f->size() ) {
			*v = f->val( e->nr - 1);
			return true;
		} else {
			printf("Error: Field size does not match node nor element vectors size.\n");
			return false;
		}
	}
	return false;
}

void print_val(Grid *g, vector<Field*> fields,double P[3])
{
	Elem *e;
	e = g->getElemPtv( P );
	//printf("vals = %g,%g,%g,%g\n",vals[0], vals[1], vals[2], vals[3]);
	double v;

	if (e == 0) 
		printf("Unable to locate element for point P (%g,%g,%g)\n", P[0], P[1], P[2]);
	else {
		//printf("Point P (%g,%g,%g) is located in elem: %d\n", P[0], P[1], P[2],e->nr);		
		printf("(");
		for (int fi = 0; fi < fields.size(); fi++){
			if (calc_val(fields[fi],g,e, &v, P)) {
				if (fi>0) printf(", ");
				printf("%g", v );
			}
		}
		printf(")\n");
	}
}

double get_val(Grid *g, Field* f,double P[3], double *v)
{
	Elem *e;
	e = g->getElemPtv( P );

	if (e == 0) 
		printf("Unable to locate element for point P (%g,%g,%g)\n", P[0], P[1], P[2]);
	else {
		return calc_val(f,g,e,v,P);
	}
	return false;
}

void csphere( Grid * g, vector<Field*> f, double p[3], double r, int np)
{
	double sp;
	int ix,iy,iz;
	double pp[3];
	double v;
	double v_mod; 
	double v_mod_sum = 0;
	
	vector<int> ic( f.size() );
	for (int i=0; i<ic.size(); i++)
		ic[i] = 0;
	
	vector<double> sum( f.size() );
	for (int i=0; i<sum.size(); i++)
		sum[i] = 0;
	
	sp = 1 / (double) (2*np-2);
	
	
	for (ix=0;ix<(np*2 -1);ix++){
		for (iy=0;iy<(np*2 -1);iy++){
			for (iz=0;iz<(np*2 -1);iz++) {
				pp[0] = -1 + (double) ix * sp;
				pp[1] = -1 + (double) iy * sp;
				pp[2] = -1 + (double) iz * sp;
				
				//printf("pp = (%lf, %lf, %lf)\n",pp[0],pp[1],pp[2]);
				
				if (LENGTH(pp) <= 1) {
					MULT(pp,pp,r);
					ADD(pp,p,pp);
 					//print_val(g,f,pp);
					
					v_mod = 0;
					for (int fi=0;fi<f.size();fi++) {
						if (get_val(g,f[fi],pp, &v) == true) {
							sum[fi] += v;
							v_mod += v*v;
							ic[fi]++;
						}
					}	
					v_mod_sum += sqrt(v_mod);
				}
			}
		}
	}

	printf("Average value in sphere (collected from %d points): ", ic[0]);	
	for (int i=0; i<sum.size(); i++)
		if (ic[i] != 0) {
			sum[i] /= ic[i];
			if (i>0) printf(", ");
			printf("%lf",sum[i]);
		}
	if (ic[0] != 0) {
		printf(": |f|=%lf\n",v_mod_sum/ic[0]);
	}
	printf("\n");
	
}

struct TValElem
{
    double val;
    int ie;
    int is;
};

int TVE_cmp( const void * v1, const void * v2 )
{
        TValElem *ve1, *ve2;
        ve1 = (TValElem*) v1;
        ve2 = (TValElem*) v2;

        if (ve1->val < ve2->val) return 1;
        else if (ve1->val > ve2->val) return -1;
        else return 0;
}

void showmax( Grid * g, vector<Field*> f, int iSub, int n )
{
    int i,j;
    TValElem * vv;
    double v;
    
    vv = new TValElem[ f[0]->size() ];
    printf("Allocated... %d\n", f[0]->size() );
    for (i=0; i< f[0]->size(); i++) {
        v = 0;
        for (j=0;j<f.size();j++)
            v += f[j]->val(i)*f[j]->val(i);

        vv[i].val = sqrt(v);
        vv[i].ie = i+1;
    }

    qsort( vv, f[0]->size(), sizeof( TValElem ), TVE_cmp );

    j = 0;
    for (i=0; i<f[0]->size(); i++) {
        if (j == n) break;

        if ((iSub == 0) || ((g->_elems[ vv[i].ie ])->subdomain() == iSub)) {
            j++;
            printf("Val[%d] = %g, Elem = %d\n", j, vv[i].val, vv[i].ie);
        }
    }

    delete vv;

}

int main(int argc, char *argv[])
{
	int i;
	DpFieldFileInfo dffi;
	DpFieldReader dfr;
	vector<Field*> fields;
	char *fname;
	char *new_fname;
	bool bPoint = false;
	bool bLine = false;
	double P[3];
	double L1[3], L2[3];
	int ndL;
	int ignr = 1;
	char *gfname = 0;
	DpGridFileInfo dgfi;
    DpReader dr;
	Grid *g;
	bool bSphere = false;
	double P_sph[3], r_sph;
	int np_sph;
	bool bList = false;
	char * ffname;
	bool bMax = false;
	int iMaxSub = 0;
	int iMaxN = 0;

	printf("before\n");
	DiffSimu smgr( argc, argv );
	printf("after\n");

	if (argc < 2) {
		printHelp();
		exit(-1);
	}
	
	i = 1;
	while (i<argc) {
		
		if (strcmp("-l",argv[i]) == 0) {
			bList = true;
			ffname = argv[++i];
		}	
		

		if (strcmp("-P", argv[i]) == 0) {
			bPoint = true;
			P[0] = atof( argv[++i] );
			P[1] = atof( argv[++i] );
			P[2] = atof( argv[++i] );
		}

		if (strcmp("-L",argv[i]) == 0) {
			bLine = true;
			L1[0] = atof( argv[++i] );
			L1[1] = atof( argv[++i] );
			L1[2] = atof( argv[++i] );
			L2[0] = atof( argv[++i] );
			L2[1] = atof( argv[++i] );
			L2[2] = atof( argv[++i] );
			ndL = atoi( argv[++i] );
		}

		if (strcmp("-s", argv[i]) == 0) {
			bSphere = true;
			P_sph[0] = atof( argv[++i] );
			P_sph[1] = atof( argv[++i] );
			P_sph[2] = atof( argv[++i] );
			r_sph =  atof( argv[++i] );
			np_sph = atoi( argv[++i] );
		}

		if (strcmp("-mmax", argv[i]) == 0) {
						bMax = true;
						iMaxSub = atoi( argv[++i] );
						iMaxN = atoi( argv[++i] );
		}

		i++;
	}
	
	if (bList) {
		printf("before\n");
		dffi.ReadFromFile( ffname );
		printf("after\n");
		dffi.debug();
	}
	
	if (bPoint) {
		Elem * e;
		g = smgr.getGrid();
		
		vector<Field *> fields;		
		fields = smgr.getFields();
		
		print_val(g,fields,P);
		
	}

	if (bLine) {
		Elem * e;
		g= smgr.getGrid();
		g->prepareElemPtv(9);
		
		vector<Field *> fields;
		fields = smgr.getFields();
		
		for (i=0; i <= ndL; i++) {
			P[0] = L1[0] + (L2[0] - L1[0]) * (i/(double)ndL);
			P[1] = L1[1] + (L2[1] - L1[1]) * (i/(double)ndL);
			P[2] = L1[2] + (L2[2] - L1[2]) * (i/(double)ndL);
			
			print_val(g,fields,P);
			
		}
	}
	
	if (bSphere) {
		smgr.getGrid()->prepareElemPtv(9);
		csphere(smgr.getGrid(), smgr.getFields(), P_sph, r_sph, np_sph );
	}

	if (bMax) {
		showmax( smgr.getGrid(), smgr.getFields(), iMaxSub, iMaxN );
	}

	return(0);
}

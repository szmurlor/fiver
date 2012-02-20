/* Author: Robert Szmurlo
 * created: 2005.01... 
 */ 


#include "fieldenergy.h"

CFieldEnergy::CFieldEnergy()
{
	fNormVal = 0;
	iNormSub = -1;
	bNorm = false;
	dOver = 0;
	calcType = POWER;
	fieldType = FLUXDENSITY;
	DEBUG = false;
	bEX = false;
}

CFieldEnergy::~CFieldEnergy()
{
}

double CFieldEnergy::calcMax(int isub)
{
	double tm = -1e80;
	double tmp_value;
	int i,j;

	for (i = 0; i < grid->_elems.size(); i++) {
		if ((isub <= 0) || (isub == grid->_elems[i]->subdomain())) {
			tmp_value = 0;
			for (j=0;j<fields.size();j++)
				tmp_value += fields[j]->val(i)*fields[j]->val(i);

			if (tmp_value > tm) 
				tm = tmp_value;
		}
	}
	if (DEBUG) printf("Maximum value in subdomain (%d): %g\n",isub,tm);

	return (tm);
}

void CFieldEnergy::calculateScalar(Grid *g, SimpleField *f)
{
	vector<double> results;
	double total_energy = 0;
	int i;
	double v;
	int isub;
	double mat_val;
	double el_energy;
	double max_value = 0;
	double tmp_value;
	

	if (iNormSub >= 0) {
		
		if (DEBUG) printf("Maximum value for normalization: %g\n",max_value);
	}
	
	if (fNormVal != 0)
		max_value = fNormVal * fNormVal;

	if (DEBUG) {
		for (i = 0; i < mat.size(); i++) {
			printf("Material %d = %f\n", i+1, mat[i] );
		}
	}

	results.resize( mat.size() );
	for (i=0; i < results.size(); i++)
		results[i] = 0;

	for ( i = 0; i < g->_elems.size(); i++ ) {
		v = f->val(i) * f->val(i);
		if (max_value != 0)
			v /= max_value;

		isub = g->indexOfSubdomain( g->_elems[i]->subdomain() );
		mat_val = mat[isub];

		if (calcType == ENERGY)
				v *= 0.5;
		if ((fieldType == FLUXDENSITY) && (mat_val != 0))
				v /= mat_val;
		if (fieldType == FIELDINTENSITY)
				v *= mat_val;

	  el_energy = g->_elems[i]->volume();
	
		total_energy += el_energy;
		results[isub] = results[isub] + el_energy;
	}

	if (sub_idx.size() == 0) {
		for (i=0; i < results.size(); i++)
			if (calcType == ENERGY)
				printf("Energy in subdomain %d = %g\n",g->_subdomains[i].nr, results[i]);
			else
				printf("Power in subdomain %d = %g\n",g->_subdomains[i].nr, results[i]);
	} else {
		for (i=0; i < sub_idx.size(); i++) {
			isub = g->indexOfSubdomain( sub_idx[i] );
			if (calcType == ENERGY)
				printf("Energy in subdomain %d = %g\n",g->_subdomains[isub].nr, results[isub]);
			else
				printf("Power in subdomain %d = %g\n",g->_subdomains[isub].nr, results[isub]);
		}
	}

	if (calcType == ENERGY)
		printf("Total energy = %g (in whole model)\n", total_energy );
	else
		printf("Total power = %g (in whole model)\n", total_energy );


}

double CFieldEnergy::val(int ie)
{
	int i;
	double v;
	double mat_val;
	int isub;

	isub = grid->indexOfSubdomain( grid->_elems[ie]->subdomain() );
		
	mat_val = mat[isub];

	v = 0;
	for (i=0;i<fields.size();i++)
		v += fields[i]->val(ie)*fields[i]->val(ie); 

	return( equation( mat_val, v ) );
}

double CFieldEnergy::equation( double mat_val, double v )
{
	if (bNorm) {
		if (fNormVal != 0)
			v /= fNormVal;
		else
			printf("Warning! fNormVal = 0!\n");
	}

	if (calcType == ENERGY)
			v *= 0.5;
	if ((fieldType == FLUXDENSITY) && (mat_val != 0))
			v /= mat_val;
	if (fieldType == FIELDINTENSITY)
			v *= mat_val;

	return (v);
}

void CFieldEnergy::calculate3D(Grid *g, SimpleField *f1, SimpleField *f2, SimpleField *f3 )
{
	vector<double> results;
	vector<double> vols;
	vector<double> vols_over;
	vector<int> ielem;
	double vol;
	double total_energy = 0;
	int i;
	double v;
	int isub;
	double mat_val;
	double el_energy;
	double tmp_value;
	double tvol;
	double volOver = 0;
	double maxV;
	double max_value_mat;

	fields.resize(3);
	fields[0] = f1;
	fields[1] = f2;
	fields[2] = f3;
	grid = g;

	if (DEBUG) printf("Starting Calculate3D\n");
	if (iNormSub >= 0) {
		fNormVal = calcMax( iNormSub );
		if (DEBUG) printf("Maximum value for normalization (Vx^2+Vy^2+Vz^2) = : %g\n",fNormVal);
	}
	
	//printf("fNormVal = %g\n",fNormVal);
	max_value = fNormVal;


	if (DEBUG) {
		printf("Materials:\n");
		for (i = 0; i < mat.size(); i++) 
			printf("Material %d = %f\n", i+1, mat[i] );
	}

	if (DEBUG) printf("Initiliazing variables...\n");
	vols_over.resize( mat.size() );
	vols.resize( mat.size() );
	results.resize( mat.size() );
	ielem.resize( mat.size() );
	for (i=0; i < results.size(); i++) {
		results[i] = 0;
		vols[i] = 0;
		ielem[i] = 0;
		vols_over[i] = 0;
	}
	vol = 0;

	if (DEBUG) printf("Calculating the intergral...\n");
	for ( i = 0; i < g->_elems.size(); i++ ) {
		isub = g->indexOfSubdomain( g->_elems[i]->subdomain() );
		
		v = val(i);

		tvol = g->_elems[i]->volume();
		vols[isub] = vols[isub] + tvol;
		vol += tvol;
		el_energy = v * tvol;
		total_energy += el_energy;
		results[isub] = results[isub] + el_energy;

		mat_val = mat[isub];
		max_value_mat = equation(mat_val,max_value);

		if ( v > (dOver * max_value_mat) ) {
			ielem[isub]= ielem[isub]+1;
			if (isub == 2) {
				//printf("%d, v = %g\n",i,v);
				
			}
			vols_over[isub] = vols_over[isub] + tvol;
		}

	}

	if (DEBUG) printf("Printing results:\n");
	if (sub_idx.size() == 0) {
		for (i=0; i < results.size(); i++)
			if (calcType == ENERGY)
				printf("Energy in subdomain %d = %g\n",g->_subdomains[i].nr, results[i]);
			else
				printf("Power in subdomain %d = %g\n",g->_subdomains[i].nr, results[i]);
	} else {
		for (i=0; i < sub_idx.size(); i++) {
			isub = g->indexOfSubdomain( sub_idx[i] );
			if (calcType == ENERGY)
				printf("Energy in subdomain %d = %g\n",g->_subdomains[isub].nr, results[isub]);
			else
				printf("Power in subdomain %d = %g\n",g->_subdomains[isub].nr, results[isub]);
		}
	}

	if (calcType == ENERGY)
		printf("Total energy = %g (in whole model)\n", total_energy );
	else
		printf("Total power = %g (in whole model)\n", total_energy );

	if (bOver) {
			if (sub_idx.size() == 0) {
				for (i=0; i < results.size(); i++)
					if (calcType == ENERGY)
						printf("Vol where energy over %g, in subdomain %d = %g\n",(dOver * max_value), g->_subdomains[i].nr, vols_over[i]);
					else
						printf("Vol where power over %g, in subdomain %d = %g\n",(dOver * max_value), g->_subdomains[i].nr, vols_over[i]);
			} else {
				for (i=0; i < sub_idx.size(); i++) {
					isub = g->indexOfSubdomain( sub_idx[i] );
					if (calcType == ENERGY)
						printf("Vol where energy over %g, in subdomain %d = %g\n",(dOver * max_value), g->_subdomains[isub].nr, vols_over[isub]);
					else {
						printf("dOver = %g, max_value = %g\n",dOver, max_value);
						printf("Elems no = %d\n",ielem[isub]);
						printf("Vol where power over %g, in subdomain %d = %g\n",(dOver * max_value), g->_subdomains[isub].nr, vols_over[isub]);
					}
				}
			}
	}

	// calculate standard deviation
	if (bEX) {
		if (sub_idx.size() == 0) {
		// calculating stanadard deviation in total model
			printf("Standard deviation in Total Model not implemented.\n");
			double evol = vol;
			double p_avg = total_energy / evol;
			double sum = 0;
			
			if (DEBUG) printf("Total volume = %g\n", evol);
			if (DEBUG) printf("Average for EX calculation (average density of energy (or power)) = %g\n", p_avg);

			for (int j = 0; j < g->_elems.size(); j++) {
				v = val(j);

				tvol = g->_elems[i]->volume();
				sum += tvol * (v-p_avg)*(v-p_avg);
			}
			sum = sqrt( sum / evol );
			printf("Standard deviation (EX) in total model  = %g\n", sub_idx[i], sum);
		} else {
		// calculating standard deviation in selected subdomains
			for (i=0; i < sub_idx.size(); i++) {
				isub = g->indexOfSubdomain( sub_idx[i] );
				double evol = vols[ isub ];
				double p_avg = results[ isub ] / evol;
				double sum = 0;
				
				if (DEBUG) printf("Total volume of subdomain %d = %g\n", sub_idx[i], evol);
				if (DEBUG) printf("Average for EX calculation = %g\n", p_avg);

				for (int j = 0; j < g->_elems.size(); j++) {
					if (g->_elems[j]->subdomain() == sub_idx[i]) {
						v = val(j);

						tvol = g->_elems[i]->volume();
						sum += tvol * (v-p_avg)*(v-p_avg);
					}
				}
				sum = sqrt( sum / evol );
				printf("Standard deviation (EX) in subdomain %d = %g\n", sub_idx[i], sum);
				printf("Variance indicator (relative) (VEX) in subdomain %d = %g\n", sub_idx[i], sum / p_avg);
			}
		}

	}
}


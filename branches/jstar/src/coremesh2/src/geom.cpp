#include "geom.h"

#define SMALL_NUMBER 1e-20

double 
normalny4p(double * w1, double * w2, double * w3, double * w4,
           double wynik[3])
{
    double a1, a2, a3, b1, b2, b3, nomin, t;

    a1 = (w1)[0] - (w2)[0];
    a2 = (w1)[1] - (w2)[1];
    a3 = (w1)[2] - (w2)[2];

    b1 = (w1)[0] - (w3)[0];
    b2 = (w1)[1] - (w3)[1];
    b3 = (w1)[2] - (w3)[2];

    double A, B, C, D;
    A = a2 * b3 - a3 * b2;
    B = a3 * b1 - a1 * b3;
    C = a1 * b2 - a2 * b1;
    // from belonging of Node 1 to surface
    D = -(A * (w1)[0] + B * (w1)[1] + C * (w1)[2]);
    nomin = A*A + B*B + C*C;
    // sprawdzam wezly po kolei:
    t = -(A * (w4)[0] + B * (w4)[1] + C * (w4)[2] + D) / nomin;

    if (t >= 0) {
        wynik[0] = A/sqrt(nomin);
        wynik[1] = B/sqrt(nomin);
        wynik[2] = C/sqrt(nomin);
    } else {
        wynik[0] = -A/sqrt(nomin);
        wynik[1] = -B/sqrt(nomin);
        wynik[2] = -C/sqrt(nomin);
    }
	return(t);
}


void NORM(double n[3], double n1[3], double n2[3], double n3[3])
{
	double D1[3], D2[3];

	SUB(D1,n2,n1);
	SUB(D2,n3,n1);

	CROSS(n,D1,D2);
}


double DIST_Point_Plane(double P[3], double n1[3], double n2[3], double n3[3])
{
	double E1[3],E2[3];
	double N1[3];
	double du0;
	double d1;
  /* compute plane equation of triangle(V0,V1,V2) */
  SUB(E1,n2,n1);
  SUB(E2,n3,n1);
  CROSS(N1,E1,E2);
  d1=-DOT(N1,n1);
  /* plane equation 1: N1.X+d1=0 */

  /* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
  du0=DOT(N1,P)+d1;
 
	return du0;
}

double DIST_Point_Plane2(double P[3], double PS[3], double N[3])
{
	double N1[3],n[3];
	double d1,du0;
	SET(n,N);
	VERSOR(N1,n);
//	printf("VERSOR N1 = "); printf_vec(N1);
	
  d1=-DOT(N1,PS);
//	printf("d1 = %f\n",d1);
  /* plane equation 1: N1.X+d1=0 */

  /* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
  du0=DOT(N1,P)+d1;
 
	return du0;
	
}

double DIST_Point_Point(double P1[3], double P2[3] )
{
	double E1[3];

	SUB(E1,P1,P2);

	return( LENGTH(E1) );
}



double VOLUME_tetra(double n0[3], double n1[3], double n2[3], double n3[3])
{

	return ( 1./6.*(
				DET33(n1,n2,n3) 
			- DET33(n0,n2,n3)
			+ DET33(n0,n1,n3)
			- DET33(n0,n1,n2)) 
	);
	
}

double DIST_Point_Line(double P1[3], double N[3], double PL[3] )
{
	double P[3], t[3];
	SET(P,P1);
//	printf("P="); printf_vec(P);
	SUB(P,P1,PL);
//	printf("P="); printf_vec(P);
	CROSS(t,P,N);
//	printf("CROSS(t = "); printf_vec(t);
//	printf("LENGTH(P) = %f\n",LENGTH(P));
	if (LENGTH(N)==0)
		return 0;
	else
		return(LENGTH(t) / LENGTH(N));
}

// Zwraca punkty dest1 i dest2 bedace koncami najkrotszego odcinka
// pomiedzy prostymi P1->P2 i P3->P4.
// Punkt dest1 lezy na prostej P1->P2, a dest2 na P3->P4
// wzory tutaj: http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline3d/
void Shortest_Segment_Between_Lines(double P1[3], double P2[3], double P3[3], double P4[3], double dest1[3], double dest2[3])
{
    double d1343, d4321, d1321, d4343, d2121;
    double mua, mub;
    d1343 = (P1[0] - P3[0])*(P4[0]-P3[0])+
            (P1[1] - P3[1])*(P4[1]-P3[1])+
            (P1[2] - P3[2])*(P4[2]-P3[2]);

    d4321 = (P4[0] - P3[0])*(P2[0]-P1[0])+
            (P4[1] - P3[1])*(P2[1]-P1[1])+
            (P4[2] - P3[2])*(P2[2]-P1[2]);

    d1321 = (P1[0] - P3[0])*(P2[0]-P1[0])+
            (P1[1] - P3[1])*(P2[1]-P1[1])+
            (P1[2] - P3[2])*(P2[2]-P1[2]);

    d4343 = (P4[0] - P3[0])*(P4[0]-P3[0])+
            (P4[1] - P3[1])*(P4[1]-P3[1])+
            (P4[2] - P3[2])*(P4[2]-P3[2]);

    d2121 = (P2[0] - P1[0])*(P2[0]-P1[0])+
            (P2[1] - P1[1])*(P2[1]-P1[1])+
            (P2[2] - P1[2])*(P2[2]-P1[2]);

    mua = (d1343*d4321-d1321*d4343)/(d2121*d4343-d4321*d4321);
    mub = (d1343 + mua*d4321)/d4343;

    dest1[0] = P1[0] + mua*(P2[0] - P1[0]);
    dest1[1] = P1[1] + mua*(P2[1] - P1[1]);
    dest1[2] = P1[2] + mua*(P2[2] - P1[2]);

    dest2[0] = P3[0] + mub*(P4[0] - P3[0]);
    dest2[1] = P3[1] + mub*(P4[1] - P3[1]);
    dest2[2] = P3[2] + mub*(P4[2] - P3[2]);
}

double DIST_Point_Line2(double P1[3], double N[3], double PL[3], double Pout[3] )
{
    double P[3], t[3], dp;
	double verN[3];
	SET(P,P1);
	//  printf("P="); printf_vec(P);
	SUB(P,P1,PL);
	//  printf("P="); printf_vec(P);
	CROSS(t,P,N);
	if (LENGTH(N)==0)
		return 0;
	else {
		dp = DOT(P,N) / LENGTH(N);
		VERSOR(verN,N);
		Pout[0] = PL[0] + verN[0] * dp;
		Pout[1] = PL[1] + verN[1] * dp;
		Pout[2] = PL[2] + verN[2] * dp;
		return(LENGTH(t) / LENGTH(N));
	}
	//  printf("CROSS(t = "); printf_vec(t);
	//  printf("LENGTH(P) = %f\n",LENGTH(P));
}

double ISEC_Plane_Plane(double p1[3], double n1[3], double p2[3], double n2[3],
		       double p3[3], double pp3[3])
{
	double invdet;
	double pp3p[3];
	double d1,d2;
	
	CROSS(pp3,n1,n2);
	pp3p[0] = SQR(pp3[0]);
	pp3p[1] = SQR(pp3[1]);
	pp3p[2] = SQR(pp3[2]);
	
	d1 = -DOT(p1,n1);
	d2 = -DOT(p2,n2);
	//printf("d1 = %f\n",d1);
	//printf("d2 = %f\n",d2);
	
	if ((pp3p[0]>pp3p[1]) && (pp3p[0]>pp3p[2]) && (pp3p[0] > 0)) {
		// pp3p[0] znajdalej od zera
		//printf("x\n");
		invdet = 1 / pp3[0];
		p3[0] = 0;
		p3[1] = n1[2]*d2 - n2[2]*d1;//z;
		p3[2] = n2[1]*d1 - n1[1]*d2;//y;
	} else {
		if ((pp3p[1]>pp3p[2]) && (pp3p[1] > 0)) {
			//printf("y\n");
			// pp3p[1] dalej od zera niz pp3p[2]
			invdet = 1 / pp3[1];
			p3[0] = n1[2]*d2 - n2[2]*d1;
			p3[1] = 0;
			p3[2] = n2[0]*d1 - n1[0]*d2;
		} else {
			if (pp3p[2] > 0) {
				//printf("z\n");
				// pp3p[2] najdalej od zera
				invdet = 1 / pp3[2];
				p3[0] = n1[1]*d2 - n2[1]*d1;
				p3[1] = n2[0]*d1 - n1[0]*d2;
				p3[2] = 0;
			} else {
				return 0;
			}
		}
	}
	//printf("invdet = %f\n",invdet);
	//printf("p3 = (%f,%f,%f)\n",p3[0],p3[1],p3[2]);
	MULT(p3,p3,invdet);
	invdet = 1.0 / (sqrt(pp3p[0]+pp3p[1]+pp3p[2]));
	MULT(pp3,pp3,invdet);
	
	return 1;
	
}

double ISEC_Line_Sphere(double p1[3], double n1[3], double ps[3], double r, double pp1[3], double pp2[3])
{
	double a,b,c,delta,t1,t2;
	double p1p[3], n1p[3];
	// translating coordinates system so the sphere center is located in the (0,0,0)
	SUB(p1p,p1,ps);
	
	a = SQR(n1[0])+SQR(n1[1])+SQR(n1[2]);
	b = 2 * DOT(p1p,n1);
	c = SQR(p1p[0])+SQR(p1p[1])+SQR(p1p[2]) - SQR(r);
	delta = SQR(b) - 4 * a * c;
	printf("delta = %f\n",delta);
	if (delta>0) {
		delta = sqrt(delta);
		t1 = (-b - delta) / (2 * a);
		t2 = (-b + delta) / (2 * a);
		
		SET(pp1,p1p);
		MULT(n1p,n1,t1);
		ADD(pp1,pp1,n1p);
		ADD(pp1,pp1,ps);
		
		SET(pp2,p1p);
		MULT(n1p,n1,t2);
		ADD(pp2,pp2,n1p);
		ADD(pp2,pp2,ps);
		
		return 2.0;
	} else {
		if (delta == 0) {
			t1 = (-b) / (2 * a);
			t2 = 0;
			
			SET(pp1,p1p);
			MULT(n1p,n1,t1);
			ADD(pp1,pp1,n1p);
			ADD(pp1,pp1,ps);
			
			return 1.0;
		} else {
			t1 = t2 = 0;
			return 0.0;
		}
	}
}

double IS_Point_In_Tri(double p[3], double u1[3], double u2[3], double u3[3]) 
{
	int i0, i1;
	double pp[3];
	pp[0] = FABS(p[0]);
	pp[1] = FABS(p[1]);
	pp[2] = FABS(p[2]);
	if ((pp[0]>pp[1]) || (pp[0]>pp[2])) {
		i0 = 0;
		if (pp[1]>pp[2])
			i1 = 2;
		else
			i1 = 1;
	} else {
		i0 = 1;
		i1 = 2;
	}
	
	//printf("i0 = %d, i1 = %d\n",i0,i1);
	POINT_IN_TRI(p,u1,u2,u3);
	i0 = 0;
	i1 = 1;
	POINT_IN_TRI(p,u1,u2,u3);
	
	return 0;
	
}

double IS_Point_In_Cyl(double p1[3], double p2[3], double axis[3], double r, double p[3])
{
			double axisp[3];
			axisp[0] = -axis[0];
			axisp[1] = -axis[1];
			axisp[2] = -axis[2];
			
			//printf("DIST_Point_Line = %f\n",DIST_Point_Line(p,axis,p1));
			//printf("DIST_Point_Plane2 = %f\n", DIST_Point_Plane2(p,p1,axis));
			//printf("DIST_Point_Plane2 = %f\n", DIST_Point_Plane2(p,p2,axisp));
			if ((DIST_Point_Line(p,axis,p1) <= r)
			 && (DIST_Point_Plane2(p,p1,axis) >= 0)
			 && (DIST_Point_Plane2(p,p2,axisp) >= 0)) 
					return 1;
			
			return 0;
}


/*************************************************************************************/
/*************************************************************************************/
/***        Fast Intersection Function between ray/cylinder                       ****/
/***                Graphics Gems (with small modification by R.Szmurlo)          ****/
/***            Cylinder is infinite.                                             ****/
/*************************************************************************************/
int TestIntersionCylinder(double Vc[3], double Pc[3], double r,double Vl[3], double Pl[3], double p1[3], double p2[3])
{
	double RC[3];
	double d;
	double t,s;
	double n[3],O[3],dl[3];
	double ln;
	double in,out;
	double ZERO = 1e-9;
	
	SUB(RC,Pl,Pc);
	CROSS(n,Vl,Vc);
	ln = LENGTH(n);

	if ( (ln<ZERO)&&(ln>-ZERO) ) return 0;

	VERSOR(n,n);

	d= FABS( DOT(RC,n) );

        if (d<=r)
	{
		CROSS(O,RC,Vc);
		t= - DOT(O,n)/ln;
		CROSS(O,n,Vc);
		VERSOR(O,O);
		s= FABS( sqrt(r*r - d*d) / DOT(Vl,O) );

		in=t-s;
		out=t+s;

		if (in<-ZERO)
			if (out<-ZERO) return 0;

		MULT(dl,Vl,in);
    		ADD(p1,Pl,dl);
		MULT(dl,Vl,out);
    		ADD(p2,Pl,dl);

/*		TVector HB=newposition-cylinder._Position;
		pNormal=HB - cylinder._Axis*(HB.dot(cylinder._Axis));
		pNormal.unit();
*/
		if (in == out) 
			return 1;
		else
			return 2;
	}
    
	return 0;
}

/** p1 and p2 lyes on cylinder axis. */
double ISEC_Line_Cyl(double p1[3], double p2[3], double axis[3], double r, double u1[3], double u2[3], double pp1[3], double pp2[3])
{
	double d1, d2;
	double v[3],v1[3], v2[3];
	double pl1[3], pl2[3];
	int i;
	double res1,res2;
	bool b1,b2;
	
	SUB(v,u2,u1);
	i = TestIntersionCylinder(axis,p1,r,v,u1, pp1,pp2);
	/*	printf_vec(pp1);
	printf_vec(pp2);
	printf("%d\n",i);
	*/
	
	// the line is completely out of range of infinite cylinder.
	//if (i==0) return (0);
	
	b1 = false;
	b2 = false;
	
	SUB(v1,p2,p1);
	SUB(v2,p1,p2);
	
	if (i>0) {
		d1 = DIST_Point_Plane2(pp1,p1,v1);
		d2 = DIST_Point_Plane2(pp1,p2,v2);
		if ((d1*d2) >= 0) b1 = true;
	}
	
	if (i>1) {
	
		d1 = DIST_Point_Plane2(pp2,p1,v1);
		d2 = DIST_Point_Plane2(pp2,p2,v2);
		if ((d1*d2) >= 0) b2 = true;
	}
	
	if ((b1==true) && (b2==true)) {
		
		// both points are located between bottom and top of cylinder
		return (double) i;
	} else {		
		
		// Both bottom and top is intersected or side and (bottom or top) is intersected.
		res1 = ISEC_Line_Plane(p1,v1,u1,u2,pl1);
		res2 = ISEC_Line_Plane(p2,v2,u1,u2,pl2);
		
		d1 = DIST_Point_Line(pl1,axis,p1);
		d2 = DIST_Point_Line(pl2,axis,p1);
		if ((b1 == true) || (b2 == true)) {
			
			// only one point is located between top and bottom of cylinder.
			// one another point must intersect either bottom or top of cylinder.
			// double p[3], double n[3], double p1[3], double p2[3], double pp[3])
			
			if (b2==true) 
				SET(pp1,pp2);
			
			if ((d1 <= r) && (d2 <= r)) {
				//printf("Error!!! (d1 <= r) && (d2 <= r) in: %s, %d\n",__FILE__,__LINE__);
				SET(pp1,pl1);
				SET(pp2,pl2);
				return (2);
			}
			if (d1 <= r)
				SET(pp2,pl1);
			if (d2 <= r) 
				SET(pp2,pl1);
		} else {
			
			// both intersection points are located outside teh cylinder bottom and top.
			// checking if intersecting bottom or top.
			
			if (d1<=r) {
				SET(pp1,pl1);
				
				if (d2 <= r) {
					SET(pp2,pl2);
				}	else
				 return (1);
				
				return (2);
			} else {
				if (d2 <= r) {
					SET(pp1,pl2);
					return (1);
				} else {
					return (0);
				}
			}
			return 0;
		}
	}
	return (double) i;
}


double ISEC_Segment_Cyl(double p1[3], double p2[3], double axis[3], double r, double u1[3], double u2[3], double pp1[3], double pp2[3])
{
	double v1[3], v2[3];
	double d1,d2,d3,d4;
	double res;
	
	res = ISEC_Line_Cyl(p1,p2,axis,r,u1,u2,pp1,pp2);
	
	if (res > 0) {
		SUB(v1,u2,u1);
		SUB(v2,u1,u2);
	
		d1 = DIST_Point_Plane2(pp1,u1,v1);
		d2 = DIST_Point_Plane2(pp1,u2,v2);
		
		d3 = DIST_Point_Plane2(pp2,u1,v1);
		d4 = DIST_Point_Plane2(pp2,u2,v2);
		
		//TRACEF( ("d1 = %f, d2 = %f\n",d1,d2) );
		//TRACEF( ("d3 = %f, d4 = %f\n",d3,d4) );
		if (d1*d2 < 0) {
			SET(pp1,pp2);
			res = res - 1;
		} 
		
		if (d3*d4 < 0) {
			res = res - 1;
		}
	}
	return(res);
}

double DIST_Line_Line(double u1[3], double u2[3], double v1[3], double v2[3])
{
	// according to: http://mathworld.wolfram.com/Line-LineDistance.html
	// D = |c.(a x b)| / |a x b|
	double a[3],b[3],c[3],t1[3];
	double n,d;
	SUB(a,u2,u1);
	SUB(b,v2,v1);
	SUB(c,v1,u1);
	
	CROSS(t1,a,b);
	n = FABS(DOT(c,t1));
	d = LENGTH(t1);
	if (d != 0) {
		return(n/d);
	}
	// u1,a - surface normal to line u1, u2
	ISEC_Line_Plane(u1,a, v1,v2, t1);
	d = DIST_Point_Line(t1,a,u1);
	return (d);
}

double ISEC_Line_Plane( double p[3], double n[3], double p1[3], double p2[3], double pp[3])
{
	// according to: http://astronomy.swin.edu.au/~pbourke/geometry/planeline/
	double nomin,D,t;
	double v[3];
	
	D = -DOT(p,n);	
	SUB(v,p1,p2);	
	nomin = DOT(n,v);
	if (nomin != 0) {
		t = ( (DOT(n,p1) + D) / nomin);
	}	else {
		t = 0;
		return(0);
	}
	MULT(v,v,-t);
	ADD(pp,p1,v); 
	
	return (1);
}

double ISEC_Segment_Plane( double p[3], double n[3], double p1[3], double p2[3], double pp[3], double & t)
{
	// according to: http://astronomy.swin.edu.au/~pbourke/geometry/planeline/
	double nomin,D;
	double v[3];
	
	D = -DOT(p,n);	
	SUB(v,p1,p2);	
	nomin = DOT(n,v);
	if (nomin != 0) {
		t = ( (DOT(n,p1) + D) / nomin);
	}	else {
		t = 0;
		return(0);
	}
	MULT(v,v,-t);
	ADD(pp,p1,v); 
	
	if ((t>=0.0) && (t<=1.0))
		return (1);
	else
		return 0;
}

void ROTXcs( double dest[3], double p[3], double cs, double sn)
{
/*
 x' = x
 y' = y*cos q - z*sin q
 z' = y*sin q + z*cos q

        (1    0      0    0)
Rx(q) = (0  cos q  -sin q  0)
        (0 sin q  cos q  0)
        (0    0     0     1)	
	*/
	
	dest[0] = p[0];
	dest[1] = p[1] * cs - p[2] * sn;
	dest[2] = p[1] * sn + p[2] * cs;
}

void ROTYcs( double dest[3], double p[3], double cs, double sn)
{
/*
 x' = x*cos q + z*sin q
 y' = y
z' = z*cos q - x*sin q

        (cos q  0  sin q   0)
Ry(q) = (0      1    0      0)
        (-sin q  0  cos q    0)
        (0      0    0     1) 
	*/
	
	dest[0] = p[0] * cs + p[2] * sn;
	dest[1] = p[1];
	dest[2] = p[2] * cs - p[0] * sn;
}

void ROTZcs( double dest[3], double p[3], double cs, double sn)
{
/*
x' = x*cos q - y*sin q
 y' = x*sin q + y*cos q 
 z' = z 

         ( cos q  -sin q  0  0)
Rz (q) = (sin q  cos q  0  0)
         ( 0        0    1  0)
         ( 0        0    0  1)
	*/
	
	dest[0] = p[0] * cs - p[1] * sn;
	dest[1] = p[0] * sn + p[1] * cs;
	dest[2] = p[2];
}

double CALC_COS_X(double p[3]) 
{
	double v[3] = {1,0,0};
	double pp[3];
	
	VERSOR(pp,p);
	return(DOT(pp,v));
}

double CALC_SIN_X(double p[3])
{
	double pp[3];
	VERSOR(pp,p);
	return(sqrt(SQR(pp[2]) + SQR(pp[1])));
}

double CALC_COS_Y(double p[3])
{
	double v[3] = {0,1,0};
	double pp[3];
	
	VERSOR(pp,p);
	return(DOT(pp,v));
}

double CALC_SIN_Y(double p[3])
{
	double pp[3];
	VERSOR(pp,p);
	return(sqrt(SQR(pp[0]) + SQR(pp[2])));
}

double CALC_COS_Z(double p[3])
{
	double v[3] = {0,0,1};
	double pp[3];
	
	VERSOR(pp,p);
	return(DOT(pp,v));
}

double CALC_SIN_Z(double p[3])
{
	double pp[3];
	VERSOR(pp,p);
	return(sqrt(SQR(pp[0]) + SQR(pp[1])));
}

double CALC_SIN_COS_TO_X_Y(double p[3], double *sn, double *cs)
{
	/*
	1. Calculate sin,cos normal.
	2. To determine the sign of sinus, check the DOT product of (P x X axis) and OZ axis:
		DOT( CROSS(p,OX), OZ)
	*/
	double px[3];
	
	double ax_z[3] = {0,0,1};
	SET(px,p);
	px[2] = 0;
	*sn = CALC_SIN_X(px);
	*cs = CALC_COS_X(px);
	
	px[0] = 0;
	px[1] = - p[2];
	px[2] = p[1];
/*	printf("px = ");printf_vec(px);
	printf("ax_z = ");printf_vec(ax_z);
	printf("DOT of px and ax_z = %f\n",DOT(px,ax_z));*/
	if (DOT(px,ax_z)>0) *sn = -(*sn);
	return (0);
}


double CALC_SIN_COS_TO_Y_X(double p[3], double *sn, double *cs)
{
	double px[3];
	
	double ax_z[3] = {0,0,1};
	SET(px,p);
	px[2] = 0;
	*sn = CALC_SIN_Y(px);
	*cs = CALC_COS_Y(px);
	
	/* CROSS( p, {0,1,0} ) */
	px[0] = p[2];
	px[1] = 0;
	px[2] = - p[0];
/*	printf("px = ");printf_vec(px);
	printf("ax_z = ");printf_vec(ax_z);
	printf("DOT of px and ax_z = %f\n",DOT(px,ax_z));*/
	if (DOT(px,ax_z)>0) *sn = -(*sn);
	return (0);
}


double CALC_SIN_COS_TO_Z_Y(double p[3], double *sn, double *cs)
{
	double px[3];
	
	double ax_x[3] = {1,0,0};
	SET(px,p);
	px[0] = 0;
	*sn = CALC_SIN_Z(px);
	*cs = CALC_COS_Z(px);
	
	/* CROSS( {0,0,1}, p ) */
	px[0] = - p[1];
	px[1] = p[0];
	px[2] = 0;
/*	printf("px = ");printf_vec(px);
	printf("ax_z = ");printf_vec(ax_z);
	printf("DOT of px and ax_z = %f\n",DOT(px,ax_z));*/
	if (DOT(px,ax_x)>0) *sn = -(*sn);
	return (0);
}

void mult33(double dest[3][3], double a[3][3], double b[3][3])
{
    int i, j, k;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            dest[i][j] = 0;
            for (k = 0; k < 3; k++)
                dest[i][j] += a[i][k] * b[k][j];
        }
}

bool IS_Point_In_Tetra(double p[3], double p1[3], double p2[3], double p3[3], double p4[3])
{
	double d1,d2,d3,d4;
	
	d1 = DIST_Point_Plane(p1, p2, p3, p4);
	if (d1 > 0) 
		d1 = DIST_Point_Plane(p, p2, p3, p4);
	else 
		d1 = DIST_Point_Plane(p, p2, p4, p3);

	d2 = DIST_Point_Plane(p2, p1, p3, p4);
	if (d2 > 0) 
		d2 = DIST_Point_Plane(p, p1, p3, p4);
	else 
		d2 = DIST_Point_Plane(p, p1, p4, p3);

	d3 = DIST_Point_Plane(p3, p2, p1, p4);
	if (d3 > 0) 
		d3 = DIST_Point_Plane(p, p2, p1, p4);
	else 
		d3 = DIST_Point_Plane(p, p2, p4, p1);

	d4 = DIST_Point_Plane(p4, p2, p3, p1);
	if (d4 > 0) 
		d4 = DIST_Point_Plane(p, p2, p3, p1);
	else 
		d4 = DIST_Point_Plane(p, p2, p1, p3);

	//printf("d1,d2,d3,d4 = %lf, %lf, %lf, %lf\n",d1,d2,d3,d4);
	if ( (d1>-SMALL_NUMBER) && (d2>-SMALL_NUMBER) && (d3>-SMALL_NUMBER) && (d4>-SMALL_NUMBER) )
		return true;
		
	return false;
}


static void coeffIni (double& A, double& B, double& C,
		      double& D, double& E, double& F,
		      double x_i, double y_i, double z_i,
		      double x_j, double y_j, double z_j)
{
  A = x_i*y_j - y_i*x_j;
  B = x_i*z_j - z_i*x_j;
  C = x_i - x_j;
  D = y_i*z_j - z_i*y_j;
  E = y_i - y_j;
  F = z_i - z_j;
}

static void coeff3Deven (double coeff[4][4], int i,
			 double A, double B, double C,
			 double D, double E, double F,
			 double x_i, double y_i, double z_i)
{
  coeff[i][0] =  x_i*D - y_i*B + z_i*A;
  coeff[i][1] = -y_i*F + z_i*E - D;
  coeff[i][2] =  x_i*F - z_i*C + B;
  coeff[i][3] = -x_i*E + y_i*C - A;
}

static void coeff3Dodd (double coeff[4][4], int i,
			double A, double B, double C,
			double D, double E, double F,
			double x_i, double y_i, double z_i)
{
  coeff[i][0] = -x_i*D + y_i*B - z_i*A;
  coeff[i][1] =  y_i*F - z_i*E + D;
  coeff[i][2] = -x_i*F + z_i*C - B;
  coeff[i][3] = +x_i*E - y_i*C + A;
}

//-----------------------------------------------------------------------------
void calcCoeffInGlobFunc 
  (
         double coeff[4][4],  // N_i=coeff(0,i)+sum_{r=1}^d coeff(r,i)*x_i
         double&           volume,
   		 double       elmcoor[4][3]
  )
//-----------------------------------------------------------------------------
{

  // indices i,j.m,p in Zienkiewicz & Taylor correspond to node 1,2,3,4 here:
  double x_i = elmcoor[0][0];  double y_i = elmcoor[0][1];  double z_i = elmcoor[0][2];
  double x_j = elmcoor[1][0];  double y_j = elmcoor[1][1];  double z_j = elmcoor[1][2];
  double x_m = elmcoor[2][0];  double y_m = elmcoor[2][1];  double z_m = elmcoor[2][2];
  double x_p = elmcoor[3][0];  double y_p = elmcoor[3][1];  double z_p = elmcoor[3][2];
  double A, B, C, D, E, F;

  coeffIni (A, B, C, D, E, F, x_m, y_m, z_m, x_p, y_p, z_p);

  coeff3Dodd (coeff, 0, A, B, C, D, E, F, x_j, y_j, z_j);
  coeff3Deven (coeff, 1, A, B, C, D, E, F, x_i, y_i, z_i);
  coeffIni (A, B, C, D, E, F, x_i, y_i, z_i, x_j, y_j, z_j);
  coeff3Dodd (coeff, 2, A, B, C, D, E, F, x_p, y_p, z_p);
  coeff3Deven (coeff, 3, A, B, C, D, E, F, x_m, y_m, z_m);

  volume = -(coeff[0][0] + coeff[1][0] + coeff[2][0] + coeff[3][0]);
  volume /= 6;

  // according to page 92 in the cited book, 
  // N_i = (coeff(i,0) + coeff(i,1)*x + coeff(i,2)*y) + coeff(i,3)*z/(6*volume)
}

/* Ziemkiewicz: Metoda Elementów Skończonych str. 101.
*/
void calcTetraShapeFuncs(double nodes[4][3], double sf[4][4], double *vol)
{
	
	calcCoeffInGlobFunc(sf, *vol, nodes);
/*

	*vol = VOLUME_tetra(nodes[0], nodes[1], nodes[2], nodes[3]);

	for (i=0; i<4; i++) {
		// ai
		sf[ i ][0] = DET33( nodes[ ind[i] ], nodes[ ind[i+1] ], nodes[ ind[i+2] ] );

		// bi
		mat[0][0] = 1; 		mat[0][1] = nodes[ ind[i]   ][1]; 	mat[0][2] = nodes[ ind[i]   ][2];
		mat[1][0] = 1; 		mat[1][1] = nodes[ ind[i+1] ][1]; 	mat[1][2] = nodes[ ind[i+1] ][2];
		mat[2][0] = 1; 		mat[2][1] = nodes[ ind[i+2] ][1]; 	mat[2][2] = nodes[ ind[i+2] ][2];
		sf[ i ][1] = -det33(mat);
	
		//ci
		mat[0][0] = nodes[ ind[i]   ][0];	mat[0][1] = 1; 		mat[0][2] = nodes[ ind[i]   ][2];
		mat[1][0] = nodes[ ind[i+1] ][0];	mat[1][1] = 1; 		mat[1][2] = nodes[ ind[i+1] ][2];
		mat[2][0] = nodes[ ind[i+2] ][0];	mat[2][1] = 1; 		mat[2][2] = nodes[ ind[i+2] ][2];
		sf[ i ][2] = det33(mat);
	
		//di
		mat[0][0] = nodes[ ind[i]   ][0];mat[0][1] = nodes[ ind[i]   ][1];	mat[0][2] = 1;
		mat[1][0] = nodes[ ind[i+1] ][0];mat[1][1] = nodes[ ind[i+1] ][1]; 	mat[1][2] = 1;
		mat[2][0] = nodes[ ind[i+2] ][0];mat[2][1] = nodes[ ind[i+2] ][1]; 	mat[2][2] = 1;
		sf[ i ][3] = -det33(mat);
	}
*/

}

#define a(x) sf[x][0]
#define b(x) sf[x][1]
#define c(x) sf[x][2]
#define d(x) sf[x][3]

double valTetraPtv( double P[3],double nodes[4][3], double v[4])
{
	double sf[4][4];
	double vol;
	int i;
	double res = 0;
	
	calcTetraShapeFuncs( nodes, sf, &vol );
//	printf("P = %g,%g,%g\n",P[0], P[1], P[2]);
//	printf("volume = %g\n", vol);
	
	for (i=0; i < 4; i++) {
//		printf("ai = %g, bi = %g, ci= %g, di = %g\n", a(i), b(i), c(i), d(i) );
		res += (a(i) + b(i) * P[0] + c(i) * P[1] + d(i) * P[2]) * v[i];
//		printf("res = %g\n",res);
		//printf("vi = %g\n",v[i]);
	}
	vol = fabs(vol);
	res =  res / (6 * vol);

	return(res);
}

#define _i(x) idx[x]
#define _j(x) idx[x+1]
#define _k(x) idx[x+2]
#define _x(a) nodes[a][0]
#define _y(a) nodes[a][1]

double valTriPtv( double P[3],double nodes[3][3], double v[3])
{
	int i;
	int idx[] = {0, 1, 2, 0, 1};
	double a[3], b[3], c[3];
	double surf = 0;
	
	for (i=0; i < 3; i++) {
		a[i] = _x(_j(i))*_y(_k(i)) - _x(_k(i))*_y(_j(i)); 
		b[i] = _y(_j(i)) - _y(_k(i));
		c[i] = _x(_k(i)) - _x(_j(i));
	}
	
	surf = _x(_j(0)) * _y(_k(0)) - _y(_j(0)) * _x(_k(0)) - _x(_i(0)) * _y(_k(0)) + _x(_k(0)) * _y(_i(0)) + _x(_i(0)) * _y(_j(0)) - _x(_j(0)) * _y(_i(0));

	double rv = 0;
	if (surf !=0) {
		for (i=0; i < 3; i++) 
			rv += (a[i] + b[i] * P[0] + c[i] * P[1]) * v[i] / surf;
	}

	return (rv);
	
}

void tensLoc2Glob( double direction[3], double Tr, double Trn, double globtensor[3][3])
{
	int i,j;
	double sigma[3][3];
	double tmp[3][3];
	double A[3][3];
	double At[3][3];
	double cb,sb=0,ca,sa;
	double lv, // length of direction in 3d
		   lp; // length of direction in 2d on plane OXY
	
	for (i=0; i < 3; i++)
		for (j=0; j < 3; j++)
			sigma[i][j] = 0;
	sigma[0][0] = Trn;
	sigma[1][1] = Trn;
	sigma[2][2] = Tr;
	
	lv = LENGTH(direction);
	lp = sqrt(direction[0]*direction[0]+direction[1]*direction[1]);
	if (lp<1e-20) {
		sa = 0;
		ca = 1;
	} else {
		sa = direction[1] / lp;
		ca = direction[0] / lp;
		sb = lp / lv;
	}

	if (lv < 1e-20) {
		cb = 1;
		sa = 0;
	} else {
		cb = direction[2] / lv;
		sb = lp / lv;
	}

//	printf("lv = %lf, lp = %lf\n", lv, lp);

	A[0][0] = ca * cb;    A[0][1] = -sa * cb;    A[0][2] = -sb;
	A[1][0] = sa;         A[1][1] = ca;          A[1][2] = 0;
	A[2][0] = ca * sb;    A[2][1] = -sa * sb;    A[2][2] = cb;
	
	// if (DEBUG) {printf("A: "); print_matrix(A); printf("\n");};
	for (i=0; i<3;i++)
		for (j=0;j<3;j++)
			At[i][j] = A[j][i];

	// if (DEBUG) {printf("At: "); print_matrix(At); printf("\n");};
	mult33(tmp, At, sigma);
	// if (DEBUG) {printf("tmp: "); print_matrix(tmp); printf("\n");};
	mult33(globtensor, tmp, A );
	
}
 

void printf_matrix33( double a[3][3] )
{
    int i,j;
    double suma = 0;
    printf("[\n");
    for (i=0; i<3; i++, printf("\n"))
        for (j=0;j<3;j++) {
            printf("%g ",a[i][j]);
            suma += a[i][j];
	    }
    printf("] suma = %g\n", suma);
}

void PROJ_Point_Plane( double P[3], double P1[3], double N[3], double Result[3] )
{
	double t;
	double D;
	double V[3];
        VERSOR(V,N);
	D = - DOT(P1,V);
	t = - (DOT(V,P) + D);
	
	MULT(Result,V,t);
	ADD(Result,P,Result);
}

void Helpers::rotate( double p[3], double s[3], double a)
{
//        cout << "rot:"
//             << "p(" << p[0] << "," << p[1] << "," << p[2] << "), "
//             << "s(" << s[0] << "," << s[1] << "," << s[2] << "), "
//             << "@" << a << "\n";
        double ux,uy,uz,vx,vy,vz,sa,ca,wx,wy,wz;
        double x, y, z, u, v, w;

        x = p[0]; y = p[1]; z = p[2];
        u = s[0]; v = s[1]; w = s[2];

        ux=u*x;
        uy=u*y;
        uz=u*z;
        vx=v*x;
        vy=v*y;
        vz=v*z;
        wx=w*x;
        wy=w*y;
        wz=w*z;

        sa=sin(a);
        ca=cos(a);

        p[0] = u*(ux+vy+wz)+(x*(v*v+w*w)-u*(vy+wz))*ca+(-wy+vz)*sa;
        p[1] = v*(ux+vy+wz)+(y*(u*u+w*w)-v*(ux+wz))*ca+(wx-uz)*sa;
        p[2] = w*(ux+vy+wz)+(z*(u*u+v*v)-w*(ux+vy))*ca+(-vx+uy)*sa;
}

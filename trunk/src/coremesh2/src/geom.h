#ifndef _GEOM_H
#define _GEOM_H

#include <math.h>
#include <stdio.h>

#define FABS(x) ((double)fabs(x))        /* implement as is fastest on your machine */

/* if USE_EPSILON_TEST is true then we do a check: 
         if |dv|<EPSILON then dv=0.0;
   else no check is done (which is less robust)
*/
#define USE_EPSILON_TEST TRUE  
#define EPSILON 0.000001

#define SQR(x) ((x)*(x))


/* some macros */
#define CROSS(dest,v1,v2)                      \
              dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
              dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
              dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DET33(a,b,c) \
						(a[0]*b[1]*c[2]+a[1]*b[2]*c[0]+a[2]*b[0]*c[1] \
						-a[0]*b[2]*c[1]-a[1]*b[0]*c[2]-a[2]*b[1]*c[0]) 

#define det33(mat) DET33(mat[0],mat[1],mat[2])
		
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) dest[0]=v1[0]-v2[0]; dest[1]=v1[1]-v2[1]; dest[2]=v1[2]-v2[2]; 

#define ADD(dest,v1,v2) dest[0]=v1[0]+v2[0]; dest[1]=v1[1]+v2[1]; dest[2]=v1[2]+v2[2]; 

#define MULT(dest,v,factor) dest[0]=factor*v[0]; dest[1]=factor*v[1]; dest[2]=factor*v[2];

#define SET(dest,src) dest[0]=src[0]; dest[1]=src[1]; dest[2]=src[2];

#define ZERO(dest) dest[0]=0.0; dest[1]=0.0; dest[2]=0.0;

#define SET3v(dest,v1,v2,v3) {dest[0]=(v1);dest[1]=(v2);dest[2]=(v3);}
/*szmurlor*/
#define V_AVG(dest,v1,v2,v3) dest[0] = (v1[0]+v2[0]+v3[0])/3; \
                             dest[1] = (v1[1]+v2[1]+v3[1])/3; \
														 dest[2] = (v1[2]+v2[2]+v3[2])/3;
#define VERSOR(dest,__v1) {double __length = sqrt(SQR(__v1[0])+SQR(__v1[1])+SQR(__v1[2])); \
												dest[0] = (__length==0?\
                                  0:\
																	__v1[0] / __length);\
												dest[1] = (__length==0?\
                                  0:\
																	__v1[1] / __length);\
												dest[2] = (__length==0?\
                                  0:\
																	__v1[2] / __length); \
												}
#define LENGTH(x) (sqrt(SQR(x[0])+SQR(x[1])+SQR(x[2])))



/* sort so that a<=b */
#define SORT(a,b)       \
             if(a>b)    \
             {          \
               double c; \
               c=a;     \
               a=b;     \
               b=c;     \
             }

#define ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1) \
              isect0=VV0+(VV1-VV0)*D0/(D0-D1);    \
              isect1=VV0+(VV2-VV0)*D0/(D0-D2);


#define COMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,isect0,isect1) \
  if(D0D1>0.0f)                                         \
  {                                                     \
    /* here we know that D0D2<=0.0 */                   \
    /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
    ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
  }                                                     \
  else if(D0D2>0.0f)                                    \
  {                                                     \
    /* here we know that d0d1<=0.0 */                   \
    ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
  }                                                     \
  else if(D1*D2>0.0f || D0!=0.0f)                       \
  {                                                     \
    /* here we know that d0d1<=0.0 or that D0!=0.0 */   \
    ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1);          \
  }                                                     \
  else if(D1!=0.0f)                                     \
  {                                                     \
    ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
  }                                                     \
  else if(D2!=0.0f)                                     \
  {                                                     \
    ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
  }                                                     \
  else                                                  \
  {                                                     \
    /* triangles are coplanar */                        \
    return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2);      \
  }



/* this edge to edge test is based on Franlin Antonio's gem:
   "Faster Line Segment Intersection", in Graphics Gems III,
   pp. 199-202 */ 
#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0[i0]-U1[i0];                                   \
  By=U0[i1]-U1[i1];                                   \
  Cx=V0[i0]-U0[i0];                                   \
  Cy=V0[i1]-U0[i1];                                   \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return 1;                      \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return 1;                      \
    }                                                 \
  }                                

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
  double Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
  Ax=V1[i0]-V0[i0];                            \
  Ay=V1[i1]-V0[i1];                            \
  /* test edge U0,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U0,U1);                    \
  /* test edge U1,U2 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U1,U2);                    \
  /* test edge U2,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
  double a,b,c,d0,d1,d2;                     \
  /* is T1 completly inside T2? */          \
  /* check if V0 is inside tri(U0,U1,U2) */ \
  a=U1[i1]-U0[i1];                          \
  b=-(U1[i0]-U0[i0]);                       \
  c=-a*U0[i0]-b*U0[i1];                     \
  d0=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U2[i1]-U1[i1];                          \
  b=-(U2[i0]-U1[i0]);                       \
  c=-a*U1[i0]-b*U1[i1];                     \
  d1=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U0[i1]-U2[i1];                          \
  b=-(U0[i0]-U2[i0]);                       \
  c=-a*U2[i0]-b*U2[i1];                     \
  d2=a*V0[i0]+b*V0[i1]+c;                   \
  if(d0*d1>0.0)                             \
  {                                         \
    if(d0*d2>0.0) return 1;                 \
  }                                         \
}

#define printf_vec(x) printf("%f,%f,%f\n",x[0],x[1],x[2]);

double
normalny4p(double * w1, double * w2, double * w3, double * w4,
           double wynik[3]);

void NORM(double n[3], double n1[3], double n2[3], double n3[3]);

double DIST_Point_Plane(double P[3], double n1[3], double n2[3], double n3[3]);
/* P - Punkt
 * PS - punkt na plaszczyznie
 * N - wektor normalny
 */
double DIST_Point_Plane2(double P[3], double PS[3], double N[3]);

double DIST_Point_Point(double P1[3], double P2[3] );

/* N - kierunek linii 
   PL - punkt
   Czyli linia: Px = L2[0] + t * L1[0], Py = L2[1] + t * L1[1], itd.
   */
double DIST_Point_Line(double P1[3], double N[3], double PL[3] );

/* N - kierunek linii
   PL - punkt na linii
   P - ktorego odleglosc licze.
   Pout - najblizszy punkt na linii */
double DIST_Point_Line2(double P1[3], double N[3], double PL[3], double Pout[3] );


/**
u1, u2 - point of line 1
v1, v2 - point of line 2
*/
double DIST_Line_Line(double u1[3], double u2[3], double v1[3], double v2[3]);
void Shortest_Segment_Between_Lines(double P1[3], double P2[3], double P3[3], double P4[3], double dest1[3], double dest2[3]);
double VOLUME_tetra(double n0[3], double n1[3], double n2[3], double n3[3]);

/**
 * p1,n1 - p1 - point of plane 1, n1 - normal of plane 1;
 * p2,n2 - p2 - point of plane 2, n2 - normal of plane 2;
 * p3,pp3 - p3 - point of intersecting plane, pp3 - direction vector of intersecting line.
 * */
double ISEC_Plane_Plane(double p1[3], double n1[3], double p2[3], double n2[3],
		       double p3[3], double pp3[3]);
/**
	return value:
 		number of intersection points.
		if (return value = 0) then pp1 and pp2 are undefined.
*/
 double ISEC_Line_Sphere(double p1[3], double n1[3], double ps[3], double r, double pp1[3], double pp2[3]);

// jezeli punkt jest na krawedzi lub punkcie zwraca 0- czyli nie jest.
// 0 - nie jest w srodku.
// 1 - jest w srodku.
double IS_Point_In_Tri(double p[3], double u1[3], double u2[3], double u3[3]);

int ISEC_Segment_Tetra( double p1[3], double n1[3], double nodes[4][3], double p[2][3] );

//
// p1 - center of bottom base
// p2 - center of top base
// axis - vector of symetry axis with direction from p1 ---> p2
// r - radius of cylinder
// p -point of interest
double IS_Point_In_Cyl(double p1[3], double p2[3], double axis[3], double r, double p[3]);

bool IS_Point_In_Tetra(double p[3], double p1[3], double p2[3], double p3[3], double p4[3]);

double ISEC_Line_Cyl(double p1[3], double p2[3], double axis[3], double r, double u1[3], double u2[3], double pp1[3], double pp2[3]);

double ISEC_Segment_Cyl(double p1[3], double p2[3], double axis[3], double r, double u1[3], double u2[3], double pp1[3], double pp2[3]);

double ISEC_Line_Plane( double p[3], double n[3], double p1[3], double p2[3], double pp[3]);
double ISEC_Segment_Plane( double p[3], double n[3], double p1[3], double p2[3], double pp[3], double &t);

/* angle is in RADIANS */
void ROTXcs( double dest[3], double p[3], double cs, double sn);
void ROTYcs( double dest[3], double p[3], double cs, double sn);
void ROTZcs( double dest[3], double p[3], double cs, double sn);

// CALC_COS_X - calculates cosinus of angle between vector [ p[0], p[1], p[2] ] and Versor: [1, 0 , 0]
double CALC_COS_X(double p[3]);
double CALC_SIN_X(double p[3]);
double CALC_COS_Y(double p[3]);
double CALC_SIN_Y(double p[3]);
double CALC_COS_Z(double p[3]);
double CALC_SIN_Z(double p[3]);

/* Calculate sin and cos for rotation to eliminate Y coordinate in vector after 
   rotation, around Z axis. (If we rotate vector round Z axis,
   p[1] (py) coordinate will be zero.)
	 Example:
	 	double p[3] = {1,1,1};
	 	CALC_SIN_COS_TO_X_Y(p,&sn,&cs);
		ROTZcs(p,p,cs,sn);
	 Result:
	  p = {1,414,0,1};
 */
double CALC_SIN_COS_TO_X_Y(double p[3], double *sn, double *cs);

/* sin cos for rotation to eliminate x after rotation around z axis.*/
double CALC_SIN_COS_TO_Y_X(double p[3], double *sn, double *cs);

/*sin cos for rotation to eliminate z after rotation around x axis.*/
double CALC_SIN_COS_TO_Z_Y(double p[3], double *sn, double *cs);

void mult33( double dest[3][3], double a[3][3], double b[3][3]);

/* Ziemkiewicz: Metoda Elementów Skończonych str. 101 */
void calcTetraShapeFuncs(double nodes[4][3], double sf[4][4], double * vol);

double valTetraPtv( double P[3], double nodes[4][3], double v[4]);
double valTriPtv( double P[3], double nodes[3][3], double v[3]);


void tensLoc2Glob( double direction[3], double Tr, double Trn, double globtensor[3][3]);

void printf_matrix33( double a[3][3] );

/* Project point on plane.
   P - point to be projected
   P1 and N - definition of the plane
 */
void PROJ_Point_Plane( double P[3], double P1[3], double N[3], double Result[3] );

namespace Helpers
{
    void rotate(double[3], double[3], double);
}
#endif

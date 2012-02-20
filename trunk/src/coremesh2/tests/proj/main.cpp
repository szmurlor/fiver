#include <geom.h>
#include <stdlib.h>

int main(int argc, char ** argv )
{
    double P1[3], N[3], P[3], Result[3];

    P1[0]  = atof(argv[1]);
    P1[1]  = atof(argv[2]);
    P1[2]  = atof(argv[3]);
    N[0]  = atof(argv[4]);
    N[1]  = atof(argv[5]);
    N[2]  = atof(argv[6]);
    P[0]  = atof(argv[7]);
    P[1]  = atof(argv[8]);
    P[2]  = atof(argv[9]);
    
    PROJ_Point_Plane( P, P1, N, Result );

    printf("Result = %lf, %lf, %lf\n", Result[0], Result[1], Result[2] );


    return 0;
}

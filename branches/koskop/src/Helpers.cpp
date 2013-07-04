#include "Helpers.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <fviewer.h>
#include <fivermain.h>
#include <geom.h>

void Helpers::drawCube( double P[], double VX[], double VY[], double VZ[] )
{
        glBegin(GL_QUADS);
        glNormal3f(VZ[0], VZ[1], VZ[2] );
        glVertex3f(P[0] -VX[0]+VY[0]+VZ[0], P[1] -VX[1]+VY[1]+VZ[1], P[2] -VX[2]+VY[2]+VZ[2]);
        glVertex3f(P[0] -VX[0]-VY[0]+VZ[0], P[1] -VX[1]-VY[1]+VZ[1], P[2] -VX[2]-VY[2]+VZ[2]);
        glVertex3f(P[0] +VX[0]-VY[0]+VZ[0], P[1] +VX[1]-VY[1]+VZ[1], P[2] +VX[2]-VY[2]+VZ[2]);
        glVertex3f(P[0] +VX[0]+VY[0]+VZ[0], P[1] +VX[1]+VY[1]+VZ[1], P[2] +VX[2]+VY[2]+VZ[2]);

        glNormal3f(-VZ[0], -VZ[1], -VZ[2] );
        glVertex3f(P[0] -VX[0]+VY[0]-VZ[0], P[1] -VX[1]+VY[1]-VZ[1], P[2] -VX[2]+VY[2]-VZ[2]);
        glVertex3f(P[0] -VX[0]-VY[0]-VZ[0], P[1] -VX[1]-VY[1]-VZ[1], P[2] -VX[2]-VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]-VY[0]-VZ[0], P[1] +VX[1]-VY[1]-VZ[1], P[2] +VX[2]-VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]+VY[0]-VZ[0], P[1] +VX[1]+VY[1]-VZ[1], P[2] +VX[2]+VY[2]-VZ[2]);

        glNormal3f(VY[0], VY[1], VY[2] );
        glVertex3f(P[0] -VX[0]+VY[0]+VZ[0], P[1] -VX[1]+VY[1]+VZ[1], P[2] -VX[2]+VY[2]+VZ[2]);
        glVertex3f(P[0] -VX[0]+VY[0]-VZ[0], P[1] -VX[1]+VY[1]-VZ[1], P[2] -VX[2]+VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]+VY[0]-VZ[0], P[1] +VX[1]+VY[1]-VZ[1], P[2] +VX[2]+VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]+VY[0]+VZ[0], P[1] +VX[1]+VY[1]+VZ[1], P[2] +VX[2]+VY[2]+VZ[2]);

        glNormal3f(-VY[0], -VY[1], -VY[2] );
        glVertex3f(P[0] -VX[0]-VY[0]+VZ[0], P[1] -VX[1]-VY[1]+VZ[1], P[2] -VX[2]-VY[2]+VZ[2]);
        glVertex3f(P[0] -VX[0]-VY[0]-VZ[0], P[1] -VX[1]-VY[1]-VZ[1], P[2] -VX[2]-VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]-VY[0]-VZ[0], P[1] +VX[1]-VY[1]-VZ[1], P[2] +VX[2]-VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]-VY[0]+VZ[0], P[1] +VX[1]-VY[1]+VZ[1], P[2] +VX[2]-VY[2]+VZ[2]);


        glNormal3f(-VX[0], -VX[1], -VX[2] );
        glVertex3f(P[0] -VX[0]-VY[0]+VZ[0], P[1] -VX[1]-VY[1]+VZ[1], P[2] -VX[2]-VY[2]+VZ[2]);
        glVertex3f(P[0] -VX[0]-VY[0]-VZ[0], P[1] -VX[1]-VY[1]-VZ[1], P[2] -VX[2]-VY[2]-VZ[2]);
        glVertex3f(P[0] -VX[0]+VY[0]-VZ[0], P[1] -VX[1]+VY[1]-VZ[1], P[2] -VX[2]+VY[2]-VZ[2]);
        glVertex3f(P[0] -VX[0]+VY[0]+VZ[0], P[1] -VX[1]+VY[1]+VZ[1], P[2] -VX[2]+VY[2]+VZ[2]);

        glNormal3f(VX[0], VX[1], VX[2] );
        glVertex3f(P[0] +VX[0]-VY[0]+VZ[0], P[1] +VX[1]-VY[1]+VZ[1], P[2] +VX[2]-VY[2]+VZ[2]);
        glVertex3f(P[0] +VX[0]-VY[0]-VZ[0], P[1] +VX[1]-VY[1]-VZ[1], P[2] +VX[2]-VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]+VY[0]-VZ[0], P[1] +VX[1]+VY[1]-VZ[1], P[2] +VX[2]+VY[2]-VZ[2]);
        glVertex3f(P[0] +VX[0]+VY[0]+VZ[0], P[1] +VX[1]+VY[1]+VZ[1], P[2] +VX[2]+VY[2]+VZ[2]);

        glEnd();
}

void Helpers::unprojectPointOnPlane(double dest[3], double P[2], double planeP[3], double planeN[3])
{
    GLint viewport[4];
    GLdouble modelViewMatrix[16];
    GLdouble projectionMatrix[16];
    double A[3];
    double B[3];
    ZERO(A);
    ZERO(B);

    FViewer * fv = getCurrentViewer();
    fv->camera()->getViewport(viewport);
    fv->camera()->getModelViewMatrix(modelViewMatrix);
    fv->camera()->getProjectionMatrix(projectionMatrix);

    gluUnProject(P[0], P[1], 0.0, modelViewMatrix, projectionMatrix, viewport, &A[0], &A[1], &A[2]);
    gluUnProject(P[0], P[1], 1.0, modelViewMatrix, projectionMatrix, viewport, &B[0], &B[1], &B[2]);

    ISEC_Line_Plane(planeP, planeN, A, B, dest);
}

void Helpers::unprojectPointOnLine(double dest[3], double P1[3], double P2[3], double point[2])
{
    int viewport[4];
    GLdouble modelViewMatrix[16];
    GLdouble projectionMatrix[16];

    FViewer * fv = getCurrentViewer();
    fv->camera()->getViewport(viewport);
    fv->camera()->getModelViewMatrix(modelViewMatrix);
    fv->camera()->getProjectionMatrix(projectionMatrix);

    double P3[3];
    double P4[3];
    double notused[3];
    ZERO(P3);
    ZERO(P4);
    ZERO(notused);

    gluUnProject(point[0], point[1], 0.0, modelViewMatrix, projectionMatrix, viewport, &P3[0], &P3[1], &P3[2]);
    gluUnProject(point[0], point[1], 1.0, modelViewMatrix, projectionMatrix, viewport, &P4[0], &P4[1], &P4[2]);
    Shortest_Segment_Between_Lines(P1, P2, P3, P4, dest, notused);
}

double Helpers::getAngle(double A[3], double B[3], double O[3], double C[3])
{
    double S[3];
    double E[3];
    double a[3];
    double b[3];
    double c[3];

    SUB( S, A, O );
    SUB( E, B, O );
    if(LENGTH(S) == 0.0 || LENGTH(E) == 0.0)
        return 0.0;
    CROSS(a,S,E);
    VERSOR(b, a);
    VERSOR(c, C);
    //    qDebug() << "a: " << a[0] << "," << a[1] << "," << a[2];
    //    qDebug() << "b: " << b[0] << "," << b[1] << "," << b[2];
    //    qDebug() << "c: " << c[0] << "," << c[1] << "," << c[2];
    //    qDebug() << "c':" << c[0] << "," << c[1] << "," << c[2];
    //    qDebug() << "S: " << S[0] << "," << S[1] << "," << S[2];
    //    qDebug() << "E: " << E[0] << "," << E[1] << "," << E[2];
    double direction = DOT(c, b);
    //qDebug() << "@direction: " << direction;
    double sinx = direction*LENGTH(a)/(LENGTH(S)*LENGTH(E));
    double angle = asin(sinx);
    return angle;
}


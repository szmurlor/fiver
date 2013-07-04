#ifndef HELPERS_H
#define HELPERS_H

namespace Helpers
{
    void drawCube( double[], double[], double[], double[] );
    void unprojectPointOnPlane(double[3], double[2], double[3], double[3]);
    void unprojectPointOnLine(double[3], double[3], double[3], double[2]);
    double getAngle(double[3], double[3], double[3], double[3]);
}
#endif // HELPERS_H

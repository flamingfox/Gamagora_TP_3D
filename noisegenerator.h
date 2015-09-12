#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "math.h"

class NoiseGenerator
{
protected :


public :
    NoiseGenerator();

    static double perlinNoise(double x, double y, float harmonique);
    static double perlinNoise(double x, double y);

private :

    static double findnoise2(double x, double y, float harmonique);
    static double interpolate(double a,double b,double x);
};

#endif // NOISEGENERATOR_H

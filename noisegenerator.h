#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "math.h"
#include "parametres.h"

namespace NoiseGenerator
{
    double perlinNoise(double x, double y);
    double perlinNoise(double x, double y, int seed);


    double findnoise(double x, double y, int seed);
    double interpolate(double a,double b,double x);
}

#endif // NOISEGENERATOR_H

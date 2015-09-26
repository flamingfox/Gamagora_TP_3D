#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "math.h"
#include "parametres.h"

namespace NoiseGenerator
{
    static double perlinNoise(double x, double y);
    static double perlinNoise(double x, double y, int seed);


    static double findnoise(double x, double y, int seed);
    static double interpolate(double a,double b,double x);
}

#endif // NOISEGENERATOR_H

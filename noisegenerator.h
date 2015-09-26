#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "math.h"
#include "parametres.h"

namespace NoiseGenerator
{
    double perlinNoise(double x, double y);
    double perlinNoise(double x, double y, int seed);

    double perlinNoiseGradiant(double x, double y, double res);
    double perlinNoiseGradiant2(double x, double y, double res);

}

#endif // NOISEGENERATOR_H

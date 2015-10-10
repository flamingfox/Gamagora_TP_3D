#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "math.h"
#include "parametres.h"
#include "stdlib.h"

/**
 * fonction de bruit
 */
namespace NoiseGenerator
{
    double perlinNoise(double x, double y);

    /**
     * @brief Détermine le value noise en interpolant les noises voisins
     * @param[in] x Position en x.
     * @param[in] y Position en y.
     * @param[in] seed
     * @return Le value noise à la position x, y.
     */
    double perlinNoise(double x, double y, int seed);

    /**
     * @return bruit de perlin par gradient entre 0 et 1
     */
    double perlinNoiseGradiant(double x, double y, double res);

    /**
     * @return bruit de perlin par gradient entre -1 et 1
     */
    double perlinNoiseGradiant2(double x, double y, double res);

    /**
    * @param max valeur maximale que la fonction peut retourner
    * @return une valeur entre 0 et max
    */
    double random(double max);

    /**
    * @param max valeur maximale que la fonction peut retourner
    * @return une valeur entre -max et max
    */
    double random2(double max);

}

#endif // NOISEGENERATOR_H

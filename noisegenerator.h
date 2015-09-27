#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "math.h"
#include "parametres.h"

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
     * @brief Calcul du value noise à la position x, y.
     * @param[in] x Position en x.
     * @param[in] y Position en y.
     * @param[in] seed
     * @return Le value noise à la position x, y.
     */
    double findnoise(double x, double y, int seed);

    /**
     * @brief Interpole entre la valeur a et b selon x.
     * @param[in] a Premier extermité de l'interpolation.
     * @param[in] b Second extermité de l'interpolation.
     * @param[in] x Valeur d'interpolation. Valeur entre 0 et 1.
     * @return L'interpolation entre a et b.
     */
    double interpolate(double a,double b,double x);

    /**
     * @return bruit de perlin par gradient entre 0 et 1
     */
    double perlinNoiseGradiant(double x, double y, double res);

    /**
     * @return bruit de perlin par gradient entre -1 et 1
     */
    double perlinNoiseGradiant2(double x, double y, double res);

}

#endif // NOISEGENERATOR_H

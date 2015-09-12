#include "zoneterrain.h"

ZoneTerrain::ZoneTerrain(Eigen::Vector2f centre, float altitude, float rayonAttenuation, float harmoniqueNoise, int coefficientNoiseX, int coefficientNoiseY, float amplitudeNoise) :
centre(centre), altitude(altitude), rayonAttenuation(rayonAttenuation), coefficientNoiseX(coefficientNoiseX),
coefficientNoiseY(coefficientNoiseY), amplitudeNoise(amplitudeNoise), harmoniqueNoise(harmoniqueNoise)
{}

ZoneTerrain::ZoneTerrain(Eigen::Vector2f centre, float altitude, float rayonAttenuation, float harmoniqueNoise, int coefficientNoiseX, int coefficientNoiseY) :
ZoneTerrain(centre, altitude, rayonAttenuation, harmoniqueNoise, 1, 1, 1)
{}

ZoneTerrain::ZoneTerrain(Eigen::Vector2f centre, float altitude, float rayonAttenuation, float harmoniqueNoise) :
    ZoneTerrain(centre, altitude, rayonAttenuation, harmoniqueNoise, 1, 1)
{}

float ZoneTerrain::getAltitudePoint(float x, float y)
{
    float retour = altitude;

    retour += NoiseGenerator::perlinNoise(x/coefficientNoiseX, y/coefficientNoiseY, harmoniqueNoise)*amplitudeNoise;

    return retour;
}

float ZoneTerrain::getPonderationPoint(float x, float y)
{
    Eigen::Vector2f vecteur = Eigen::Vector2f(x, y)-centre;

    if(vecteur.norm() > rayonAttenuation)
        return 0.0;

     float retour = ( 1 - ( (vecteur.norm()*vecteur.norm()) / (rayonAttenuation * rayonAttenuation) ) );

     return retour*retour;
}


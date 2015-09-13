#ifndef ZONETERRAIN_H
#define ZONETERRAIN_H

#include "Eigen/Core"

#include "noisegenerator.h"

class ZoneTerrain
{
protected :
    Eigen::Vector2f centre;
    float altitude;
    float rayonAttenuation;
    float coefficientNoiseX;
    float coefficientNoiseY;
    float amplitudeNoise;

public:
    ZoneTerrain(Eigen::Vector2f centre, float altitude, float rayonAttenuation, int coefficientNoiseX, int coefficientNoiseY, float amplitudeNoise);
    ZoneTerrain(Eigen::Vector2f centre, float altitude, float rayonAttenuation, int coefficientNoiseX, int coefficientNoiseY);
    ZoneTerrain(Eigen::Vector2f centre, float altitude, float rayonAttenuation);

    float getAltitudePoint(float x, float y);
    float getPonderationPoint(float x, float y);

};

#endif // ZONETERRAIN_H

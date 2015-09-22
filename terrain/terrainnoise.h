#ifndef TERRAINNOISE_H
#define TERRAINNOISE_H

#include "parametres.h"
#include "terrain2.h"
#include "noisegenerator.h"

class TerrainNoise: public Terrain2
{
public:
    TerrainNoise();
    TerrainNoise(int _longueur, int _largeur);



protected:
    float getHauteurXY(float x, float y) const;
    Eigen::Vector3f getNormalXY(float x, float y) const;


    float noise(int amplitude, float periode, float x, float y)const;
    float ridge(float hauteur, float seuil)const;
};

#endif // TERRAINNOISE_H

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
    float getHauteurXYSansVerif(float x, float y) const;
    Eigen::Vector3f getNormalXY(float x, float y) const;



    /// \brief Terrain::noise
    /// \param amplitude
    /// \param periode
    /// \param x
    /// \param y
    /// \return return a random controlled value from a perlin noise
    float noise(int amplitude, float periode, float x, float y)const;
    float ridge(float hauteur, float seuil)const;

    float getMinElevation2() const;
    float getMaxElevation2() const;
};

#endif // TERRAINNOISE_H

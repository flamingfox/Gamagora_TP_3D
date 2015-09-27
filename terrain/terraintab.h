#ifndef TERRAINTAB_H
#define TERRAINTAB_H

#include <QImage>
#include "interpolation.h"
#include "terrain.h"

class TerrainTab: public Terrain
{
public:
    TerrainTab():   height(0),  width(0),   amplitude(0),   hauteurMin(0),  hauteurMax(0){}
    TerrainTab(const QImage& img, float longueur, float largeur, float amplitude = 1.0f);
    TerrainTab(const QImage& img, int _nbHeight, int _nbWidth, float longueur, float largeur, float _amplitude = 1.0f);
    TerrainTab(const TerrainTab& copy);

    ~TerrainTab();

    int height, width;
    float *grille = nullptr, **grille2d = nullptr;

    float amplitude;
    float hauteurMin, hauteurMax;



private:

    float getHauteurXY(float x, float y) const;
    Eigen::Vector3f getNormalXY(float x, float y) const;

    void initGrille();
    void simpleInitImage(const QImage& img);

    inline float get(int x, int y) const;

    inline void updateMinElevation();
    inline void updateMaxElevation();
    inline void updateElevation();


    float minElevation() const;
    float maxElevation() const;
};


/*************************************************************************************/
//fonction inline



/// \brief TerrainTab::updateMaxElevation
inline void TerrainTab::updateMinElevation()
{
    hauteurMin = grille[0];
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 1; i < height*width; i++){
        float hauteur = grille[i];
        if(hauteur<hauteurMin)    hauteurMin=hauteur;
    }
}

/// \brief TerrainTab::updateMinElevation
inline void TerrainTab::updateMaxElevation()
{
    hauteurMax = grille[0];
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 1; i < height*width; i++){
        float hauteur = grille[i];
        if(hauteur>hauteurMax)    hauteurMax = hauteur;
    }
}


inline void TerrainTab::updateElevation()
{
    hauteurMin = grille[0];
    hauteurMax = grille[0];
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 1; i < height*width; i++){
        float hauteur = grille[i];
        if(hauteur<hauteurMin)    hauteurMin=hauteur;
        if(hauteur>hauteurMax)    hauteurMax = hauteur;
    }
}


#endif // TERRAINTAB_H

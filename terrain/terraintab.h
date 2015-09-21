#ifndef TERRAINTAB_H
#define TERRAINTAB_H

#include <QImage>
#include "interpolation.h"
#include "terrain2.h"

class TerrainTab: Terrain2
{
public:
    TerrainTab():   height(0),  width(0),   amplitude(0)    {}
    TerrainTab(const QImage& img, float longueur, float largeur, float amplitude = 1.0f);
    TerrainTab(const QImage& img, int _nbHeight, int _nbWidth, float longueur, float largeur, float _amplitude = 1.0f);
    TerrainTab(const TerrainTab& copy);

    ~TerrainTab();

    int height, width;
    float *grille = nullptr, **grille2d = nullptr;

    float amplitude;

private:

    float getHauteurXY(float x, float y) const;
    Eigen::Vector3f getNormalXY(float x, float y) const;

    void initGrille();
    void simpleInitImage(const QImage& img);

    inline float get(int x, int y) const;
};

#endif // TERRAINTAB_H

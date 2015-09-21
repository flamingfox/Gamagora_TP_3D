#ifndef TERRAINTAB_H
#define TERRAINTAB_H

#include <QImage>
#include "interpolation.h"
#include "terrain2.h"

class TerrainTab: Terrain2
{
public:
    TerrainTab():   Terrain2(1000,1000), height(0),  width(0),   amplitude(0)    {}
    TerrainTab(const QImage& img, float amplitude = 1.0f);
    TerrainTab(const QImage& img, int _nbHeight, int _nbWidth, float _amplitude);
    TerrainTab(const TerrainTab& copy);

    ~TerrainTab();

    int height, width;
    float *grille = nullptr, **grille2d = nullptr;

    float amplitude;


    void getNormal(float pointX, float pointY) const;

private:

    float getHauteurXY(float x, float y) const;

    void initGrille();
    void simpleInitImage(const QImage& img);

    inline float get(int x, int y) const;
};

#endif // TERRAINTAB_H

#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

class Terrain : public Mesh
{
protected :
    float scale;
    float uniteDistance;
    int longueur, largeur;
    int resolution;

public:
    enum TYPE_TERRAIN_BASE {PLAT, SIMPLE, NOISE_3, CRETE, CRETE_NOISE, CRETE_NOISE_POINTE};
    Terrain();
    Terrain(TYPE_TERRAIN_BASE type, int _longueur, int _largeur, int _resolution, float _uniteDistance);
    Terrain(TYPE_TERRAIN_BASE type, int _longueur, int _largeur, int _resolution, float _uniteDistance, std::vector<ZoneTerrain> parametre);

    Eigen::Vector2d getDimension();

    float getHauteur(Eigen::Vector2f pointXY);
    float getHauteur(float pointX, float pointY);

    void rescale(float scale);

private :

    /***** fonction de génération de terrain ******/
    // longeur = longeur de la carte, largeur = largeur de la carte,
    // unite_Distance = correspondance en Km de longueur et largeur,
    // resolution = nb de point entre deux unité de distance de longueur et largeur

    void plan(int _longueur, int _largeur, float uniteDistance, int resolution, bool sauvegardeRecursive);
    void generationTerrainSimple(int width, int lenght, float uniteDistance, int resolution, bool sauvegardeRecursive);
    void generationTerrain3noise(int width, int lenght, float uniteDistance, int resolution, bool sauvegardeRecursive);
    void generationTerrainCrete(int width, int lenght, float uniteDistance, int resolution, bool sauvegardeRecursive);
    void generationTerrainCreteNoise(int width, int lenght, float uniteDistance, int resolution, bool sauvegardeRecursive);
    void generationTerrainCreteNoisePointe(int width, int lenght, float uniteDistance, int resolution, bool sauvegardeRecursive);
    void generationTerrainParametre(Terrain &terrainBase, std::vector<ZoneTerrain> parametre);

    float interpolation(float a, float b, float x);
};

#endif // TERRAIN_H

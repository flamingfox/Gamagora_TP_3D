#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"
#include <QImage>
#include "interpolation.h"
#include "rayon.h"
#include "box.h"

class Terrain : public Mesh
{
protected :
    //float uniteDistance;
    int longueur, largeur;
    int nbPointLongueur, nbPointLargeur;
    std::vector<Eigen::Vector3f> normalsPoints;
    Box englobant;

public:
    Terrain();
    Terrain(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur);
    //Terrain(int _longueur, int _largeur, int _resolution, std::vector<ZoneTerrain> parametre);

    Terrain(const QImage& img, float _longueur, float _largeur, float amplitude, int _nbHeight, int _nbWidth);
    Terrain(const QImage& img, float _longueur, float _largeur, float amplitude);

    Eigen::Vector2d getDimension() const;

    float getHauteur(Eigen::Vector2f pointXY) const;
    float getHauteur(float pointX, float pointY) const;

    void generationTerrain(int width, int lenght, int nbPointLongueur, int nbPointLargeur);

    bool inOut(Eigen::Vector3f pointXYZ);

    bool interesct(const Rayon& rayon, float coeffDistance) const;
    void calculNormals();

private :

    /***** fonction de génération de terrain ******/
    // longeur = longeur de la carte, largeur = largeur de la carte,
    // unite_Distance = correspondance en Km de longueur et largeur,
    // resolution = nb de point entre deux unité de distance de longueur et largeur

    void plan(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur);
    void applicationNoise(int amplitude, int periode);
    void applicationRidge(float seuil, float amplitude, int periode);
    void applicationWarp(int amplitude, int periode);
    //void generationTerrainParametre(Terrain &terrainBase, std::vector<ZoneTerrain> parametre);

    float maxElevation();

    float interpolation(float a, float b, float x);

    void simpleInitImage(const QImage& img, float _longueur, float _largeur, float _amplitude);
    void simpleInitTopo();
};

#endif // TERRAIN_H

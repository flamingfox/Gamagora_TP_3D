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
    ///
    /// \brief longueur distance sur axe y
    /// \brief largeur distance sur axe x
    ///
    int longueur, largeur;
    int type;

    ///
    /// \brief nbPointLongueur nombre de point sur axe y
    /// \brief nbPointLargeur nombre de point sur axe x
    ///
    int nbPointLongueur, nbPointLargeur;
    std::vector<Eigen::Vector3f> normalsPoints;
    Box englobant;

public:
    Terrain();
    Terrain(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur, int _type);
    //Terrain(int _longueur, int _largeur, int _resolution, std::vector<ZoneTerrain> parametre);

    Terrain(const QImage& img, float _longueur, float _largeur, float amplitude, int _nbHeight, int _nbWidth);
    Terrain(const QImage& img, float _longueur, float _largeur, float amplitude);

    Eigen::Vector2d getDimension() const;

    inline int gettype() const {return type;}

    float getHauteur(Eigen::Vector2f &pointXY) const;
    float getHauteur(float pointX, float pointY) const;

    Eigen::Vector3f getNormal(const Eigen::Vector3f &pointXYZ) const;
    Eigen::Vector3f getNormal(const Eigen::Vector2f &pointXY) const;
    Eigen::Vector3f getNormal(const float pointX, const float pointY) const;

    void generationTerrain(int width, int lenght, int nbPointLongueur, int nbPointLargeur);
    void generationTerrainSin(int width, int lenght, int nbPointLongueur, int nbPointLargeur);
    bool inOut(const Eigen::Vector3f& pointXYZ);

    bool intersect(const Rayon& rayon, float &coeffDistance) const;
    void calculNormals();
    float maxElevation()const;
    float maxelev;


    void initFinal();

private :

    /***** fonction de génération de terrain ******/
    // longeur = longeur de la carte, largeur = largeur de la carte,
    // unite_Distance = correspondance en Km de longueur et largeur,
    // resolution = nb de point entre deux unité de distance de longueur et largeur

    void plan(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur);
    void applicationNoise(int amplitude, int periode);
    void applicationRidge(float seuil, float amplitude, int periode);
    void applicationWarp(int amplitude, int periode);
    void applicationSin(int amplitude, int periode);
    //void generationTerrainParametre(Terrain &terrainBase, std::vector<ZoneTerrain> parametre);



    float interpolation(float a, float b, float x);

    void simpleInitImage(const QImage& img, float _longueur, float _largeur, float _amplitude);
    void simpleInitTopo();

    inline const Vector3f& getPoint(int i, int j) const;
    inline const Vector3f& getPoint(const Eigen::Vector2i& pos) const;
    inline const Vector3f& getN(int i, int j) const;
    inline const Vector3f& getN(const Eigen::Vector2i& pos) const;
};

#endif // TERRAIN_H

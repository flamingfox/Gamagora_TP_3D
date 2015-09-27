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

    ///
    /// \brief maxelevation
    ///
    float maxelevation, minelevation;

    ///
    /// \brief nbPointLongueur nombre de point sur axe y
    /// \brief nbPointLargeur nombre de point sur axe x
    ///
    int nbPointLongueur, nbPointLargeur;
    std::vector<Eigen::Vector3f> normalsPoints;
    Box englobant;

public:
    Terrain();
    Terrain(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur);

    inline Eigen::Vector2d getDimension() const{return Eigen::Vector2d( largeur, longueur);}
    inline float getElevationMax()const{return maxelevation;}
    inline float getElevationMin()const{return minelevation;}

    Mesh getMesh();// à compléter
    Eigen::Vector3f getNormal(float pointX, float pointY) const;

    bool intersectWithFunction(const Rayon& rayon, float &coeffDistance, int &i) const;
    float getHauteur(float x, float y)const;

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

    float interpolation(float a, float b, float x);

    inline const Vector3f& getPoint(int i, int j) const;
    inline const Vector3f& getPoint(const Eigen::Vector2i& pos) const;
    inline const Vector3f& getN(int i, int j) const;
    inline const Vector3f& getN(const Eigen::Vector2i& pos) const;

    float maxElevation()const;
    float minElevation()const;

    float noise(int amplitude, float periode, float x, float y)const;
    float ridge(const float& hauteur, const float& seuil, const float& amplitude, const float& periode, const float& x, const float& y)const;
    float attenuation(float h, float min, float max)const;
};

#endif // TERRAIN_H

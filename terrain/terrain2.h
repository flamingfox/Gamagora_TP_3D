#ifndef TERRAIN2_H
#define TERRAIN2_H

#include "./object.h"
#include "./rendu/colorgradient.h"
#include "./parametres.h"


class Terrain2: public Object
{
public:
    Terrain2();
    Terrain2(float longueur, float largeur);
    Terrain2(float longueur, float largeur, float amplitude);

    float longueur, largeur;

    float getVal(const Vector3f& p) const;

    float getHauteur(float x, float y) const;
    float getHauteur(const Vector2f& pointXY) const;
    float getHauteur(const Vector3f& pointXYZ) const;

    Eigen::Vector3f getNormal(float x, float y) const;
    Eigen::Vector3f getNormal(const Vector2f& pointXY) const;
    Eigen::Vector3f getNormal(const Vector3f& pointXYZ) const;

    bool inOut(const Eigen::Vector3f& pointXYZ) const;

    virtual float getHauteurXY(float x, float y) const = 0;


    //Eigen::Vector3f normale;
    ColorGradient heatMapGradient;    // Used to create a nice array of different colors.

protected:
    virtual Eigen::Vector3f getNormalXY(float x, float y) const;

    bool intersect2(const Rayon& rayon, float &coeffDistance) const;
    void translate2(const Vector3f& t);
    void getColor2(float& r, float& g, float& b, float hauteur = 0) const;

};

#endif // TERRAIN2_H

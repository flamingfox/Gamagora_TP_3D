#ifndef TERRAIN2_H
#define TERRAIN2_H

#include "./box.h"

class Terrain2
{
public:
    Terrain2();
    Terrain2(float longueur, float largeur);

    float longueur, largeur;
    Box box;

    float getHauteur(float x, float y) const;
    float getHauteur(const Vector2f& pointXY) const;
    float getHauteur(const Vector3f& pointXYZ) const;

    Eigen::Vector3f getNormal(float x, float y) const;
    void translate(const Eigen::Vector3f& t);

    bool inOut(const Eigen::Vector3f& pointXYZ) const;


    //Eigen::Vector3f normale;

protected:
    virtual float getHauteurXY(float x, float y) const = 0;
};

#endif // TERRAIN2_H

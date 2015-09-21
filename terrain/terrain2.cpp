#include "terrain2.h"

Terrain2::Terrain2():
    longueur(0),    largeur(0)
{
}

Terrain2::Terrain2(float longueur, float largeur):
    longueur(longueur), largeur(largeur)
{

}


void Terrain2::translate(const Eigen::Vector3f& t)
{
    box.min += t;
    box.max += t;
}

float Terrain2::getHauteur(float x, float y) const
{
    x -= box.min(0);
    x /= largeur;
    y -= box.min(1);
    y /= longueur;
    if(x < 0 || y < 0 || x > 1 || y > 1)
        return HAUTEUR_HORS_MAP;

    return getHauteurXY(x,y);
}


float Terrain2::getHauteur(const Vector2f& pointXY) const
{
    return getHauteur(pointXY(0), pointXY(1));
}

float Terrain2::getHauteur(const Vector3f& pointXYZ) const
{
    return getHauteur(pointXYZ(0), pointXYZ(1));
}



bool Terrain2::inOut(const Eigen::Vector3f& pointXYZ) const
{
    return (pointXYZ(2) <= getHauteur(pointXYZ(0), pointXYZ(1)));
}

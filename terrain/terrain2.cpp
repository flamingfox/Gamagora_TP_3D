#include "terrain2.h"

Terrain2::Terrain2():
    Terrain2(0,0)
{
}

Terrain2::Terrain2(float longueur, float largeur):
    Terrain2(longueur, largeur, 0)
{   
}


Terrain2::Terrain2(float longueur, float largeur, float amplitude):
    Object(longueur, largeur, amplitude),   longueur(longueur), largeur(largeur)
{
    heatMapGradient.createDefaultHeatMapGradient();
}


/**********************************************************/

/***/
float Terrain2::getHauteur(float x, float y) const
{
    x -= box.min(0);
    x /= largeur;
    y -= box.min(1);
    y /= longueur;
    //met les coordonnées entre 0 et 1.
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


/**********************************************************/

Vector3f Terrain2::getNormal(float x, float y) const
{
    x -= box.min(0);
    x /= largeur;
    y -= box.min(1);
    y /= longueur;
    //met les coordonnées entre 0 et 1.
    if(x < 0 || y < 0 || x > 1 || y > 1)
        return Vector3f(0,0,0);

    return getNormalXY(x,y);
}

Vector3f Terrain2::getNormal(const Vector2f& pointXY) const
{
    return getNormal(pointXY(0), pointXY(1));
}


Vector3f Terrain2::getNormal(const Vector3f& pointXYZ) const
{
    return getNormal(pointXYZ(0), pointXYZ(1));
}


/**********************************************************/


bool Terrain2::inOut(const Eigen::Vector3f& pointXYZ) const
{
    return (pointXYZ(2) <= getHauteur(pointXYZ(0), pointXYZ(1)));
}

bool Terrain2::intersect2(const Rayon& rayon, float &coeffDistance) const{

    float dmin = 0.0;
    float dmax = 3000.0;

    if(!box.intersect(rayon, dmin, dmax ))
        return false;

    dmin = 0.0;
    dmax = 3000.0;

    coeffDistance = dmin;

    for( int i=0; i<256; i++ )
    {
        Eigen::Vector3f pos = rayon.getOrigine() + coeffDistance*rayon.getDirection();
        float h = getHauteur( pos(0), pos(1) );
        if(h == HAUTEUR_HORS_MAP)
            break;

        h = pos(2) - h;

        if( h <(0.002 * coeffDistance) ) {
                return true;
        }else if(coeffDistance > dmax )
                break;

        coeffDistance += 0.5*h;
    }

    return false;
}

/************************************************************************/

void Terrain2::translate2(const Vector3f& t)
{
    (void) t;   //rien faire
}

void Terrain2::getColor2(float& r, float& g, float& b, float hauteur) const
{
    heatMapGradient.getColorAtValue(hauteur, r,g,b);
}


float Terrain2::getVal(const Vector3f& p) const
{
    return getHauteur(p(0), p(1));
}

/***********************************************************************/



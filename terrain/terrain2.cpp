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

bool Terrain2::intersect2(const Rayon& rayon, float &coeffDistance) const
{
    float dmin;
    float dmax;

    if(!box.intersect(rayon, dmin, dmax ))
        return false;

    //à modifier à cause de la précision des floats, mais c'est bizarre que ça marche comme un gant. faire attention que le point de départ ne soit pas en dehors de la box.
    //dmin = 0.0;
    //dmax = 3000.0;

    coeffDistance = dmin;

    for(int i = 0;  i<256;  i++)
    {
        Eigen::Vector3f pos = rayon.getOrigine() + coeffDistance*rayon.getDirection();
        float h = getHauteur( pos );
        if(h == HAUTEUR_HORS_MAP)
        {
            if(i == 0)  {
                coeffDistance += 0.01f;
                continue;
            }
            else
                break;
        }
        h = fabsf(pos(2) - h);

        if( h <(0.002 * coeffDistance) ) {
                return true;
        }else if(coeffDistance > dmax )
                break;

        coeffDistance += 0.25*h;    // des interfacts apparaissent à 0.5, le rayon dépasse le points qu'il devrait attendre
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


Eigen::Vector3f Terrain2::getNormalXY(float x, float y) const
{
    float h = getHauteurXY(x,y);
    float   rx = 1/largeur,
            ry = 1/longueur;
    float   xg = std::max(x-rx, 0.f),
            xd = std::min(x+rx, 1.f),
            yb = std::min(y+ry, 1.f),
            yh = std::max(y-ry, 0.f);
    float   g = getHauteurXY(xg,y),
            d = getHauteurXY(xd,y),
            b = getHauteurXY(x,yb),
            ha = getHauteurXY(x,yh);
    Eigen::Vector3f vg((xg-x)*largeur, 0, g-h),
                    vd((xd-x)*largeur, 0, d-h),
                    vb(0, (yb-y)*longueur, b-h),
                    vh(0, (yh-y)*longueur, ha-h);
    float   distg = vg.norm(),
            distd = vd.norm(),
            distb = vb.norm(),
            disth = vh.norm();
    Eigen::Vector3f v1 = vg.cross(vh),
                    v2 = vh.cross(vd),
                    v3 = vd.cross(vb),
                    v4 = vb.cross(vg);
    Eigen::Vector3f normale(0,0,0);
    if(distg*disth > 0)
        normale += v1.normalized()*distg*disth;
    if(disth*distd > 0)
        normale += v2.normalized()*disth*distd;
    if(distd*distb > 0)
        normale += v3.normalized()*distd*distb;
    if(distb*distg > 0)
        normale += v4.normalized()*distb*distg;
    normale.normalize();
    return normale;
}

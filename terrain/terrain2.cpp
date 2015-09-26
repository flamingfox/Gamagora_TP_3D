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

void Terrain2::getColor2(float& r, float& g, float& b, float hauteur, const Eigen::Vector3f& n) const
{
    (void) n;
    heatMapGradient.getColorAtValue(hauteur, r,g,b);
}

void Terrain2::getColor3(float& r, float& g, float& b, float x, float y) const
{
    float hauteur = getHauteur(x,y);
    hauteur /= box.diffZ();
    Vector3f normale = getNormal(x,y);
    float pente = 1-normale.dot(Vector3f(0,0,1));

    float roche = 3 + NoiseGenerator::perlinNoiseGradiant2(x, y, 1000) + NoiseGenerator::perlinNoiseGradiant2(x+30, y-60, 100) + NoiseGenerator::perlinNoiseGradiant2(x,y, 46);
    roche *= pente;
    roche *= 3.f;

    float herbe = 3 + NoiseGenerator::perlinNoiseGradiant2(x+100, y-100, 1234) + NoiseGenerator::perlinNoiseGradiant2(x-100,y-50, 123) + NoiseGenerator::perlinNoiseGradiant2(x-1, y-30,39);
    herbe *= 1-hauteur;

    float neige = 3 + NoiseGenerator::perlinNoiseGradiant2(x+555, y-10, 1324) + NoiseGenerator::perlinNoiseGradiant2(x-200,y-54, 223) + NoiseGenerator::perlinNoiseGradiant2(x-10, y+12,91);
    neige *= hauteur;

    float   sr = roche*1/0.39f+neige,
            sg = roche*1/0.36f+herbe+neige,
            sb = roche*1/0.29f+neige;
/*
    r = (roche*0.39f+neige)/sr;
    g = (roche*0.36f+herbe+neige)/sg;
    b = (roche*0.29f+neige)/sb;
*/
    if(herbe > roche && herbe > neige)
    {
        if(herbe > 1)
            herbe = 1;
        r = 0;
        g = herbe;
        b = 0;

    }
    else if(roche > herbe && roche > neige)
    {
        if(roche > 1)
            roche = 1;
        r = roche*0.36f;
        g = roche*0.24f;
        b = roche*0.07f;
    }
    else
    {
        if(neige > 1)
            neige = 1;
        r = neige;
        g = neige;
        b = neige;
    }
}


float Terrain2::getVal(const Vector3f& p) const
{
    return getHauteur(p(0), p(1));
}

/***********************************************************************/



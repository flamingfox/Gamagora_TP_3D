#include "terrain.h"

Terrain::Terrain():
    Terrain(0,0)
{
}

Terrain::Terrain(float longueur, float largeur):
    Terrain(longueur, largeur, 0)
{   
}

Terrain::Terrain(float longueur, float largeur, float amplitude):
    longueur(longueur), largeur(largeur)
{
    box = Box(Vector3f(0,0,0),Vector3f(largeur,longueur,amplitude));
    heatMapGradient.createDefaultHeatMapGradient();
}

/**********************************************************/

/***/
float Terrain::getHauteur(float x, float y) const
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

float Terrain::getHauteur(const Vector2f& pointXY) const
{
    return getHauteur(pointXY(0), pointXY(1));
}

float Terrain::getHauteur(const Vector3f& pointXYZ) const
{
    return getHauteur(pointXYZ(0), pointXYZ(1));
}


/**********************************************************/

Vector3f Terrain::getNormal(float x, float y) const
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

Vector3f Terrain::getNormal(const Vector2f& pointXY) const
{
    return getNormal(pointXY(0), pointXY(1));
}


Vector3f Terrain::getNormal(const Vector3f& pointXYZ) const
{
    return getNormal(pointXYZ(0), pointXYZ(1));
}


/**********************************************************/


bool Terrain::inOut(const Eigen::Vector3f& pointXYZ) const
{
    return (pointXYZ(2) <= getHauteur(pointXYZ(0), pointXYZ(1)));
}

bool Terrain::intersect(const Rayon& rayon, float &coeffDistance, int &i) const
{
    float dmin;
    float dmax;

    if(!box.intersect(rayon, dmin, dmax ))
        return false;

    //à modifier à cause de la précision des floats, mais c'est bizarre que ça marche comme un gant. faire attention que le point de départ ne soit pas en dehors de la box.
    //dmin = 0.0;
    //dmax = 3000.0;

    coeffDistance = dmin;

    for(i = 0;  i<256;  i++)
    {
        Eigen::Vector3f pos = rayon.getOrigine() + coeffDistance*rayon.getDirection();
        float h = getHauteur( pos );
        if(h == HAUTEUR_HORS_MAP)        {
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

        coeffDistance += 0.3*h;

    }

    return false;
}

/************************************************************************/

void Terrain::translate2(const Vector3f& t)
{
    (void) t;   //rien faire
}


/*void Terrain::getColor2(float& r, float& g, float& b, float hauteur, const Eigen::Vector3f& n) const
{
    (void) n;
    heatMapGradient.getColorAtValue(hauteur, r,g,b);
}*/


void Terrain::getColor(float& r, float& g, float& b, float x, float y) const
{
    float hauteur = getHauteur(x,y);
    hauteur /= box.diffZ();
    Vector3f normale = getNormal(x,y);
    float pente = 1-normale.dot(Vector3f(0,0,1));

    float roche = 3 + NoiseGenerator::perlinNoiseGradiant2(x, y, 1000) + NoiseGenerator::perlinNoiseGradiant2(x+30, y-60, 103) + NoiseGenerator::perlinNoiseGradiant2(x,y, 3);
    roche *= 0.25f+pente*0.75f;
    roche *= 0.3f;

    float herbe = 3 + NoiseGenerator::perlinNoiseGradiant2(x+10100, y+10100, 1234) + NoiseGenerator::perlinNoiseGradiant2(x-100,y-50, 123) + NoiseGenerator::perlinNoiseGradiant2(x-1, y-30,2);
    herbe *= 0.25f+(1-hauteur)*(1-hauteur)*0.75f;
    herbe *= 0.25f;

    float neige = 3 + NoiseGenerator::perlinNoiseGradiant2(x+555, y+1010, 1324) + NoiseGenerator::perlinNoiseGradiant2(x-200,y-54, 223) + NoiseGenerator::perlinNoiseGradiant2(x-10, y+12,0.5);
    neige *= 0.1f+hauteur*hauteur*0.9f;
    neige *= 0.25f;

    /*float   sr = roche*1/0.39f+neige,
            sg = roche*1/0.36f+herbe+neige,
            sb = roche*1/0.29f+neige;*/
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

/***********************************************************************/



/*bool Mesh::intersectWithMesh(const Rayon& rayon, float &coeffDistance) const{

    float dmin = 0.0;
    float dmax = 3000.0;

    if(!englobant.intersect(rayon, dmin, dmax ))
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
}*/


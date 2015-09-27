#include "terrainnoise.h"

#define HAUTEUR_MAX_NOISE 300

TerrainNoise::TerrainNoise() : Terrain()
{}

TerrainNoise::TerrainNoise(int _longueur, int _largeur) :
    Terrain(_longueur, _largeur)
{
    box = Box(Vector3f(0,0,0), Vector3f(_largeur,_longueur, 0));
    hauteurMin = minElevation();
    hauteurMax = maxElevation();
    box.max(2) = hauteurMax;

}





/***************************************************************/






/***************************************************************/

/**
 * @param x abscisse du terrain (entre 0 et 1)
 * @param y ordonnée du terrain (entre 0 et 1)
 * @return la hauteur du terrain à ses coordonnées x, y
*/
float TerrainNoise::getHauteurXY(float x, float y) const
{
    if(x < 0 || y < 0 || x > largeur || y > longueur)
            return HAUTEUR_HORS_MAP;

    float h = nrw::noise(400,300,x*largeur,y*longueur);
    h = nrw::ridge(h, 250);

    float h2 = nrw::noise(100, 100, x*largeur, y*longueur);
    h -= h2*nrw::attenuation(h,50,200);

    float h3 = nrw::noise(50,30,x*largeur,y*longueur);
    h += h3*nrw::attenuation(h,50,200);

    return h;
}

/**
 * utile pour le calcul de normal sur terrain avec noise
 * @param x abscisse du terrain (entre 0 et 1)
 * @param y ordonnée du terrain (entre 0 et 1)
 * @return la hauteur du terrain à ses coordonnées x, y
*/


Eigen::Vector3f TerrainNoise::getNormalXY(float x, float y) const
{
    float   ha = getHauteurXY(x,y);
    float   rx = 1/largeur,
            ry = 1/longueur;
    float   g = getHauteurXY(x-rx,y),
            d = getHauteurXY(x+rx,y),
            b = getHauteurXY(x,y+ry),
            h = getHauteurXY(x,y-ry);
    Eigen::Vector3f vg(-1, 0, g-ha),
                    vd(1, 0, d-ha),
                    vb(0, 1, b-ha),
                    vh(0, -1, h-ha);
    float   distg = vg.norm(),
            distd = vd.norm(),
            distb = vb.norm(),
            disth = vh.norm();
    Eigen::Vector3f v1 = vg.cross(vh),
                    v2 = vh.cross(vd),
                    v3 = vd.cross(vb),
                    v4 = vb.cross(vg);
    v1.normalize();
    v2.normalize();
    v3.normalize();
    v4.normalize();
    Eigen::Vector3f normale = v1*distg*disth + v2*disth*distd + v3*distd*distb + v4*distb*distg;
    return normale.normalized();
}




float TerrainNoise::minElevation() const{
    float hMin = FLT_MAX;
    float hauteur;
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 0 ; i<(int)largeur; i++){
        for(int y = 0; y<longueur; y++){
            hauteur = getHauteur(i,y);
            if(hauteur<hMin)hMin=hauteur;
        }
    }
    return hMin;
}

float TerrainNoise::maxElevation() const{
    float hMax = FLT_MIN;
    float hauteur;
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 0 ; i<(int)largeur; i++){
        for(int y = 0; y<longueur; y++){
            hauteur = getHauteur(i,y);
            if(hauteur>hMax)hMax=hauteur;
        }
    }
    return hMax;
}


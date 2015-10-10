#include "terrainnoise.h"

TerrainNoise::TerrainNoise() : Terrain()
{}

TerrainNoise::TerrainNoise(int _longueur, int _largeur) :
    Terrain(_longueur, _largeur)
{
    hauteurMin = minElevation();
    hauteurMax = maxElevation();
    box.max(2) = hauteurMax;
}


/**
 * @param x abscisse du terrain (entre 0 et largeur)
 * @param y ordonnée du terrain (entre 0 et longueur)
 * @return la hauteur du terrain à ses coordonnées x, y
*/
float TerrainNoise::getHauteur(float x, float y) const
{
    if(x < 0 || y < 0 || x > largeur || y > longueur)
            return HAUTEUR_HORS_MAP;

    return getHauteur2(x,y);    //inline
}

/**
 * @brief Retourne la hauteur du terrain aux coordonnées (x,y). \n
 * Ne vérifie pas que l'on est en dehors du terrain. \n
 * Utile pour avoir une bonne normal sur les bords du terrain.
 * @param x abscisse du terrain (entre 0 et largeur)
 * @param y ordonnée du terrain (entre 0 et longueur)
 * @return la hauteur du terrain à ses coordonnées x, y
 */
inline float TerrainNoise::getHauteur2(float x, float y) const
{
    nrw::warp(x,y, 50 ,350);
    float h = nrw::noise(400,300,x,y);

    h = nrw::ridge(h, 250);

    float h2 = nrw::noise(100, 100, x, y);

    h -= h2*nrw::attenuation(h,50,200);

    float h3 = nrw::noise(50,30,x,y);
    h += h3*nrw::attenuation(h,50,200);

    return h;
}

Eigen::Vector3f TerrainNoise::getNormal(float x, float y) const
{

    float   ha = getHauteur2(x,y);
    float   g = getHauteur2(x-RAYON_NORMAL,y),
            d = getHauteur2(x+RAYON_NORMAL,y),
            b = getHauteur2(x,y+RAYON_NORMAL),
            h = getHauteur2(x,y-RAYON_NORMAL);
    Eigen::Vector3f vg(-RAYON_NORMAL, 0, g-ha),
                    vd(RAYON_NORMAL, 0, d-ha),
                    vb(0, RAYON_NORMAL, b-ha),
                    vh(0, -RAYON_NORMAL, h-ha);
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


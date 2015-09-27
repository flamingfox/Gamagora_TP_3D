#include "terrainnoise.h"

#define HAUTEUR_MAX_NOISE 300

TerrainNoise::TerrainNoise() : Terrain2()
{}

TerrainNoise::TerrainNoise(int _longueur, int _largeur) :
    Terrain2(_longueur, _largeur)
{
    box = Box(Vector3f(0,0,0), Vector3f(_largeur,_longueur, HAUTEUR_MAX_NOISE));
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
    if(x < 0    || y < 0 || x > 1 || y > 1)
        return HAUTEUR_HORS_MAP;
    return getHauteurXYSansVerif(x,y);
}


/**
 * utile pour le calcul de normal sur terrain avec noise
 * @param x abscisse du terrain (entre 0 et 1)
 * @param y ordonnée du terrain (entre 0 et 1)
 * @return la hauteur du terrain à ses coordonnées x, y
*/
float TerrainNoise::getHauteurXYSansVerif(float x, float y) const
{
    float h = nrw::noise(400,300,x*largeur,y*longueur);
    h = nrw::ridge(h, 250);

    float h2 = nrw::noise(100, 100, x*largeur, y*longueur);
    h -= h2*(h/250);

    float h3 = nrw::noise(50,30,x*largeur,y*longueur);
    float div = h/250;
    if(div> 1)
        std::cout << "bizarre?" << std::endl;
    h += h3*div*div;

    return h;
}


Eigen::Vector3f TerrainNoise::getNormalXY(float x, float y) const
{
    float   ha = getHauteurXYSansVerif(x,y);
    float   rx = 1/largeur,
            ry = 1/longueur;
    float   g = getHauteurXYSansVerif(x-rx,y),
            d = getHauteurXYSansVerif(x+rx,y),
            b = getHauteurXYSansVerif(x,y+ry),
            h = getHauteurXYSansVerif(x,y-ry);
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




float TerrainNoise::getMinElevation2() const{
    return 0;
}
float TerrainNoise::getMaxElevation2() const{
    return 300;
}



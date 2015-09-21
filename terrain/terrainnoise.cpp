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




float TerrainNoise::noise(int amplitude, float periode, float x, float y)const{
    float h = NoiseGenerator::perlinNoise( x/periode, y/periode);
    h = (h+1)/2;
    return amplitude * h;
}

float TerrainNoise::ridge(float hauteur, float seuil)const{
    if(hauteur > seuil)return (2*seuil - hauteur);
    else return hauteur;
}


/***************************************************************/

float TerrainNoise::getHauteurXY(float x, float y) const
{
    if(x < 0    || y < 0 || x > 1 || y > 1)
        return HAUTEUR_HORS_MAP;
    float h = noise(200,0.1,x,y);
    float h2 = ridge(h, 160);

    float h3 = h2 + noise(20,0.01,x,y);
    //float h4 = ridge(h3, 160);

    return h3;
}

Eigen::Vector3f TerrainNoise::getNormalXY(float x, float y) const
{
    float h = getHauteurXY(x,y);
    float   xg = std::max(x-0.01f, 0.f),
            xd = std::min(x+0.01f, 1.f),
            yb = std::min(y+0.01f, 1.f),
            yh = std::max(y-0.01f, 0.f);
    float   g = getHauteurXY(xg,y),
            d = getHauteurXY(xd,y),
            b = getHauteurXY(x,yb),
            ha = getHauteurXY(x,yh);
    Eigen::Vector3f vg(xg-x, 0, h-g),
                    vd(xd-x, 0, h-d),
                    vb(0,yb-y,h-b),
                    vh(0,yh-y, h-ha);
    vg.normalize();
    vd.normalize();
    vb.normalize();
    vh.normalize();
    Eigen::Vector3f v1 = vg.cross(vh),
                    v2 = vh.cross(vd),
                    v3 = vd.cross(vb),
                    v4 = vb.cross(vg);
    v1.normalize();
    v2.normalize();
    v3.normalize();
    v4.normalize();
    Eigen::Vector3f normale = v1 + v2 + v3 + v4;
    normale.normalize();
    return normale;
}

/*
float Terrain::warp()
*/

/*void TerrainNoise::applicationWarp(int amplitude, int periode)
{
    //for(size_t i = 0; i < geom.size(); i++)
    for(Eigen::Vector3f& p: geom)
    {
        float warp = amplitude * NoiseGenerator::perlinNoise( p(0)*(1.0/periode)+2.78, p(1)*(1.0/periode)+8.72);

        p(0) += warp;
        p(1) += warp;
    }
}
*/




#include "terrainnoise.h"


TerrainNoise::TerrainNoise() : Terrain2()
{}

TerrainNoise::TerrainNoise(int _longueur, int _largeur) :
    Terrain2(_longueur, _largeur)
{
    box = Box(Vector3f(0,0,0), Vector3f(_largeur,_longueur, 300));
}








/* -------------------------------------------- */
/* -------------------------------------------- */
/*            générateur de terrain             */
/* -------------------------------------------- */
/* -------------------------------------------- */



bool TerrainNoise::intersect(const Rayon& rayon, float &coeffDistance) const{

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





float TerrainNoise::noise(int amplitude, float periode, float x, float y)const{
    float h = NoiseGenerator::perlinNoise( x/periode, y/periode);
    h = (h+1)/2;
    return amplitude * h;
}

float TerrainNoise::ridge(float hauteur, float seuil)const{
    if(hauteur > seuil)return (2*seuil - hauteur);
    else return hauteur;
}

float TerrainNoise::getHauteurXY(float x, float y) const
{
    if(x < 0    || y < 0 || x > 1 || y > 1)
        return HAUTEUR_HORS_MAP;
    float h = noise(200,300,x,y);
    float h2 = ridge(h, 160);

    float h3 = h2 + noise(20,40,x,y);
    //float h4 = ridge(h3, 160);

    return h3;
}

Eigen::Vector3f TerrainNoise::getNormalXY(float x, float y) const
{
    float h = getHauteurXY(x,y);
    float   g = getHauteurXY(x-0.01,y),
            d = getHauteurXY(x+0.01,y),
            b = getHauteurXY(x,y+0.01),
            ha = getHauteurXY(x,y-0.01);
    Eigen::Vector3f vg(-0.01, 0, h-g),
                    vd(0.01, 0, h-d),
                    vb(0,0.01,h-b),
                    vh(0,-0.01, h-ha);
    Eigen::Vector3f v1 = vg.cross(vb),
                    v2 = vb.cross(vd),
                    v3 = vd.cross(vh),
                    v4 = vh.cross(vg);
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




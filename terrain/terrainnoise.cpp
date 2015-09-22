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
    float h = noise(250,300,x*largeur,y*longueur);
    float h2 = ridge(h, 180);

    float h3 = h2 + noise(20,40,x*largeur,y*longueur);
    //float h4 = ridge(h3, 160);

    return h3;
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




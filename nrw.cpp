#include "nrw.h"

namespace nrw
{
    float noise(int amplitude, float periode, float x, float y)
    {
        float h = NoiseGenerator::perlinNoise( x/periode, y/periode);
        h = (h+1)/2;
        return amplitude * h;
    }

    float ridge(float hauteur, float seuil)
    {
        if(hauteur > seuil)return (2*seuil - hauteur);
        else return hauteur;
    }


    void warp(float& x, float& y, float amplitude, float periode)
    {
        //float w = amplitude * NoiseGenerator::perlinNoise( x*(1.0/periode)+2.78, y*(1.0/periode)+8.72);
        float w = amplitude * NoiseGenerator::perlinNoise( x*(1.0/periode), y*(1.0/periode));
        x += w;
        y += w;
    }

    void warp(float x, float y, float& x2, float& y2, float amplitude, float periode)
    {
        float w = amplitude * NoiseGenerator::perlinNoise( x*(1.0/periode), y*(1.0/periode));
        x2 = x+w;
        y2 = y+w;
    }

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

    float attenuation(float h, float min, float max)
    {
        if(h<min)return 0;
        if(h>max)return 1;
        double t = (h-min)/(max-min);
        return (1-((1-(t*t))*(1-(t*t))));
    }
}

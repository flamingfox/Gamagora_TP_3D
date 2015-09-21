#include "noisegenerator.h"
#include "parametres.h"

NoiseGenerator::NoiseGenerator()
{}

double NoiseGenerator::perlinNoise(double x,double y, int seed)
{
    double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
    double floory=(double)((int)y);
    double s,t,u,v;//Integer declaration
    s = findnoise2(floorx,floory, seed);
    t = findnoise2(floorx+1,floory, seed);
    u = findnoise2(floorx,floory+1, seed); //Get the surrounding pixels to calculate the transition.
    v = findnoise2(floorx+1,floory+1, seed);
    double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
    double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
    return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

double NoiseGenerator::perlinNoise(double x, double y)
{
    double tmp = perlinNoise(x, y, SEED);
    return tmp;
    //return (tmp+1)/2;
}

double NoiseGenerator::findnoise2(double x, double y, int seed)
{
    int n=(int)x+(int)y*57;
    n=(n<<13)^n;

    int nn= (n * (n * n + 789221) + 1376312589 + seed )&0x7fffffff;
    return 1.0-((double)nn/ 1073741824.0 );
}

double NoiseGenerator::interpolate(double a, double b, double x)
{
    double ft=x * 3.1415927;
    double f=(1.0-cos(ft))* 0.5;
    return a*(1.0-f)+b*f;
}

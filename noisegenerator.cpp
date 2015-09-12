#include "noisegenerator.h"

NoiseGenerator::NoiseGenerator()
{}

double NoiseGenerator::perlinNoise(double x,double y, float harmonique)
{
    double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
    double floory=(double)((int)y);
    double s,t,u,v;//Integer declaration
    s = findnoise2(floorx,floory, harmonique);
    t = findnoise2(floorx+1,floory, harmonique);
    u = findnoise2(floorx,floory+1, harmonique); //Get the surrounding pixels to calculate the transition.
    v = findnoise2(floorx+1,floory+1, harmonique);
    double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
    double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
    return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

double NoiseGenerator::perlinNoise(double x, double y)
{
    return perlinNoise(x, y, 1);
}

double NoiseGenerator::findnoise2(double x, double y, float harmonique)
{
    int n=(int)x+(int)y*(57/harmonique);
    n=(n<<13)^n;
    int nn=(n*(n*n*(60493+19990303)+1376312589)&0x7fffffff) / harmonique;
    return 1.0-((double)nn/ (1073741824.0 / harmonique) );
}

double NoiseGenerator::interpolate(double a, double b, double x)
{
    double ft=x * 3.1415927;
    double f=(1.0-cos(ft))* 0.5;
    return a*(1.0-f)+b*f;
}

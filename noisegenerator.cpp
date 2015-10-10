#include "noisegenerator.h"

namespace NoiseGenerator
{

/**
 * @brief Calcul du value noise à la position x, y.
 * @param[in] x Position en x.
 * @param[in] y Position en y.
 * @param[in] seed
 * @return Le value noise à la position x, y.
 */
double findnoise(double x, double y, int seed)
{
    int n=(int)x+(int)y*57;
    n=(n<<13)^n;

    int nn= (n * (n * n + 789221) + 1376312589 + seed )&0x7fffffff;
    return 1.0-((double)nn/ 1073741824.0 );
}

/**
 * @brief Interpole entre la valeur a et b selon x.
 * @param[in] a Premier extermité de l'interpolation.
 * @param[in] b Second extermité de l'interpolation.
 * @param[in] x Valeur d'interpolation. Valeur entre 0 et 1.
 * @return L'interpolation entre a et b.
 */
double interpolate(double a, double b, double x)
{
    double ft=x * 3.1415927;
    double f=(1.0-cos(ft))* 0.5;
    return a*(1.0-f)+b*f;
}


double perlinNoise(double x,double y, int seed)
{
    double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
    double floory=(double)((int)y);
    double s,t,u,v;//Integer declaration
    s = findnoise(floorx,floory, seed);
    t = findnoise(floorx+1,floory, seed);
    u = findnoise(floorx,floory+1, seed); //Get the surrounding pixels to calculate the transition.
    v = findnoise(floorx+1,floory+1, seed);
    double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
    double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
    return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

double perlinNoise(double x, double y)
{
    double value = perlinNoise(x, y, SEED);
    return value;
}

/************************************************************************************************************/


#define unit 1.0/sqrt(2)
const double gradient[8][2] = {{unit,unit},{-unit,unit},{unit,-unit},{-unit,-unit},{1,0},{-1,0},{0,1},{0,-1}};

/**tableau de tous les nombres de 0 à 256-1 placé aléatoirement mais en 1 seul exemplaire
en double pour les besoins de l'algorithme*/
const int perm[] = { 151,160,137,91,90,15,                 // Hash lookup table as defined by Ken Perlin.  This is a randomly
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,    // arranged array of all numbers from 0-255 inclusive.
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
                    151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};


double perlinNoiseGradiant(double x, double y, double res)
{
    return (1+perlinNoiseGradiant2(x,y,res))*0.5;
}


double perlinNoiseGradiant2(double x, double y, double res)
{
    double tempX,tempY;
    unsigned int ii,jj,gi0,gi1,gi2,gi3;
    int x0, y0;
    double tmp,s,t,u,v,Cx,Cy,Li1,Li2;

    //Adapter pour la résolution
    double resDiv = 1.0/res;
    x *= resDiv;
    y *= resDiv;

    //On récupère les positions de la grille associée à (x,y)
    x0 = floor(x);
    y0 = floor(y);

    //Masquage
    ii = x0 & 255;  //le masquage par & est beaucoup plus rapide qu'avec un modulo
    jj = y0 & 255;

    //Pour récupérer les vecteurs
    gi0 = perm[ii + perm[jj]] & 7;
    gi1 = perm[ii + 1 + perm[jj]] & 7;
    gi2 = perm[ii + perm[jj + 1]] & 7;
    gi3 = perm[ii + 1 + perm[jj + 1]] & 7;

    //on récupère les vecteurs et on pondère
    tempX = x-x0;
    tempY = y-y0;
    s = gradient[gi0][0]*tempX + gradient[gi0][1]*tempY;

    tempX = x-(x0+1);
    //tempY = y-y0;
    t = gradient[gi1][0]*tempX + gradient[gi1][1]*tempY;

    tempX = x-x0;
    tempY = y-(y0+1);
    u = gradient[gi2][0]*tempX + gradient[gi2][1]*tempY;

    tempX = x-(x0+1);
    //tempY = y-(y0+1);
    v = gradient[gi3][0]*tempX + gradient[gi3][1]*tempY;


    //Lissage
    tmp = x-x0;
    Cx = tmp*tmp*(3 - 2 * tmp);  //3t^2-2t^3

    Li1 = s + Cx*(t-s);
    Li2 = u + Cx*(v-u);

    tmp = y-y0;
    Cy = tmp*tmp*(3 - 2 * tmp);

    return Li1 + Cy*(Li2-Li1);
}


double random(double max)
{
    double val = (rand()%10000)/10000.0;
    return (val*max);
}

double random2(double max)
{
    return random(max*2)-max;
}

}   //end NoiseGenerator

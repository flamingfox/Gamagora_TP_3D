#include "terrain.h"

Terrain::Terrain() : longueur(0), largeur(0), nbPointLongueur(0), nbPointLargeur(0)
{}

Terrain::Terrain(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur) : longueur(_longueur),
    largeur(_largeur), nbPointLongueur(_nbPointLongueur), nbPointLargeur(_nbPointLargeur)
{
    maxelevation = maxElevation();
    minelevation = minElevation();
    englobant = Box(Vector3f(0,0,minelevation),Vector3f(longueur,largeur,maxelevation));
}

///
/// \brief Terrain::getMesh
/// \return return the Mesh
///
Mesh Terrain::getMesh(){
    //plan(_longueur,_largeur,_nbPointLongueur,_nbPointLargeur);
    //generationTerrain(largeur, longueur, nbPointLongueur, nbPointLargeur);
}

///
/// \brief Terrain::getNormal
/// \param pointX
/// \param pointY
/// \return return the normal of the terrain in x,y
///
Vector3f Terrain::getNormal(float pointX, float pointY) const
{
    Vector3f p1(pointX-1,pointY-1,getHauteur(pointX-1,pointY-1));
    Vector3f p2(pointX+1,pointY-1,getHauteur(pointX+1,pointY-1));
    Vector3f p3(pointX-1,pointY+1,getHauteur(pointX-1,pointY+1));

    Vector3f a = p3-p2;
    Vector3f b = p1-p2;
    Vector3f normal = a.cross(b);
    return normal.normalized();
}

///
/// \brief Terrain::intersectWithFunction
/// \param rayon
/// \param coeffDistance return the result of distance in this variable
/// \param i return the number of iterations
/// \return find the intersection distance of a ray (given in parameters) with the terrain
///
bool Terrain::intersectWithFunction(const Rayon& rayon, float &coeffDistance, int& i) const{

    float dmin = 0.0;
    float dmax = 3000.0;

    coeffDistance = dmin;
    for( i=0; i<256; i++ )
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

///
/// \brief Terrain::noise
/// \param amplitude
/// \param periode
/// \param x
/// \param y
/// \return return a random controlled value from a perlin noise
///
float Terrain::noise(int amplitude, float periode, float x, float y)const{

    float h = NoiseGenerator::perlinNoise( x/periode, y/periode);
    h = (h+1)/2;
    return amplitude * h;
}

///
/// \brief Terrain::ridge
/// \param hauteur
/// \param seuil
/// \param amplitude
/// \param periode
/// \param x
/// \param y
/// \return
///
float Terrain::ridge(const float& hauteur, const float& seuil, const float& amplitude, const float& periode, const float& x, const float& y) const{

    float hRidge = amplitude * NoiseGenerator::perlinNoise(x/periode, y/periode);
    hRidge += seuil;

    if(hauteur > hRidge)
        return  (2*hRidge - hauteur);

    return hauteur;
}

///
/// \brief Terrain::getHauteur
/// \param x
/// \param y
/// \return return the elevation of the terrain in x,y
///
float Terrain::getHauteur(float x, float y)const{

    if(x < 0 || y < 0 || x > largeur || y > longueur)
        return HAUTEUR_HORS_MAP;

    float h = noise(200, 300, x, y);
    h = ridge(h, 150, 20, 100, x, y);

    h = h + noise(20, 100, x, y)*attenuation(h, 100, 130);
    return h;

}

///
/// \brief Terrain::attenuation
/// \param h valeur que l'on souhaite atténuer
/// \param min seuil minimum à partir duquel la fonctin renvoie un résulttat > à 0
/// \param max seuil à partir duquel la fonction renvoie toujours 1
/// \return return a factor between 0 and 1 depending on the elevation
///
float Terrain::attenuation(float h, float min, float max)const{
    if(h<min)return 0;
    if(h>max)return 1;
    double t = (h-min)/(max-min);
    return (1-((1-(t*t))*(1-(t*t))));
}

/*
float Terrain::warp()
*/

///
/// \brief Terrain::maxElevation
/// \return return the highest elevation of the terrain
///
float Terrain::maxElevation()const
{
    float hMax = FLT_MIN;
    float hauteur;
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 0 ; i<longueur; i++){
        for(int y = 0; y<largeur; y++){
            hauteur = getHauteur(i,y);
            if(hauteur>hMax)hMax=hauteur;
        }
    }
    return hMax;
}

///
/// \brief Terrain::minElevation
/// \return return the lowest elevation of the terrain
///
float Terrain::minElevation()const
{
    float hMin = FLT_MAX;
    float hauteur;
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 0 ; i<longueur; i++){
        for(int y = 0; y<largeur; y++){
            hauteur = getHauteur(i,y);
            if(hauteur<hMin)hMin=hauteur;
        }
    }
    return hMin;
}

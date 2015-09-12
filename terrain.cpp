#include "terrain.h"

Terrain::Terrain() : scale(1), longueur(0), largeur(0), resolution(0), uniteDistance(0)
{}

Terrain::Terrain(TYPE_TERRAIN_BASE type, int _longueur, int _largeur, int _resolution, float _uniteDistance) : longueur(_longueur),
    largeur(_largeur), resolution(_resolution), scale(1), uniteDistance(_uniteDistance)
{
    switch (type) {
    case PLAT :
        plan(_longueur, _largeur, _uniteDistance, _resolution, false);
        break;
    case SIMPLE :
        generationTerrainSimple(_longueur, _largeur, _uniteDistance, _resolution, false);
        break;

    case NOISE_3 :
        generationTerrain3noise(_longueur, _largeur, _uniteDistance, _resolution, false);
        break;

    case CRETE :
        generationTerrainCrete(_longueur, _largeur, _uniteDistance, _resolution, false);
        break;
    case CRETE_NOISE :
        generationTerrainCreteNoise(_longueur, _largeur, _uniteDistance, _resolution, false);
        break;
    case CRETE_NOISE_POINTE :
        generationTerrainCreteNoisePointe(_longueur, _largeur, _uniteDistance, _resolution, false);
        break;
    }
}

Terrain::Terrain(Terrain::TYPE_TERRAIN_BASE type, int _longueur, int _largeur, int _resolution,
                 float _uniteDistance, std::vector<ZoneTerrain> parametre) :
    Terrain(type, _longueur, _largeur, _resolution, _uniteDistance)
{
    generationTerrainParametre(*this, parametre);
}

Eigen::Vector2d Terrain::getDimension()
{
    //return Eigen::Vector2d(longueur*resolution*uniteDistance, largeur*resolution*uniteDistance);
    return Eigen::Vector2d( ( longueur-(1.0/resolution) )*uniteDistance*scale,
                            ( largeur-(1.0/resolution) )*uniteDistance*scale);
}

float Terrain::getHauteur(Eigen::Vector2f pointXY)
{
    return getHauteur(pointXY(0), pointXY(1));
}

float Terrain::getHauteur(float pointX, float pointY)
{
    Eigen::Vector2d dim = getDimension();
    long nbPointX = largeur*resolution;
    long nbPointY = largeur*resolution;

    int indiceX = (pointX) * ( nbPointX/dim(0) );
    int indiceY = pointY * ( nbPointY/dim(1) );
    indiceX *= nbPointX;

    Eigen::Vector3f point = geom.at( indiceX + indiceY);

    return point(2);
}

void Terrain::rescale(float scale)
{
    float tmp = scale / this->scale;
    Mesh::rescale(tmp);
    this->scale = scale;

}


/* -------------------------------------------- */
/* -------------------------------------------- */
/*            générateur de terrain             */
/* -------------------------------------------- */
/* -------------------------------------------- */

void Terrain::plan(int _longueur, int _largeur, float _uniteDistance, int _resolution, bool sauvegardeRecursive)
{
    std::vector<Eigen::Vector3f> g;
    std::vector<int> t;
    for(float i = 0; i < _longueur; i+= (1.0/_resolution) )
    {
        for(float j = 0; j < _largeur; j+= (1.0/_resolution) )
        {
            g.push_back(Eigen::Vector3f(i*_uniteDistance, j*_uniteDistance, 0));
        }
    }

    long nbPoint_longueur = _longueur*_resolution;
    long nbPoint_largeur = _largeur*_resolution;

    for(long i = 0; i < nbPoint_longueur-1; i++){
        for(long j = 0; j < nbPoint_largeur-1; j++){

            t.push_back( i + j * nbPoint_largeur);
            t.push_back( i + (j+1) * nbPoint_largeur);
            t.push_back( (i+1) + j * nbPoint_largeur);

            t.push_back( i + (j+1) * nbPoint_largeur);
            t.push_back( (i+1) + (j+1) * nbPoint_largeur);
            t.push_back( (i+1) + j * nbPoint_largeur);
        }
    }

    this->setGeom(g);
    this->setTopo(t);
    //Terrain pl = Terrain(g, t, _longueur, _largeur, _resolution);
}

void Terrain::generationTerrainSimple(int width, int lenght, float _uniteDistance, int _resolution, bool sauvegardeRecursive)
{
    Terrain::plan(lenght, width, _uniteDistance, _resolution, sauvegardeRecursive);

    /*************************************/
    /* Modification points */
    /*************************************/

    std::vector<Eigen::Vector3f> listRetour;
    Eigen::Vector3f point;

    for(long i = 0; i < geom.size(); i++)
    {
        point = geom.at(i);

        float h = NoiseGenerator::perlinNoise(point(0)*0.1 ,point(1)*0.1) + 0.5;
        h *= 10;
        point(2) = h;

        listRetour.push_back(point);
    }

    //retour.setGeom(listRetour);

    this->setGeom(listRetour);
}

void Terrain::generationTerrain3noise(int width, int lenght, float _uniteDistance, int _resolution, bool sauvegardeRecursive)
{
    Terrain::generationTerrainSimple(width, lenght, _uniteDistance, _resolution, sauvegardeRecursive);

    /*************************************/
    /* Génération points */
    /*************************************/
    std::vector<Eigen::Vector3f> listRetour;
    Eigen::Vector3f point;

    for(long i = 0; i < geom.size(); i++)
    {
        point = geom.at(i);

        float x = point(0);
        float y = point(1);
        float h = point(2);
        h += 2*NoiseGenerator::perlinNoise(x/10 ,y/10);
        h += NoiseGenerator::perlinNoise(x/2 ,y/2);

        point(2) = h;

        listRetour.push_back(point);

    }

    this->setGeom(listRetour);
}

void Terrain::generationTerrainCrete(int width, int lenght, float _uniteDistance, int _resolution, bool sauvegardeRecursive)
{
    generationTerrain3noise(width, lenght, _uniteDistance, _resolution, sauvegardeRecursive);

    float ZMax = 0.0;
    int NBPoints = geom.size();

    for(int i = 0; i < NBPoints; i++)
    {

        geom.at(i)(2) *= ( geom.at(i)(2) /  10 );

    }
}

void Terrain::generationTerrainCreteNoise(int width, int lenght, float _uniteDistance, int _resolution, bool sauvegardeRecursive)
{
    generationTerrainCrete(width, lenght, _uniteDistance, _resolution, sauvegardeRecursive);
    int NBPoints = geom.size();

    for(int i = 0; i < NBPoints; i++)
    {
        float x = geom.at(i)(0);
        float y = geom.at(i)(1);

        //double noise = NoiseGenerator::perlinNoise(x/5 ,y/8);

        double noiseAmplitude = ( NoiseGenerator::perlinNoise(x/50 ,y/15)+0.75 ) /2;
        noiseAmplitude -= 0.2*( NoiseGenerator::perlinNoise(x/10 ,y/5)+0.75 ) /2;

        geom.at(i)(2) *= noiseAmplitude;
    }
}

void Terrain::generationTerrainCreteNoisePointe(int width, int lenght, float _uniteDistance, int _resolution, bool sauvegardeRecursive)
{
    generationTerrain3noise(width, lenght, _uniteDistance, _resolution, sauvegardeRecursive);

    int NBPoints = geom.size();

    for(int i = 0; i < NBPoints; i++)
    {
        float x = geom.at(i)(0);
        float y = geom.at(i)(1);

        double noise = NoiseGenerator::perlinNoise(x ,y);

        double noiseAmplitude = ( NoiseGenerator::perlinNoise(x/10 ,y/5)+0.5 ) /4;

        if(noiseAmplitude <=0)
            noiseAmplitude = 0;

        geom.at(i)(2) *=  geom.at(i)(2)/( 5 + noise ) * noiseAmplitude;
    }
}

void Terrain::generationTerrainParametre(Terrain &terrainBase, std::vector<ZoneTerrain> parametre)
{
    std::vector<Eigen::Vector3f> G = terrainBase.getGeom();
    int NBPoints = G.size();

    for(int i = 0; i < NBPoints; i++)
    {
        float x = G.at(i)(0);
        float y = G.at(i)(1);
        float h = 0;
        float sommePonderation = 0;

        for(int pi=0; pi < parametre.size(); pi++){
            sommePonderation += parametre.at(pi).getPonderationPoint(x, y);
        }

        if(sommePonderation > 1){
            for(int pi=0; pi < parametre.size(); pi++){
                h+= (parametre.at(pi).getPonderationPoint(x, y)/sommePonderation) * parametre.at(pi).getAltitudePoint(x,y);
            }
            sommePonderation = 1;
        }
        else if(sommePonderation > 0){
            for(int pi=0; pi < parametre.size(); pi++){
                h+= parametre.at(pi).getPonderationPoint(x, y) * parametre.at(pi).getAltitudePoint(x,y);
            }

            h+= G.at(i)(2) * (1-sommePonderation);
            G.at(i)(2) = h;
        }
    }

    this->setGeom(G);
    //Terrain(G, terrainBase.getTopo(), terrainBase.longueur, terrainBase.largeur, terrainBase.resolution);
}

float Terrain::interpolation(float a, float b, float x)
{
    return (1. - x) * a + x * b;
}


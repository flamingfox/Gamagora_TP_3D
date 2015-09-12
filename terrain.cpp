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


/********************Terrain Image************************/

Terrain::Terrain(const QImage &img, float _longueur, float _largeur, float amplitude)
{
    simpleInitImage(img, _longueur, _largeur, amplitude);
}


Terrain::Terrain(const QImage& img, float _longueur, float _largeur, float amplitude, int _nbHeight, int _nbWidth)
{
    if(_nbHeight == img.height() && _nbWidth == img.width())
        simpleInitImage(img, _longueur, _largeur, amplitude);
    else
    {
        //besoin d'interpolation


        simpleInitTopo(_nbHeight, _nbWidth);
    }
}

//construit un terrain avec le même nombre de point que le nombre de pixel de l'image
void Terrain::simpleInitImage(const QImage& img, float _longueur, float _largeur, float _amplitude)
{
    int nbHeight = img.height(),
        nbWidth = img.width();
    float espX = _largeur/(nbWidth-1),
          espY = _longueur/(nbHeight-1);

    this->geom.resize(nbHeight*nbWidth);

    for(int j = 0;  j < nbHeight;   j++)
        for(int i = 0;  i < nbWidth;    i++)
        {
            Eigen::Vector3f& p = this->geom[i+j*nbWidth];
            p(0) = i * espX;
            p(1) = j * espY;
            p(2) = (qGray(img.pixel(i, j))*_amplitude)/255.0;
        }

    this->simpleInitTopo(nbHeight, nbWidth);
}

void Terrain::simpleInitTopo(int nbHeight, int nbWidth)
{
    this->topo.reserve((nbHeight-1)*(nbWidth-1)*6); //grille 5p x 7p = 35p => 24 carrés (4*6) = 48 triangles = 144 int = (5-1) * (7-1) * 2 * 3

    for(int j = 0; j < nbHeight-1; j++){
        for(int i = 0; i < nbWidth-1; i++)
        {
            //triangle 1: 0,2,1
            topo.push_back( i + j * nbWidth);
            topo.push_back( i + (j+1) * nbWidth);
            topo.push_back( (i+1) + j * nbWidth);

            //triangle 2: 2,3,1
            topo.push_back( i + (j+1) * nbWidth);
            topo.push_back( (i+1) + (j+1) * nbWidth);
            topo.push_back( (i+1) + j * nbWidth);
        }
    }
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


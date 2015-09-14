#include "terrain.h"

Terrain::Terrain() : longueur(0), largeur(0), nbPointLargeur(0), nbPointLongueur(0)
{}

Terrain::Terrain(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur) : longueur(_longueur),
    largeur(_largeur), nbPointLongueur(_nbPointLongueur), nbPointLargeur(_nbPointLargeur)
{
    generationTerrain(largeur, longueur, nbPointLongueur, nbPointLargeur);
}

/*Terrain::Terrain(int _longueur, int _largeur, int _resolution, std::vector<ZoneTerrain> parametre) :
    Terrain(type, _longueur, _largeur, _resolution)
{
    generationTerrainParametre(*this, parametre);
}*/


/********************Terrain Image************************/

Terrain::Terrain(const QImage &img, float _longueur, float _largeur, float amplitude):
    nbPointLongueur(img.height()), nbPointLargeur(img.width())
{
    simpleInitImage(img, _longueur, _largeur, amplitude);
}


Terrain::Terrain(const QImage& img, float _longueur, float _largeur, float _amplitude, int _nbHeight, int _nbWidth):
    nbPointLongueur(_nbHeight), nbPointLargeur(_nbWidth)
{
    if(_nbHeight == img.height() && _nbWidth == img.width())
        simpleInitImage(img, _longueur, _largeur, _amplitude);
    else
    {
        geom.reserve(_nbHeight*_nbWidth);

        for(int j = 0;  j < _nbHeight;   j++)
        {
            float y = j*(float)(img.height()-1)/(_nbHeight-1);
            int y1 = floorf(y), y2 = ceilf(y);
            for(int i = 0;  i < _nbWidth;    i++)
            {
                float x = i*(float)(img.width()-1)/(_nbWidth-1);
                int x1 = floorf(x), x2 = ceilf(x);
                float z = interp::interp_hermite2D(x,y,
                                                  x1,y1,x2,y2,
                                                  (qGray(img.pixel(x1, y1))*_amplitude)/255.0,
                                                  (qGray(img.pixel(x1, y2))*_amplitude)/255.0,
                                                  (qGray(img.pixel(x2, y1))*_amplitude)/255.0,
                                                  (qGray(img.pixel(x2, y2))*_amplitude)/255.0
                                                  );
                this->geom.push_back(Eigen::Vector3f(((float)i/(_nbWidth-1))*_largeur, ((float)j/(_nbHeight-1))*_longueur, z));
            }
        }

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
    return Eigen::Vector2d( ( longueur-(longueur/nbPointLongueur) ),
                            ( largeur-(longueur/nbPointLargeur) ));
}

float Terrain::getHauteur(Eigen::Vector2f pointXY)
{
    return getHauteur(pointXY(0), pointXY(1));
}

float Terrain::getHauteur(float pointX, float pointY)
{
    Eigen::Vector2d dim = getDimension();

    int indiceX = (pointX) * ( nbPointLargeur/dim(0) );
    int indiceY = pointY * ( nbPointLongueur/dim(1) );
    indiceX *= nbPointLargeur;

    Eigen::Vector3f point = geom.at( indiceX + indiceY);

    return point(2);
}


/* -------------------------------------------- */
/* -------------------------------------------- */
/*            générateur de terrain             */
/* -------------------------------------------- */
/* -------------------------------------------- */

void Terrain::generationTerrain(int width, int lenght, int nbPointLongueur, int nbPointLargeur)
{
    plan(lenght, width, nbPointLongueur, nbPointLargeur);
    applicationNoise(200, 250);
    this->save("terrainNoise1.obj");
    applicationRidge(150, 50, 500);
    this->save("terrainRidge1.obj");
    applicationWarp(30, 100);
    this->save("terrainWarp1.obj");


    applicationNoise(20, 100);
    this->save("terrainNoise2.obj");
    applicationRidge(150, 50, 400);
    this->save("terrainRidge2.obj");
    applicationWarp(10, 75);
    this->save("terrainWarp2.obj");


    applicationNoise(5, 50);
    this->save("terrainNoise3.obj");
    applicationWarp(5, 25);
    this->save("terrainWarp3.obj");
}

bool Terrain::inOut(Eigen::Vector3f pointXYZ)
{
    if(pointXYZ(2) > getHauteur(pointXYZ(0), pointXYZ(1))){
        return false;
    }

    return true;
}

void Terrain::plan(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur)
{
    std::vector<Eigen::Vector3f> g;
    g.reserve(_nbPointLongueur*_nbPointLargeur);

    //float progressionLongueur = _longueur/(_nbPointLongueur-1.0f);      //2 points, 10m => point n2 = (10,0) => (10*(2-1),(
    //float progressionLargeur = _largeur/(_nbPointLargeur-1.0f);

    /*for(float i = 0; i < _longueur; i+= progressionLongueur )
    {
        for(float j = 0; j < _largeur; j+= progressionLargeur )
        {
            g.push_back(Eigen::Vector3f(i, j, 0));
        }
    }*/
    for(int j = 0;  j < _nbPointLongueur;   j++)
        for(int i = 0;  i < _nbPointLargeur;    i++)
            g.push_back(Eigen::Vector3f(((float)i/(_nbPointLargeur-1))*_largeur, ((float)j/(_nbPointLongueur-1))*_longueur, 0));


    std::vector<int> t;
    t.reserve((_nbPointLongueur-1)*(_nbPointLargeur-1)*6);
    for(int i = 0; i < _nbPointLongueur-1; i++){
        for(int j = 0; j < _nbPointLargeur-1; j++){

            t.push_back( i + j * _nbPointLargeur);
            t.push_back( i + (j+1) * _nbPointLargeur);
            t.push_back( (i+1) + j * _nbPointLargeur);

            t.push_back( i + (j+1) * _nbPointLargeur);
            t.push_back( (i+1) + (j+1) * _nbPointLargeur);
            t.push_back( (i+1) + j * _nbPointLargeur);
        }
    }

    this->setGeom(g);
    this->setTopo(t);
}

void Terrain::applicationNoise(int amplitude, int periode)
{
    /*************************************/
    /* Modification points */
    /*************************************/

    std::vector<Eigen::Vector3f> listRetour;
    Eigen::Vector3f point;

    for(long i = 0; i < geom.size(); i++)
    {
        point = geom.at(i);

        float h = NoiseGenerator::perlinNoise( point(0)*(1.0/periode), point(1)*(1.0/periode) );
        h = (h+1)/2;
        h *= amplitude;
        point(2) += h;

        listRetour.push_back(point);
    }

    this->setGeom(listRetour);
}

void Terrain::applicationRidge(float seuil, float amplitude, int periode)
{
    std::vector<Eigen::Vector3f> listRetour;
    Eigen::Vector3f point;

    for(long i = 0; i < geom.size(); i++)
    {
        point = geom.at(i);

        float hRidge = amplitude * NoiseGenerator::perlinNoise( point(0)*(1.0/periode), point(2)*(1.0/periode) );
        hRidge += seuil;

        //std::cout << hRidge << std::endl;

        if(point(2) > hRidge){
            point(2) = 2*hRidge - point(2);
        }

        listRetour.push_back(point);
    }

    this->setGeom(listRetour);

}

void Terrain::applicationWarp(int amplitude, int periode)
{

    std::vector<Eigen::Vector3f> listRetour;

    Eigen::Vector3f point;

    for(long i = 0; i < geom.size(); i++)
    {
        point = geom.at(i);

        float warp = amplitude * NoiseGenerator::perlinNoise( point(0)*(1.0/periode)+2.78, point(1)*(1.0/periode)+8.72);

        point(0) += warp;
        point(1) += warp;

        listRetour.push_back(point);
    }

    this->setGeom(listRetour);

}

/*void Terrain::generationTerrainSimple(int width, int lenght, int _resolution)
{

    /*************************************/
    /* Modification points */
    /*************************************/

    /*std::vector<Eigen::Vector3f> listRetour;
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

    /*std::vector<Eigen::Vector3f> listRetour;
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
}*/

/*void Terrain::generationTerrainParametre(Terrain &terrainBase, std::vector<ZoneTerrain> parametre)
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
}*/

float Terrain::maxElevation()
{
    Eigen::Vector3f point;
    float hMax = 0;

    for(long i = 0; i < geom.size(); i++)
    {
        point = geom.at(i);

        if(point(2)>hMax)
            hMax = point(2);
    }

    return hMax;
}



float Terrain::interpolation(float a, float b, float x)
{
    return (1. - x) * a + x * b;
}


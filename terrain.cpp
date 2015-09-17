#include "terrain.h"

Terrain::Terrain() : longueur(0), largeur(0), nbPointLargeur(0), nbPointLongueur(0)
{}

Terrain::Terrain(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur) : longueur(_longueur),
    largeur(_largeur), nbPointLongueur(_nbPointLongueur), nbPointLargeur(_nbPointLargeur)
{
    //plan(_longueur,_largeur,_nbPointLongueur,_nbPointLargeur);
    generationTerrain(largeur, longueur, nbPointLongueur, nbPointLargeur);
}

/*Terrain::Terrain(int _longueur, int _largeur, int _resolution, std::vector<ZoneTerrain> parametre) :
    Terrain(type, _longueur, _largeur, _resolution)
{
    generationTerrainParametre(*this, parametre);
}*/


/********************Terrain Image************************/

Terrain::Terrain(const QImage &img, float _longueur, float _largeur, float amplitude):
    nbPointLongueur(img.height()), nbPointLargeur(img.width()), longueur(_longueur), largeur(_largeur)
{
    simpleInitImage(img, _longueur, _largeur, amplitude);
}


Terrain::Terrain(const QImage& img, float _longueur, float _largeur, float _amplitude, int _nbHeight, int _nbWidth):
    nbPointLongueur(_nbHeight), nbPointLargeur(_nbWidth), longueur(_longueur), largeur(_largeur)
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

        simpleInitTopo();
    }
}

/**construit un terrain avec le même nombre de point que le nombre de pixel de l'image*/
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

    this->simpleInitTopo();
}

void Terrain::simpleInitTopo()
{
    this->topo.reserve((nbPointLongueur-1)*(nbPointLargeur-1)*6); //grille 5p x 7p = 35p => 24 carrés (4*6) = 48 triangles = 144 int = (5-1) * (7-1) * 2 * 3

    for(int j = 0; j < nbPointLongueur-1; j++){
        for(int i = 0; i < nbPointLargeur-1; i++)
        {
            //triangle 1: 0,1,2
            addTopo(i + j * nbPointLargeur,
                    (i+1) + j * nbPointLargeur,
                    i + (j+1) * nbPointLargeur);

            //triangle 2: 1,3,2
            addTopo((i+1) + j * nbPointLargeur,
                    (i+1) + (j+1) * nbPointLargeur,
                    i + (j+1) * nbPointLargeur);
        }
    }
}

void Terrain::calculNormals()
{
    normalsTriangles();   //reconstruit toutes les normales des triangles du terrain.
    //maintenant, on calcul les normales des points
    this->normalsPoints.reserve(nbGeom());

    for(int j = 0;  j < nbPointLongueur;    j++)    {
        for(int i = 0;  i < nbPointLargeur; i++)
        {
            Eigen::Vector3f n(0,0,0);

            int t6 = ((i+ j*nbPointLargeur)-j)*2;   //pour 5x5, p12: 12 = ligne 2:  t6 = (12-2)*2 = 20

            if(j < nbPointLongueur-1) {
                if(i < nbPointLargeur-1)
                    n += normals[t6];
                if(i > 0) {
                    n += normals[t6-1];
                    n += normals[t6-2];
                }
            }

            if(j > 0)
            {
                int t3 = t6-(nbPointLargeur-1)*2;   //p12:  t3 = 20 - 4*2 = 12;
                if(i < nbPointLargeur-1)
                    n += normals[t3];
                if(i > 0)                {
                    n += normals[t3-1];
                    n += normals[t3-2];
                }
            }

            normalsPoints.push_back(n.normalized());
        }
    }
}




Eigen::Vector2d Terrain::getDimension() const
{
    return Eigen::Vector2d( largeur,
                            longueur);
}

float Terrain::getHauteur(Vector2f &pointXY) const
{
    return getHauteur(pointXY(0), pointXY(1));
}


float Terrain::getHauteur(float pointX, float pointY) const
{
    if(pointX < 0 || pointY < 0 || pointX > largeur || pointY > longueur)
        return HAUTEUR_HORS_MAP;

    /*int indiceX = pointX * ( (float)nbPointLargeur / largeur );         //à revoir
    int indiceY = ( (int)pointY ) * ( (float)nbPointLongueur / longueur ) * ( nbPointLargeur );

    Eigen::Vector3f point11 = geom.at( indiceX + indiceY ),
            point12 = geom.at(indiceX+1 + indiceY),
            point21 = geom.at(indiceX + indiceY+nbPointLargeur),
            point22 = geom.at(indiceX+1 + indiceY+nbPointLargeur);

    return interp::interp_linear2D(pointX, pointY, point11(0), point11(1), point22(0), point22(1),
                                   point11(2), point12(2), point21(2), point22(2));*/

    float x = (pointX * (nbPointLargeur-1)) / largeur,    //largeur: 1m et 5 points: (1.0f*(5-1))/1 = 4.0f donc regarder l'indice 4
          y = (pointY * (nbPointLongueur-1)) / longueur;

    int x1 = floorf(x),
        y1 = floorf(y),
        x2 = ceilf(x),
        y2 = ceilf(y);

    const Eigen::Vector3f   & p11 = getPoint(x1, y1);

    if(y1 == y2)    {
        if(x1 == x2 )
            return p11(2);
        return interp::interp_linear1D(x, p11(2), getPoint(x2, y1)(2));
    }
    else    {
        const Eigen::Vector3f   & p12 = getPoint(x1, y2);
        if(x1 == x2 )
            return interp::interp_linear1D(x, p11(2), p12(2));
        return interp::interp_linear2D(x, y, x1, y1, x2, y2,
                                       p11(2), p12(2), getPoint(x2, y1)(2), getPoint(x2, y2)(2));
    }
}

Vector3f Terrain::getNormal(const Vector3f &pointXYZ) const
{
   return getNormal(pointXYZ(0), pointXYZ(1));
}

Vector3f Terrain::getNormal(const Vector2f &pointXY) const
{
    return getNormal(pointXY(0), pointXY(1));
}

Vector3f Terrain::getNormal(float pointX, float pointY) const
{
    /*int indiceX = pointX * ( (float)nbPointLargeur / largeur );
    int indiceY = ( (int)pointY ) * ( (float)nbPointLongueur / longueur ) * ( nbPointLargeur );

    const Eigen::Vector3f   & point11 = geom.at( indiceX + indiceY ),
                            & normal11 = normalsPoints.at( indiceX + indiceY ),
                            & normal12 = normalsPoints.at(indiceX+1 + indiceY),
                            & normal21 = normalsPoints.at(indiceX + indiceY+nbPointLargeur),
                            & point22 = geom.at(indiceX+1 + indiceY+nbPointLargeur),
                            & normal22 = normalsPoints.at(indiceX+1 + indiceY+nbPointLargeur),
                            normal;
                            */
    float x = (pointX * (nbPointLargeur-1)) / largeur,    //largeur: 1m et 5 points: (1.0f*(5-1))/1 = 4.0f donc regarder l'indice 4
          y = (pointY * (nbPointLongueur-1)) / longueur;

    const Eigen::Vector3f   & p11 = getPoint(floorf(x), floorf(y)),
                            & p22 = getPoint(ceilf(x), ceilf(y)),
                            & normal11 = getN(floorf(x), floorf(y)),
                            & normal12 = getN(floorf(x), ceilf(y)),
                            & normal21 = getN(ceilf(x), floorf(y)),
                            & normal22 = getN(ceilf(x), ceilf(y));

    Eigen::Vector3f normal;
    normal(0) = interp::interp_linear2D(pointX, pointY, p11(0), p11(1), p22(0), p22(1),
                                   normal11(0), normal12(0), normal21(0), normal22(0));
    normal(1) = interp::interp_linear2D(pointX, pointY, p11(0), p11(1), p22(0), p22(1),
                                   normal11(1), normal12(1), normal21(1), normal22(1));
    normal(2) = interp::interp_linear2D(pointX, pointY, p11(0), p11(1), p22(0), p22(1),
                                   normal11(2), normal12(2), normal21(2), normal22(2));

    normal.normalize();

    return normal;
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
    //this->save("terrainNoise1.obj");
    applicationRidge(150, 50, 500);
    //this->save("terrainRidge1.obj");
    //applicationWarp(30, 100);
    //this->save("terrainWarp1.obj");


    applicationNoise(20, 100);
    //this->save("terrainNoise2.obj");
    applicationRidge(150, 50, 400);
    //this->save("terrainRidge2.obj");
    //applicationWarp(10, 75);
    //this->save("terrainWarp2.obj");


    applicationNoise(5, 50);
    //this->save("terrainNoise3.obj");
    //applicationWarp(5, 25);
    this->save("terrainWarp3.obj");

    calculNormals();
    englobant = Box(geom);
}

bool Terrain::inOut(const Eigen::Vector3f& pointXYZ)
{
    if(pointXYZ(2) > getHauteur(pointXYZ(0), pointXYZ(1))){
        return false;
    }

    return true;
}

bool Terrain::intersect(const Rayon& rayon, float &coeffDistance) const{

    float dmin = 0.0;
    float dmax = 5000.0;

    if(!englobant.intersect(rayon, dmin, dmax ))
        return false;

    dmin = 0.0;
    dmax = 5000.0;

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

void Terrain::plan(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur)
{
    geom.reserve(_nbPointLongueur*_nbPointLargeur);

    for(int j = 0;  j < _nbPointLongueur;   j++)
        for(int i = 0;  i < _nbPointLargeur;    i++)
            geom.push_back(Eigen::Vector3f(((float)i/(_nbPointLargeur-1))*_largeur, ((float)j/(_nbPointLongueur-1))*_longueur, 0));

    this->simpleInitTopo();
}

void Terrain::applicationNoise(int amplitude, int periode)
{
    /*************************************/
    /* Modification points */
    /*************************************/

    //for(size_t i = 0; i < geom.size(); i++)
    for(Eigen::Vector3f& p: geom)
    {
        float h = NoiseGenerator::perlinNoise( p(0)*(1.0/periode), p(1)*(1.0/periode) );
        h = (h+1)/2;
        h *= amplitude;
        p(2) += h;
    }
}

void Terrain::applicationRidge(float seuil, float amplitude, int periode)
{
    //for(size_t i = 0; i < geom.size(); i++)
    for(Eigen::Vector3f& p: geom)
    {
        float hRidge = amplitude * NoiseGenerator::perlinNoise( p(0)*(1.0/periode), p(2)*(1.0/periode) );
        hRidge += seuil;

        //std::cout << hRidge << std::endl;

        if(p(2) > hRidge)
            p(2) = 2*hRidge - p(2);
    }
}

void Terrain::applicationWarp(int amplitude, int periode)
{
    //for(size_t i = 0; i < geom.size(); i++)
    for(Eigen::Vector3f& p: geom)
    {
        float warp = amplitude * NoiseGenerator::perlinNoise( p(0)*(1.0/periode)+2.78, p(1)*(1.0/periode)+8.72);

        p(0) += warp;
        p(1) += warp;
    }
}

float Terrain::maxElevation()
{
    float hMax = FLT_MIN;

    for(const Eigen::Vector3f& p: geom)
    {
        if(p(2)>hMax)
            hMax = p(2);
    }

    return hMax;
}



float Terrain::interpolation(float a, float b, float x){
    return (1. - x) * a + x * b;
}

inline const Vector3f& Terrain::getPoint(int i, int j) const{
    return geom[i+j*nbPointLargeur];
}

inline const Vector3f& Terrain::getPoint(const Eigen::Vector2i& pos) const{
    return getPoint(pos(0), pos(1));
}

inline const Vector3f& Terrain::getN(int i, int j) const{
    return normalsPoints[i+j*nbPointLargeur];
}

inline const Vector3f& Terrain::getN(const Eigen::Vector2i& pos) const{
    return getN(pos(0), pos(1));
}

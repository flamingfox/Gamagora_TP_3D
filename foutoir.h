#ifndef FOUTOIR
#define FOUTOIR

#endif // FOUTOIR

/* ================================================================================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/
/* ============================================  TERRAIN  =========================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/

Eigen::Vector3f getNormal(const Eigen::Vector3f &pointXYZ) const;
Eigen::Vector3f getNormal(const Eigen::Vector2f &pointXY) const;
void generationTerrain(int width, int lenght, int nbPointLongueur, int nbPointLargeur);
void generationTerrainSin(int width, int lenght, int nbPointLongueur, int nbPointLargeur);
bool inOut(const Eigen::Vector3f& pointXYZ);
bool intersectWithMesh(const Rayon& rayon, float &coeffDistance) const;
void calculNormals();
//void generationTerrainParametre(Terrain &terrainBase, std::vector<ZoneTerrain> parametre);
void simpleInitImage(const QImage& img, float _longueur, float _largeur, float _amplitude);
void simpleInitTopo();
//Terrain(int _longueur, int _largeur, int _resolution, std::vector<ZoneTerrain> parametre);
Terrain(const QImage& img, float _longueur, float _largeur, float amplitude, int _nbHeight, int _nbWidth);
Terrain(const QImage& img, float _longueur, float _largeur, float amplitude);
void initFinal();


/*Terrain::Terrain(int _longueur, int _largeur, int _resolution, std::vector<ZoneTerrain> parametre) :
    Terrain(type, _longueur, _largeur, _resolution)
{
    generationTerrainParametre(*this, parametre);
}*/

/*


/********************Terrain Image************************/

Terrain::Terrain(const QImage &img, float _longueur, float _largeur, float amplitude):
    longueur(_longueur), largeur(_largeur), nbPointLongueur(img.height()), nbPointLargeur(img.width())
{
    simpleInitImage(img, _longueur, _largeur, amplitude);
}


Terrain::Terrain(const QImage& img, float _longueur, float _largeur, float _amplitude, int _nbHeight, int _nbWidth):
    longueur(_longueur), largeur(_largeur), nbPointLongueur(_nbHeight), nbPointLargeur(_nbWidth)
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

float Terrain::getHauteur(float pointX, float pointY) const
{
    if(pointX < 0 || pointY < 0 || pointX > largeur || pointY > longueur)
        return HAUTEUR_HORS_MAP;

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


void Terrain::initFinal()
{
    calculNormals();
    englobant = Box(geom);
}




float Terrain::getHauteur(Vector2f &pointXY) const
{
    return getHauteur(pointXY(0), pointXY(1));
}


void Terrain::generationTerrain(int width, int lenght, int nbPointLongueur, int nbPointLargeur)
{
    plan(lenght, width, nbPointLongueur, nbPointLargeur);
    applicationNoise(200, 1000);
    applicationRidge(150, 50, 500);

    applicationNoise(20, 100);
    applicationRidge(150, 50, 400);

    applicationNoise(5, 50);

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

bool Terrain::intersectWithMesh(const Rayon& rayon, float &coeffDistance) const{

    float dmin = 0.0;
    float dmax = 3000.0;

    if(!englobant.intersect(rayon, dmin, dmax ))
        return false;

    dmin = 0.0;
    dmax = 3000.0;

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
        float h = NoiseGenerator::perlinNoise( p(0)/periode, p(1)/periode );
        h = (h+1)/2;
        h *= amplitude;
        p(2) += h;
    }
}



void Terrain::applicationRidge(float seuil, float amplitude, int periode)
{
    for(Eigen::Vector3f& p: geom)
    {
        float hRidge = amplitude * NoiseGenerator::perlinNoise( p(0)*(1.0/periode), p(2)*(1.0/periode) );
        hRidge += seuil;

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

void Terrain::applicationSin(int amplitude, int periode)
{
    //for(size_t i = 0; i < geom.size(); i++)
    for(Eigen::Vector3f& p: geom)
    {
        float warp = amplitude * sin(p(0)*periode) + amplitude * sin(p(1)*periode);
        p(0) += 50;
        p(1) += 50;
        p(2) += warp +50;
    }
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



/* ================================================================================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/
/* ============================================    MAIN   =========================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/
/* ================================================================================================================*/

void testNormals()
{
    Terrain m = Terrain(150, 150, 5, 5);
    m.normalsTriangles();
    m.calculNormals();
    m.save("terrain.obj");

}

void testBox()
{
    Terrain m = Terrain(150, 150, 150, 150);
    Box box(m);
    std::cout << "min = " << std::endl << box.min << std::endl <<
                 "max = " << std::endl << box.max << std::endl;
}

void testSphere()
{
    Mesh m = Mesh::sphere(Vector3f(0.5,1,-1),2,10);
    m.save("sphere.obj");
}

void testImage(const QImage& img)
{
    Terrain m = Terrain(img, 100, 100, 10);
    m.save("image.obj");
}


void testTotal()
{
    //srand(time(NULL));

    std::cout << "Hello !" << std::endl << "World generating !" << std::endl;

    Terrain m = Terrain(1500, 1500, 250, 250);
    m.save("terrain.obj");


    /*Terrain m = Terrain(Terrain::PLAT, 150, 150, 2, 0.6);
    m.save("plan.obj");

    m = Terrain(Terrain::SIMPLE, 150, 150, 2, 0.6);
    m.save("terrainSimple.obj");

    m = Terrain(Terrain::NOISE_3, 150, 150, 2, 0.6);
    m.save("terrain3noise.obj");

    m = Terrain(Terrain::CRETE, 150, 150, 2, 0.6);
    m.save("TerrainCrete.obj");

    m = Terrain(Terrain::CRETE_NOISE, 150, 150, 2, 0.6);
    m.save("TerrainCreteNoise.obj");

    m = Terrain(Terrain::CRETE_NOISE_POINTE, 150, 150, 2, 0.6);
    m.save("TerrainCreteNoisePointe.obj");
    m.rescale(5);*/

    /*std::vector<ZoneTerrain> listZone;

    listZone.push_back( ZoneTerrain( Eigen::Vector2f(100,100), 20, 25, 2, 2, 2, 6));

    listZone.push_back( ZoneTerrain( Eigen::Vector2f(20,50), -15, 25, 2, 2, 2, 3));



    m = Terrain::generationTerrainParametre(m, listZone);
    m.save("TerrainParametre.obj");*/

    /*Mesh m = Mesh::cylindre(Eigen::Vector3f(0,0,0), Eigen::Vector3f(0,2,0), 1.0);
    m.save("cylindre.obj");*/

    /*Mesh m = Mesh::sphere(Eigen::Vector3f(0,0,1), 0.25);
    m.Rotation(0,0,90);
    m.save("sphere.obj");*/

    /*m.merge(Mesh::cylindre(Eigen::Vector3f(0,0,0), Eigen::Vector3f(0,2,0), 1.0));
    m.save("tree_ooo.obj");*/

    /*Mesh a = Arbre(Arbre::SAULE).getMesh();
    a.Rotation(90,0,0);
    a.save("SAULE.obj");
    a.Translation(300,300,0);
    m.merge(a);
    a.Translation(-5,0,0);
    m.merge(a);
    a.Translation(0,-5,0);
    m.merge(a);
    a.Translation(5,0,0);
    m.merge(a);

    a = Arbre(Arbre::CHARME).getMesh();
    a.save("CHARME.obj");
    a.Rotation(90,0,0);
    a.Translation(300,160,0);
    m.merge(a);
    a.Translation(-5,0,0);
    m.merge(a);
    a.Translation(0,-5,0);
    m.merge(a);
    a.Translation(5,0,0);
    m.merge(a);

    a = Arbre(Arbre::SAPIN).getMesh();
    a.save("SAPIN.obj");
    a.Rotation(90,0,0);
    a.Translation(50,150,0);
    m.merge(a);
    a.Translation(-5,0,0);
    m.merge(a);
    a.Translation(0,-5,0);
    m.merge(a);
    a.Translation(5,0,0);
    m.merge(a);

    m.save("terrainVegetation.obj");*/

    /*GenerationVegetation gV;
    int cranAnnee = 20;
    int nbAnneeMax = cranAnnee*10;

    Terrain t;
    gV.initialisationArbreSurPlat(4, 4, 20);

    for(int nbAnnee=0; nbAnnee < nbAnneeMax; nbAnnee += cranAnnee){
        gV.iteration(cranAnnee);
        t = gV.getTerrain();
        std::ostringstream ss;
        ss << nbAnnee;
        t.save("terrainVegetation"+ss.str()+".obj");
    }*/

    //return 0;
}

void testIntersect(){
    Box b = Box(Eigen::Vector3f(1,1,1), Eigen::Vector3f(3,3,3));
    Rayon r = Rayon(Eigen::Vector3f(4,4,4), Eigen::Vector3f(-1,-1,-1));

    float distanceMin = 0, distanceMax = 0;

    if(b.intersect(r, distanceMin, distanceMax)){
        std::cout << "Intersection, dMin = " << distanceMin << " , dMax = " << distanceMax << std::endl;
    }
    else{
        std::cout << "Pas d'intersection" << std::endl;
    }
}

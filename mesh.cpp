#include "mesh.h"


Mesh::Mesh(const QImage &img, float _longueur, float _largeur, float amplitude):
    longueur(_longueur), largeur(_largeur), nbPointLongueur(img.height()), nbPointLargeur(img.width())
{
    simpleInitImage(img, _longueur, _largeur, amplitude);
}

Mesh::Mesh(const QImage& img, float _longueur, float _largeur, float _amplitude, int _nbHeight, int _nbWidth):
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


void Mesh::Translation(const Eigen::Vector3f T){
    for(Eigen::Vector3f& p : geom)
        p +=T;
}

void Mesh::Translation(const float x, const float y, const float z)
{
    for(Eigen::Vector3f& p: geom){
        p(0)+=x;
        p(1)+=y;
        p(2)+=z;
    }
}

void Mesh::Rotation(const Eigen::Vector3f T){
    Rotation(T(0), T(1), T(2));
}



void Mesh::Rotation(const float rX, const float rY, const float rZ){

    Eigen::Matrix3f matriceRotation;
    matriceRotation = Eigen::AngleAxisf(rX/360*2*M_PI, Eigen::Vector3f::UnitX()) *
            Eigen::AngleAxisf(rY/360*2*M_PI, Eigen::Vector3f::UnitY()) *
            Eigen::AngleAxisf(rZ/360*2*M_PI, Eigen::Vector3f::UnitZ());

    for(size_t i=0; i<geom.size(); i++){
        geom[i] = matriceRotation*geom[i];
    }
}

/* -------------------------------------------- */
/* -------------------------------------------- */
/* -------------------------------------------- */
/*            générateur de formes              */
/* -------------------------------------------- */
/* -------------------------------------------- */
/* -------------------------------------------- */

Mesh Mesh::cylindre(const Eigen::Vector3f& centreCercleA, const Eigen::Vector3f& centreCercleB, const float rayon, int resolution)
{
    Mesh m;
    m.geom.reserve((resolution+1)*2);
    m.topo.reserve(4*resolution);

    Eigen::Vector3f normal = centreCercleB - centreCercleA;

    Eigen::Vector3f axe1(normal(1),-normal(0),0),
                    axe2(axe1.cross(normal));

    axe1.normalize();
    axe1*=rayon;
    axe2.normalize();
    axe2*=rayon;

    //P = Rcos(t)u +Rsin(t)resolution × u + c;
    //M=C+A*cos(t)+B*sin(t)

    /*************************************/
    /* Génération points */
    /*************************************/

    float ecartRadiant = (2*M_PI)/resolution;

    for(int i=0; i<resolution; i++){
        Eigen::Vector3f p = cos(i*ecartRadiant)*axe1 + sin(i*ecartRadiant)*axe2 + centreCercleA;
        //std::cout << p << std::endl;
        m.geom.push_back( p );
        m.addTopo(resolution*2, i, (i+1)%resolution);
    }

    for(int i=0; i<resolution; i++){
        m.geom.push_back( m.geom[i]+normal );
        m.addTopo(resolution*2+1, resolution+(i+1)%resolution, resolution+i);
        m.addTopo(i,i+resolution, (i+1)%resolution);
        m.addTopo(i+resolution, resolution+(i+1)%resolution, (i+1)%resolution);
    }

    m.geom.push_back(centreCercleA); // 2 resolution
    m.geom.push_back(centreCercleB); // 2 resolution+1


    /*************************************/
    /* Génération faces */
    /*************************************/

    /***** Poles ****/
    /*for(int i=0; i<= resolution-1; i++){
        t.push_back(i); t.push_back(i+1); t.push_back(2*resolution);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(2*resolution);

    for(int i=resolution ; i<= 2*resolution-1; i++){
        t.push_back(i); t.push_back(i+1); t.push_back(2*resolution+1);
    }
    t.push_back(2*resolution-1); t.push_back(resolution); t.push_back(2*resolution+1);*/

    /***** faces cylindre *****/
    /*for(int i=0; i< resolution-1; i++){

        t.push_back(i); t.push_back(i+1); t.push_back(resolution+i);
        t.push_back(resolution+i+1); t.push_back(resolution+i); t.push_back(i+1);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(resolution);
    t.push_back(resolution); t.push_back(2*resolution-1); t.push_back(resolution-1);


    Mesh retour(v,t);*/

    m.normalsTriangles();
    return m;
}


Mesh Mesh::cone(const Eigen::Vector3f &centreCercle, const Eigen::Vector3f &pointe, const float rayon, const int resolution)
{
    Mesh m;
    m.geom.reserve(resolution+2);
    m.topo.reserve(2*resolution);

    Eigen::Vector3f normal = pointe - centreCercle;

    Eigen::Vector3f axe1(normal(1), -normal(0), 0),
                    axe2(axe1.cross(normal));

    axe1.normalize();
    axe1*=rayon;
    axe2.normalize();
    axe2*=rayon;

    //P = Rcos(t)u +Rsin(t)resolution × u + c;
    //M=C+A*cos(t)+B*sin(t)

    /*************************************/
    /* Génération points */
    /*************************************/

    float ecartRadiant = (2*M_PI)/resolution;

    for(int i=0; i<resolution; i++){
        Eigen::Vector3f p = cos(i*ecartRadiant)*axe1 + sin(i*ecartRadiant)*axe2 + centreCercle;
        //std::cout << p << std::endl;
        m.geom.push_back( p );
        m.addTopo(resolution,i,(i+1)%resolution);
        m.addTopo(resolution+1, i, (i+1)%resolution);
    }

    m.geom.push_back(centreCercle); // resolution
    m.geom.push_back(pointe); // resolution+1


    /*************************************/
    /* Génération faces */
    /*************************************/

    /***** Poles ****/
    /*for(int i=0; i< resolution-1; i++){
        t.push_back(i); t.push_back(i+1); t.push_back(resolution);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(resolution);*/

    /***** faces cylindre *****/
    /*for(int i=0; i< resolution-1; i++){

        t.push_back(i); t.push_back(i+1); t.push_back(resolution+1);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(resolution+1);


    Mesh retour(v,t);*/

    m.normalsTriangles();
    return m;
}

Mesh Mesh::sphere(const Eigen::Vector3f &centre, const float rayon, const int resolution)
{
    Mesh m = generationSphere(Eigen::Vector3f(0,0,0), rayon, resolution);
    m.Translation(centre);
    return m;
}

Mesh Mesh::galette(const Eigen::Vector3f &centre, const float rayon, const int resolution)
{
    Mesh m = generationSphere(Eigen::Vector3f(0,rayon,0), rayon, resolution);
    m.Translation(centre);
    return m;
}

Mesh Mesh::arbreSpherique(const Eigen::Vector3f& centreCercleA, const Eigen::Vector3f& centreCercleB, const float rayon1, const Eigen::Vector3f& centreCercleC, const float rayon2)
{
    Mesh c = Mesh::cylindre(centreCercleA, centreCercleB, rayon1, 32);
    Mesh s = Mesh::sphere(Eigen::Vector3f(0,0,0), rayon2, 32);
    s.Rotation(0,0,90);
    s.Translation(centreCercleC);
    s.merge(c);
    return s;
}

Mesh Mesh::arbreGalette(const Eigen::Vector3f& centreCercleA, const Eigen::Vector3f& centreCercleB, const float rayon1, const Eigen::Vector3f& centreCercleC, const float rayon2){
    Mesh c = Mesh::cylindre(centreCercleA, centreCercleB, rayon1, 32);
    Mesh s = Mesh::galette(Eigen::Vector3f(0,0,0), rayon2, 32);
    s.Rotation(0,0,90);
    s.Translation(centreCercleC);
    s.merge(c);
    return s;
}

Mesh Mesh::arbreConique(const Eigen::Vector3f &centreCercleA, const Eigen::Vector3f &centreCercleB, const float rayon1, const Eigen::Vector3f &centreCercleC, const Eigen::Vector3f &pointe, const float rayon2)
{
    Mesh c = Mesh::cylindre(centreCercleA, centreCercleB, rayon1, 32);
    Mesh s = Mesh::cone(Eigen::Vector3f(0,0,0), pointe, rayon2, 32);
    s.Translation(centreCercleC);
    s.merge(c);
    return s;
}
/* -------------------------------------------- */
/* -------------------------------------------- */
/* -------------------------------------------- */
/*            Fonctions utilitaires             */
/* -------------------------------------------- */
/* -------------------------------------------- */
/* -------------------------------------------- */

void Mesh::save(const std::string name){

    Eigen::IOFormat objFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", " ");

    std::ofstream obj;

    obj.open(name.c_str(), std::ios::out);

    obj << "#vertices:\n";

    for(size_t i = 0 ; i < geom.size(); i++) {
        obj << "v " << std::setprecision(4) << geom[i].format(objFormat) << "\n";
    }

    obj << "#faces:\n";

    for(size_t i = 0 ; i < topo.size(); i += 3){
        obj << "f " << std::setprecision(4) << topo[i] + 1 << " " << topo[i+1] + 1 << " "<< topo[i+2] + 1 << "\n";
    }

    obj << "\n";

    obj.close();

    std::cout << "wrote " << name << std::endl;
}

void Mesh::merge(const Mesh &delta)
{
    if(&delta != this){
        int taille = geom.size();
        geom.reserve(taille+delta.nbGeom());

        for(size_t i=0; i< delta.geom.size(); i++){
            geom.push_back(delta.geom[i]);
        }

        topo.reserve(this->nbTopo()+delta.nbTopo());
        for(size_t i=0; i< delta.topo.size(); i++){
            topo.push_back(delta.topo[i] + taille );
        }
    }
}

void Mesh::rescale(float scale)
{
    for(std::vector<Eigen::Vector3f>::iterator it = geom.begin();   it != geom.end();   ++it)
        *it *= scale;
    /*for(int i=0; i < geom.size(); i++){
        geom[i] *= scale;
    }*/
}

/* -------------------------------------------- */
/* -------------------------------------------- */
/* -------------------------------------------- */
/*            Fonctions accessors               */
/* -------------------------------------------- */
/* -------------------------------------------- */
/* -------------------------------------------- */

std::vector<Eigen::Vector3f> Mesh::getGeom() const
{
    return geom;
}

std::vector<int> Mesh::getTopo() const
{
    return topo;
}

void Mesh::setGeom(std::vector<Eigen::Vector3f> geom)
{
    this->geom = geom;
}

void Mesh::setTopo(std::vector<int> topo)
{
    this->topo = topo;
}

size_t Mesh::nbGeom() const
{
    return this->geom.size();
}

size_t Mesh::nbTopo() const
{
    return this->topo.size();
}

inline void Mesh::addTopo(int i0, int i1, int i2)
{
    this->topo.push_back(i0);
    this->topo.push_back(i1);
    this->topo.push_back(i2);
}

Mesh Mesh::generationSphere(const Eigen::Vector3f &centre, const float rayon, const int resolution)
{
    Mesh m;
    m.geom.reserve(2+resolution*(resolution/2));
    m.topo.reserve(resolution*resolution/2);
    //std::vector<Eigen::Vector3f> v;
    //std::vector<int> t;

    Eigen::Vector3f normalCercleOrigine(0,rayon,0), vecteurPoint1(rayon,0,0);

    Eigen::Vector3f p;

    /*std::cout << "normalCercleOrigine :" << normalCercleOrigine << std::endl;

    std::cout << "vecteurPoint1 :" << vecteurPoint1 << std::endl;

    std::cout << "normalCercleOrigine.vecteurPoint1 :" << normalCercleOrigine.dot(vecteurPoint1) << std::endl;*/

    //P = Rcos(t)u +Rsin(t)resolution × u + c;
    //M=C+A*cos(t)+B*sin(t)

    /*************************************/
    /* Génération points */
    /*************************************/

    std::vector<Eigen::Vector3f> tmp[resolution];

    float ecartRadiant = (2*M_PI)/resolution;

    tmp[0].reserve(resolution/2 +1);
    for(int i=0; i<= (resolution/2); i++){
        p = cos(i*ecartRadiant)*vecteurPoint1+ sin(i*ecartRadiant)*normalCercleOrigine  + centre;
        std::cout << p << std::endl;
        tmp[0].push_back(p);
    }

    Eigen::Matrix3f matriceRotation;
    //Eigen::Matrix3f matriceRotation(3,3);


    //rotation Z
    /*matriceRotation(0) = 0; matriceRotation(1) = 0; matriceRotation(2) = 0;
    matriceRotation(3) = 0; matriceRotation(4) = 0; matriceRotation(5) = 0;
    matriceRotation(6) = 0; matriceRotation(7) = 0; matriceRotation(8) = 1;*/

    // rotation Y
    /*matriceRotation(0) = 0; matriceRotation(1) = 0; matriceRotation(2) = 0;
    matriceRotation(3) = 0; matriceRotation(4) = 1; matriceRotation(5) = 0;
    matriceRotation(6) = 0; matriceRotation(7) = 0; matriceRotation(8) = 0;*/

    // rotation X
    matriceRotation(0) = 1; matriceRotation(1) = 0; matriceRotation(2) = 0;
    matriceRotation(3) = 0; matriceRotation(4) = 0; matriceRotation(5) = 0;
    matriceRotation(6) = 0; matriceRotation(7) = 0; matriceRotation(8) = 0;
    /*matriceRotation(0,0) = 1; matriceRotation(0,1) = 0; matriceRotation(0,2) = 0;
    matriceRotation(1,0) = 0; matriceRotation(1,1) = 0; matriceRotation(1,2) = 0;
    matriceRotation(2,0) = 0; matriceRotation(2,1) = 0; matriceRotation(2,2) = 0;*/

    for(int i=1; i< resolution; i++){

        //rotation Z
        /*matriceRotation(0) = cos(i*ecartRadiant);
        matriceRotation(3) = sin(i*ecartRadiant);
        matriceRotation(1) = -sin(i*ecartRadiant);
        matriceRotation(4) = cos(i*ecartRadiant);*/

        // rotation Y
        /*matriceRotation(0) = cos(i*ecartRadiant);
        matriceRotation(2) = sin(i*ecartRadiant);
        matriceRotation(6) = -sin(i*ecartRadiant);
        matriceRotation(8) = cos(i*ecartRadiant);*/

        // rotation x
        matriceRotation(4) = cos(i*ecartRadiant);
        matriceRotation(7) = sin(i*ecartRadiant);
        matriceRotation(5) = -sin(i*ecartRadiant);
        matriceRotation(8) = cos(i*ecartRadiant);
        /*matriceRotation(1,1) = cos(i*ecartRadiant);
        matriceRotation(2,1) = sin(i*ecartRadiant);
        matriceRotation(1,2) = -sin(i*ecartRadiant);
        matriceRotation(2,2) = cos(i*ecartRadiant);*/

        tmp[i].reserve((resolution/2)+1);
        for(int j=0; j<= (resolution/2); j++){
            p = matriceRotation * tmp[0].at(j);
            //std::cout << p << std::endl;
            tmp[i].push_back( p );
        }
    }

    m.geom.push_back((tmp[0]).at(0));

    for (int i=1; i< resolution/2 ; i++){
        for(int j=0; j< resolution ; j++){
            p = tmp[j].at(i);
            m.geom.push_back(p);
        }
    }
    m.geom.push_back(tmp[0].back()) ;


    //v.push_back(b); // 2n+1


    /*************************************/
    /* Génération faces */
    /*************************************/

    /***** Poles ****/

    for(int i=1; i< resolution; i++){
        m.addTopo(0,i,i+1);
        //t.push_back(0); t.push_back( i ); t.push_back( i+1 );
    }
    //t.push_back(0); t.push_back(resolution); t.push_back( 1 );
    m.addTopo(0,resolution,1);

    for(int i= ( (resolution/2)-2 )*resolution + 1 ; i< ( (resolution/2)-1 )*resolution; i++){
        /*t.push_back( i );
        t.push_back( ( (resolution/2)-1 )*resolution +1 );
        t.push_back( i+1 );*/
        m.addTopo(i, ( (resolution/2)-1 )*resolution +1, i+1);
    }
    /*t.push_back( ( (resolution/2)-1 ) * resolution + 1 );
    t.push_back( ( (resolution/2)-2 ) * resolution + 1 );
    t.push_back( ( (resolution/2)-1 ) * resolution );*/
    m.addTopo(((resolution/2)-1 ) * resolution + 1,
              ((resolution/2)-2 ) * resolution + 1,
              ((resolution/2)-1 ) * resolution);

    /****** Faces *****/

    for (int i=0; i< (resolution/2)-2 ; i++){
        for(int j=1; j < resolution ; j++){
            m.addTopo(i*resolution + j, (i+1)*resolution +j, (i+1)*resolution +j+1);
            /*t.push_back( i*resolution + j );
            t.push_back( (i+1)*resolution +j );
            t.push_back( (i+1)*resolution + j + 1  ) ;*/

            m.addTopo((i+1)*resolution + j+1, i*resolution + j+1, i*resolution + j);
            /*t.push_back( (i+1)*resolution + j + 1 );
            t.push_back( (i)*resolution +j+1  );
            t.push_back( i*resolution + j ) ;*/
        }

        m.addTopo(i*resolution + resolution, (i+1)*resolution + resolution, (i+1)*resolution +1);
        /*t.push_back( i*resolution + resolution );
        t.push_back( (i+1)*resolution + resolution );
        t.push_back( (i+1)*resolution + 1  ) ;*/

        m.addTopo((i+1)*resolution + 1, i*resolution +1, i*resolution + resolution);
        /*t.push_back( (i+1)*resolution + 1 );
        t.push_back( (i)*resolution + 1  );
        t.push_back( i*resolution + resolution ) ;*/
    }

    m.normalsTriangles();
    return m;
}

/*******************InOut********************/


//std::pair<Eigen::Vector3f,Eigen::Vector3f> Mesh::calculBoite()
//{
    /*if(this->nbGeom() == 0)
        return 0;*/
    /*Eigen::Vector3f min(geom[0]), max(geom[0]);

    std::vector<Eigen::Vector3f>::const_iterator it = geom.begin();
    ++it;
    for(    ;  it != geom.end();  ++it)
    {
        //pas fini.
    }*/
//}

/*******************Normal******************/

void Mesh::normalsTriangles()
{
    if(!normalsPoints.empty())
        normalsPoints.clear();

    normalsPoints.reserve(nbTopo()/3);
    for(size_t i = 0;  i < nbTopo()/3; i++)
        normalsPoints.push_back(this->normalTriangle(i));
}

Eigen::Vector3f Mesh::normalTriangle(int i) const
{
    int i1 = topo[i*3];
    int i2 = topo[i*3+1],  i3 = topo[i*3+2];
    Eigen::Vector3f s1(geom[i2]-geom[i1]),
                    s2(geom[i3]-geom[i1]);

    Eigen::Vector3f c(s1.cross(s2));
    c.normalize();
    return c;
}

void Mesh::plan(int _longueur, int _largeur, int _nbPointLongueur, int _nbPointLargeur)
{
    geom.reserve(_nbPointLongueur*_nbPointLargeur);

    for(int j = 0;  j < _nbPointLongueur;   j++)
        for(int i = 0;  i < _nbPointLargeur;    i++)
            geom.push_back(Eigen::Vector3f(((float)i/(_nbPointLargeur-1))*_largeur, ((float)j/(_nbPointLongueur-1))*_longueur, 0));
    this->simpleInitTopo();
}

void Mesh::applicationNoise(int amplitude, int periode)
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


void Mesh::applicationRidge(float seuil, float amplitude, int periode)
{
    for(Eigen::Vector3f& p: geom)
    {
        float hRidge = amplitude * NoiseGenerator::perlinNoise( p(0)*(1.0/periode), p(2)*(1.0/periode) );
        hRidge += seuil;

        if(p(2) > hRidge)
            p(2) = 2*hRidge - p(2);
    }
}


void Mesh::applicationWarp(int amplitude, int periode)
{
    //for(size_t i = 0; i < geom.size(); i++)
    for(Eigen::Vector3f& p: geom)
    {
        float warp = amplitude * NoiseGenerator::perlinNoise( p(0)*(1.0/periode)+2.78, p(1)*(1.0/periode)+8.72);

        p(0) += warp;
        p(1) += warp;
    }
}

void Mesh::applicationSin(int amplitude, int periode)
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

void Mesh::simpleInitTopo()
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

void Mesh::calculNormals()
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
                    n += normalsPoints[t6];
                if(i > 0) {
                    n += normalsPoints[t6-1];
                    n += normalsPoints[t6-2];
                }
            }

            if(j > 0)
            {
                int t3 = t6-(nbPointLargeur-1)*2;   //p12:  t3 = 20 - 4*2 = 12;
                if(i < nbPointLargeur-1)
                    n += normalsPoints[t3];
                if(i > 0)                {
                    n += normalsPoints[t3-1];
                    n += normalsPoints[t3-2];
                }
            }

            normalsPoints.push_back(n.normalized());
        }
    }
}



float Mesh::getHauteur(float pointX, float pointY) const
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

float Mesh::getHauteur(Vector2f &pointXY) const
{
    return getHauteur(pointXY(0), pointXY(1));
}


Vector3f Mesh::getNormal(float pointX, float pointY) const
{
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

bool Mesh::intersectWithMesh(const Rayon& rayon, float &coeffDistance) const{

    float dmin = 0.0;
    float dmax = 3000.0;

    /*if(!englobant.intersect(rayon, dmin, dmax ))
        return false;
    */
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

void Mesh::generationTerrain(int width, int lenght, int nbPointLongueur, int nbPointLargeur)
{
    plan(lenght, width, nbPointLongueur, nbPointLargeur);
    applicationNoise(200, 1000);
    applicationRidge(150, 50, 500);

    applicationNoise(20, 100);
    applicationRidge(150, 50, 400);

    applicationNoise(5, 50);

    calculNormals();
    //englobant = Box(geom);
}

bool Mesh::inOut(const Eigen::Vector3f& pointXYZ)
{
    if(pointXYZ(2) > getHauteur(pointXYZ(0), pointXYZ(1))){
        return false;
    }
    return true;
}

/**construit un terrain avec le même nombre de point que le nombre de pixel de l'image*/
void Mesh::simpleInitImage(const QImage& img, float _longueur, float _largeur, float _amplitude)
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

#include "mesh.h"



Mesh::Mesh(const Terrain2 &terrain, int nbHeight, int nbWidth)
{
    geom.reserve(nbHeight*nbWidth);
    //faire un resize pour OpenMP et bien indiquer l'indice au lieu du push_back()

    for(int j = 0;  j < nbHeight;   j++)
    {
        float y = j/(float)(nbHeight-1);
        float y2 = y*terrain.longueur + terrain.box.min(1);
        for(int i = 0;  i < nbWidth;   i++)
        {
            float x = i/(float)(nbWidth-1);
            float x2 = x*terrain.largeur + terrain.box.min(0);
            float h = terrain.getHauteurXY(x, y);
            float h2 = h+terrain.box.min(2);

            geom.push_back(Vector3f(x2, y2, h2));
        }
    }
    this->simpleInitTopoTerrain(nbHeight, nbWidth);
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

/**********************************************************************************************/

void Mesh::applicationNoise(int amplitude, int periode)
{
    /*************************************/
    /* Modification points */
    /*************************************/

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



/**************************************************************/

void Mesh::simpleInitTopoTerrain(int nbHeight, int nbWidth)
{
    this->topo.reserve((nbHeight-1)*(nbWidth-1)*6); //grille 5p x 7p = 35p => 24 carrés (4*6) = 48 triangles = 144 int = (5-1) * (7-1) * 2 * 3

    for(int j = 0; j < nbHeight-1; j++){
        for(int i = 0; i < nbWidth-1; i++)
        {
            //triangle 1: 0,1,2
            addTopo(i + j * nbWidth,
                    (i+1) + j * nbWidth,
                    i + (j+1) * nbWidth);

            //triangle 2: 1,3,2
            addTopo((i+1) + j * nbWidth,
                    (i+1) + (j+1) * nbWidth,
                    i + (j+1) * nbWidth);
        }
    }
}

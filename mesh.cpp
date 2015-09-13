#include "mesh.h"

void Mesh::Translation(const Eigen::Vector3f T){
    for(int i=0; i<geom.size(); i++){
        geom.at(i)+=T;
    }
}

void Mesh::Translation(const float x, const float y, const float z)
{
    for(int i=0; i<geom.size(); i++){
        geom.at(i)(0)+=x;
        geom.at(i)(1)+=y;
        geom.at(i)(2)+=z;
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

    for(int i=0; i<geom.size(); i++){
        geom[i] = matriceRotation*geom[i];
    }
    /*for(std::vector<Eigen::Vector3f>::iterator it = geom.begin();   it != geom.end();   ++it)
        *it *= matriceRotation;*/
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
    std::vector<Eigen::Vector3f> v;
    std::vector<int> t;

    Eigen::Vector3f normal = centreCercleB - centreCercleA;

    Eigen::Vector3f p, axe1, axe2;

    axe1(0) = normal(1);
    axe1(1) = -normal(0);
    axe1(2) = 0;

    /*std::cout << "normal :" << normal << std::endl;

    std::cout << "axe1 :" << axe1 << std::endl;

    std::cout << "n.axe1 :" << normal.dot(axe1) << std::endl;*/

    axe2 = axe1.cross(normal);
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

        p = cos(i*ecartRadiant)*axe1 + sin(i*ecartRadiant)*axe2 + centreCercleA;

        //std::cout << p << std::endl;

        v.push_back( p );
    }

    for(int i=0; i<resolution; i++){
        v.push_back( v[i]+normal );
    }

    v.push_back(centreCercleA); // 2 resolution
    v.push_back(centreCercleB); // 2 resolution+1


    /*************************************/
    /* Génération faces */
    /*************************************/

    /***** Poles ****/
    for(int i=0; i<= resolution-1; i++){
        t.push_back(i); t.push_back(i+1); t.push_back(2*resolution);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(2*resolution);

    for(int i=resolution ; i<= 2*resolution-1; i++){
        t.push_back(i); t.push_back(i+1); t.push_back(2*resolution+1);
    }
    t.push_back(2*resolution-1); t.push_back(resolution); t.push_back(2*resolution+1);

    /***** faces cylindre *****/
    for(int i=0; i< resolution-1; i++){

        t.push_back(i); t.push_back(i+1); t.push_back(resolution+i);
        t.push_back(resolution+i+1); t.push_back(resolution+i); t.push_back(i+1);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(resolution);
    t.push_back(resolution); t.push_back(2*resolution-1); t.push_back(resolution-1);


    Mesh retour(v,t);

    return retour;
}


Mesh Mesh::cone(const Eigen::Vector3f &centreCercle, const Eigen::Vector3f &pointe, const float rayon, const int resolution)
{
    std::vector<Eigen::Vector3f> v;
    std::vector<int> t;

    Eigen::Vector3f normal = pointe - centreCercle;

    Eigen::Vector3f p, axe1, axe2;

    axe1(0) = normal(1);
    axe1(1) = -normal(0);
    axe1(2) = 0;

    /*std::cout << "normal :" << normal << std::endl;

    std::cout << "axe1 :" << axe1 << std::endl;

    std::cout << "n.axe1 :" << normal.dot(axe1) << std::endl;*/

    axe2 = axe1.cross(normal);
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

        p = cos(i*ecartRadiant)*axe1 + sin(i*ecartRadiant)*axe2 + centreCercle;

        //std::cout << p << std::endl;

        v.push_back( p );
    }

    v.push_back(centreCercle); // resolution
    v.push_back(pointe); // resolution+1


    /*************************************/
    /* Génération faces */
    /*************************************/

    /***** Poles ****/
    for(int i=0; i< resolution-1; i++){
        t.push_back(i); t.push_back(i+1); t.push_back(resolution);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(resolution);

    /***** faces cylindre *****/
    for(int i=0; i< resolution-1; i++){

        t.push_back(i); t.push_back(i+1); t.push_back(resolution+1);
    }
    t.push_back(resolution-1); t.push_back(0); t.push_back(resolution+1);


    Mesh retour(v,t);

    return retour;
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

    for(int i = 0 ; i < geom.size(); i++) {
        obj << "v " << std::setprecision(4) << geom[i].format(objFormat) << "\n";
    }

    obj << "#faces:\n";

    for(int i = 0 ; i < topo.size(); i += 3){
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

        for(int i=0; i< delta.geom.size(); i++){
            geom.push_back(delta.geom[i]);
        }

        topo.reserve(this->nbTopo()+delta.nbTopo());
        for(int i=0; i< delta.topo.size(); i++){
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
    std::vector<Eigen::Vector3f> v;
    std::vector<int> t;

    Eigen::Vector3f normalCercleOrigine(0,1,0), vecteurPoint1(1,0,0);
    normalCercleOrigine *= rayon; vecteurPoint1 *= rayon;

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

    tmp[0].reserve(resolution/2);
    for(int i=0; i<= (resolution/2); i++){
        p = cos(i*ecartRadiant)*vecteurPoint1+ sin(i*ecartRadiant)*normalCercleOrigine  + centre;

        //std::cout << p << std::endl;

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

        tmp[i].reserve((resolution/2));
        for(int j=0; j<= (resolution/2); j++){
            p = matriceRotation * tmp[0].at(j);

            //std::cout << p << std::endl;

            tmp[i].push_back( p );
        }
    }

    v.push_back((tmp[0]).at(0));

    for (int i=1; i< resolution/2 ; i++){
        for(int j=0; j< resolution ; j++){
            p = tmp[j].at(i);

            v.push_back(p);
        }
    }
    v.push_back(tmp[0].back()) ;


    //v.push_back(b); // 2n+1


    /*************************************/
    /* Génération faces */
    /*************************************/

    /***** Poles ****/

    for(int i=1; i< resolution; i++){
        t.push_back(0); t.push_back( i ); t.push_back( i+1 );
    }
    t.push_back(0); t.push_back(resolution); t.push_back( 1 );

    for(int i= ( (resolution/2)-2 )*resolution + 1 ; i< ( (resolution/2)-1 )*resolution; i++){
        t.push_back( i );
        t.push_back( ( (resolution/2)-1 )*resolution +1 );
        t.push_back( i+1 );
    }
    t.push_back( ( (resolution/2)-1 ) * resolution + 1 );
    t.push_back( ( (resolution/2)-2 ) * resolution + 1 );
    t.push_back( ( (resolution/2)-1 ) * resolution );


    /****** Faces *****/

    for (int i=0; i< (resolution/2)-2 ; i++){
        for(int j=1; j < resolution ; j++){

            t.push_back( i*resolution + j );
            t.push_back( (i+1)*resolution +j );
            t.push_back( (i+1)*resolution + j + 1  ) ;

            t.push_back( (i+1)*resolution + j + 1 );
            t.push_back( (i)*resolution +j+1  );
            t.push_back( i*resolution + j ) ;
        }

        t.push_back( i*resolution + resolution );
        t.push_back( (i+1)*resolution + resolution );
        t.push_back( (i+1)*resolution + 1  ) ;

        t.push_back( (i+1)*resolution + 1 );
        t.push_back( (i)*resolution + 1  );
        t.push_back( i*resolution + resolution ) ;
    }


    Mesh retour(v,t);

    return retour;
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

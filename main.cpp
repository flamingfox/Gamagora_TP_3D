#include <iostream>
#include <sstream>

#include "mesh.h"
//#include "arbre.h"
#include "terrain.h"
#include "camera.h"
//#include "generationvegetation.h" //inclue la librairie pour "srand" et "time"
#include "box.h"
#include "rayon.h"

#include <string>
#include <QImage>   //enlever -qt dans le fichier .pro pour faire marcher

using namespace Eigen;

void testTotal();
void testImage(const QImage& img);
void testSphere();
void testNormals();
void testBox();
void testCamera();
void testIntersect();

int main(int, char **)
{
    //testSphere();
    //testImage(QImage("image.png"));

    //testTotal();
    testCamera();
    //testBox();
    //testNormals();
    //testIntersect();

    return 0;
}

void testNormals()
{
    Terrain m = Terrain(150, 150, 5, 5,0);
    m.normalsTriangles();
    m.calculNormals();
    m.save("terrain.obj");

}

void testBox()
{
    Terrain m = Terrain(150, 150, 150, 150,0);
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

void testCamera(){
    Terrain m = Terrain(1000, 1000, 200, 200, 0);
    m.save("terrain.obj");
    //Terrain m2 = Terrain(1500, 1500, 250, 250, 1);
    //m.save("eau.obj");
    std::vector<Terrain*> t;
    t.push_back(&m);
    //t.push_back(&m2);

    Camera cam( Vector3f(0,300,1200), Vector3f(225,150,-500), 500, 800, 800, t);
    //cam.generateImage(500,500).save("test2.png");
    cam.rendu();

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

void testTotal()
{
    //srand(time(NULL));

    std::cout << "Hello !" << std::endl << "World generating !" << std::endl;

    Terrain m = Terrain(1500, 1500, 250, 250, 0);
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

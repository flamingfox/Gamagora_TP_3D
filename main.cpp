#include <iostream>
#include <sstream>

#include "mesh.h"
#include <Eigen/Core>
#include "arbre.h"
#include "terrain.h"
#include "generationvegetation.h"
#include <string>

#include <QImage>   //enlever -qt dans le fichier .pro pour faire marcher

void testTotal();
void testImage(const QImage& img);

int main(int argc, char *argv[])
{
    testImage(QImage("image.png"));

    //testTotal();
    return 0;
}

void testImage(const QImage& img)
{
    Terrain m = Terrain(img, 100, 100, 10);
    m.save("image.obj");
}

void testTotal()
{
    srand(time(NULL));

    std::cout << "Hello !" << std::endl << "World generating !" << std::endl;

    Terrain m = Terrain(Terrain::PLAT, 150, 150, 2, 0.6);
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
    m.rescale(5);

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

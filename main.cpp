#include <iostream>
#include <sstream>

#include "mesh.h"
//#include "arbre.h"

#include "terrain.h"
#include "terrain/terrainnoise.h"
//#include "camera.h"
//#include "generationvegetation.h" //inclue la librairie pour "srand" et "time"
//#include "box.h"
//#include "rayon.h"
#include "rendu/scene.h"

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
void testScene();

int main(int, char **)
{
    //testIntersect();
    testScene();
    return 0;
}

/***********************************************************************************************/

void testScene()
{
    TerrainNoise* noise = new TerrainNoise(100000,100000);
    //Mesh m(*noise, 200, 200);
    //m.save("terrainNoiseTest.obj");
    Camera* cam = new Camera(Vector3f(350,350,500), Vector3f(750,750,0), 600, 1200, 800);
                            //Vector3f(1000,1000,-100),Vector3f(900,900,-1000), 700, 1200, 800);

    Scene scene;
    scene.addO(noise);
    scene.addC(cam);
    Camera* cam2 = new Camera(Vector3f(1350,1350,500), Vector3f(1750,1750,0), 600, 1200, 800);
    scene.addC(cam2);
    Camera* cam3 = new Camera(Vector3f(4350,4350,500), Vector3f(4750,4750,0), 600, 1200, 800);
    scene.addC(cam3);
    Camera* cam4 = new Camera(Vector3f(99350,99350,500), Vector3f(99750,99750,0), 600, 1200, 800);
    scene.addC(cam4);
    Camera* cam5 = new Camera(Vector3f(-350,-350,1000), Vector3f(750,750,0), 700, 1200, 800);
    scene.addC(cam5);


    scene.rendu();
    delete noise;
    delete cam;
}

void testIntersect(){
    Box b = Box(Eigen::Vector3f(1,1,1), Eigen::Vector3f(3,3,3));
    Rayon r = Rayon(Eigen::Vector3f(4,4,4), Eigen::Vector3f(-1,-1,-1));

    float distanceMin = 0, distanceMax = 0;

    if(b.intersect(r, distanceMin, distanceMax))
        std::cout << "Intersection, dMin = " << distanceMin << " , dMax = " << distanceMax << std::endl;
    else
        std::cout << "Pas d'intersection" << std::endl;
}

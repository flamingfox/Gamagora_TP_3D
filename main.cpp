#include <iostream>
#include <sstream>

#include "mesh.h"
#include "terrain/terrainnoise.h"
#include "terrain/terraintab.h"
#include "rendu/scene.h"

#include <string>
#include <QImage>   //enlever -qt dans le fichier .pro pour faire marcher
#include <time.h>

using namespace Eigen;

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
    TerrainNoise* noise = new TerrainNoise(6000,6000);
    Camera* cam = new Camera(Vector3f(0,0,2000), Vector3f(2000,2000,0), 1200, 1200, 800);
    //TerrainTab* noise = new TerrainTab(QImage("./heightmap.jpg"), 2000,2000, 500);
    //Mesh m(*noise, 200, 200);
    //m.save("terrainNoiseTest.obj");

                            //Vector3f(1000,1000,-100),Vector3f(900,900,-1000), 700, 1200, 800);

    Scene scene;
    scene.addO(noise);
    //scene.addC(cam);
    Camera* cam2 = new Camera(Vector3f(-850,-850,700), Vector3f(1050,1050,0), 600, 1200, 800);
    scene.addC(cam2);
    /*Camera* cam3 = new Camera(Vector3f(4350,4350,500), Vector3f(4750,4750,0), 600, 1200, 800);
    scene.addC(cam3);
    Camera* cam4 = new Camera(Vector3f(99350,99350,500), Vector3f(99750,99750,0), 600, 1200, 800);
    scene.addC(cam4);
    Camera* cam5 = new Camera(Vector3f(-350,-350,1000), Vector3f(750,750,0), 700, 1200, 800);
    scene.addC(cam5);*/
    //scene.addParcoursCamera(noise);

    scene.rendu();
    delete noise;
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

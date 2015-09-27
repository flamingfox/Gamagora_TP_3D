#include <iostream>
#include <sstream>

#include "mesh.h"
//#include "arbre.h"

#include "terrain/terrainnoise.h"
#include "rendu/scene.h"

#include <string>
#include <QImage>   //enlever -qt dans le fichier .pro pour faire marcher
#include <time.h>

using namespace Eigen;

void testIntersect();
void testScene();

int main(int, char **)
{
    testScene();
    return 0;
}

/***********************************************************************************************/



void testScene()
{
    TerrainNoise* noise = new TerrainNoise(10000,10000);
    //Mesh m(*noise, 200, 200);
    //m.save("terrainNoiseTest.obj");
    Camera* cam = new Camera(Vector3f(350,350,500), Vector3f(750,750,0), 600, 1200, 800);
    Scene scene;
    scene.addO(noise);
    scene.addC(cam);

    scene.rendu();
    delete noise;
    delete cam;
}

/*void testIntersect(){
    Box b = Box(Eigen::Vector3f(1,1,1), Eigen::Vector3f(3,3,3));
    Rayon r = Rayon(Eigen::Vector3f(4,4,4), Eigen::Vector3f(-1,-1,-1));

    float distanceMin = 0, distanceMax = 0;

    if(b.intersect(r, distanceMin, distanceMax))
        std::cout << "Intersection, dMin = " << distanceMin << " , dMax = " << distanceMax << std::endl;
    else
        std::cout << "Pas d'intersection" << std::endl;
}*/

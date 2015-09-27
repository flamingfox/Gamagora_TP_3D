#include <iostream>
#include <sstream>

#include "mesh.h"
//#include "arbre.h"
#include "terrain/terrainnoise.h"
//#include "camera.h"
//#include "generationvegetation.h" //inclue la librairie pour "srand" et "time"
//#include "box.h"
//#include "rayon.h"
#include "rendu/scene.h"

#include <string>
#include <QImage>   //enlever -qt dans le fichier .pro pour faire marcher
#include <time.h>

using namespace Eigen;

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
    TerrainNoise* noise = new TerrainNoise(2000,2000);
    Camera* cam = new Camera(Vector3f(350,350,500), Vector3f(750,750,0), 600, 1200, 800);
                            //Vector3f(1000,1000,-100),Vector3f(900,900,-1000), 700, 1200, 800);

    Scene scene;
    scene.addO(noise);
    scene.addC(cam);

    scene.rendu();
    delete noise;
}

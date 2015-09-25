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

void testCamera();

int main(int, char **)
{
    testCamera();
    return 0;
}


void testCamera(){
    Terrain m = Terrain(1500, 1500, 250, 250);
    std::vector<Terrain*> t;
    t.push_back(&m);
    Camera cam( Vector3f(0,0, 1000), Vector3f(750, 750, -200), 700, 1200, 800, t);
    cam.rendu();
}




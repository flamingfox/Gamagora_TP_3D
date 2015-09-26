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
#include <time.h>

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

/**simule le parcours d'une camera sur le terrain de la scène*/
void addParcoursCamera(Scene& scene, TerrainNoise* noise)
{
    int t = 10000;
    int x = NoiseGenerator::perlinNoiseGradiant(rand()%t,rand()%t,rand()%t)*t;
    int y = NoiseGenerator::perlinNoiseGradiant(rand()%t,rand()%t,rand()%t)*t;

    int dirMax = 10;
    int dMax = 2;
    Vector2f dir(NoiseGenerator::perlinNoiseGradiant(rand()%dirMax,rand()%dirMax,rand()%dirMax)*dirMax,
                 NoiseGenerator::perlinNoiseGradiant(rand()%dirMax,rand()%dirMax,rand()%dirMax)*dirMax);
    if(dir.norm() > dirMax){
        dir.normalize();
        dir *= dirMax;
    }
    Vector2f dir2(NoiseGenerator::perlinNoiseGradiant(rand()%dirMax,rand()%dirMax,1+rand()%(dirMax-1))*dirMax,
                  NoiseGenerator::perlinNoiseGradiant(rand()%dirMax,rand()%dirMax,1+rand()%(dirMax-1))*dirMax);
    if(dir2.norm() > dirMax){
        dir2.normalize();
        dir2 *= dirMax;
    }

    for(int i = 0;  i < 900;    i++)    {
        Vector2f dev(NoiseGenerator::perlinNoiseGradiant2(rand()%16,rand()%16,1+rand()%(16))*dMax,
                     NoiseGenerator::perlinNoiseGradiant2(rand()%16,rand()%16,1+rand()%(16))*dMax);

        if(dev.norm() > dMax)   {
            dev.normalize();
            dev *= dMax;
        }
        dir += dev;
        if(dir.norm() > dirMax) {
            dir.normalize();
            dir *= dirMax;
        }
        x += dir(0);
        y += dir(1);

        float x2 = x + dir(0),
              y2 = y + dir(1);

        Camera* cam = new Camera(Vector3f(x,y,noise->getHauteur(x,y)+5), Vector3f(x2,y2,noise->getHauteur(x2,y2)+5), 500, 1280, 720);
        scene.addC(cam);
    }
}

void testScene()
{
    TerrainNoise* noise = new TerrainNoise(100000,100000);
    //Mesh m(*noise, 200, 200);
    //m.save("terrainNoiseTest.obj");
    //Camera* cam = new Camera(Vector3f(350,350,500), Vector3f(750,750,0), 600, 1200, 800);
                            //Vector3f(1000,1000,-100),Vector3f(900,900,-1000), 700, 1200, 800);

    Scene scene;
    scene.addO(noise);
    /*scene.addC(cam);
    Camera* cam2 = new Camera(Vector3f(1350,1350,500), Vector3f(1750,1750,0), 600, 1200, 800);
    scene.addC(cam2);
    Camera* cam3 = new Camera(Vector3f(4350,4350,500), Vector3f(4750,4750,0), 600, 1200, 800);
    scene.addC(cam3);
    Camera* cam4 = new Camera(Vector3f(99350,99350,500), Vector3f(99750,99750,0), 600, 1200, 800);
    scene.addC(cam4);
    Camera* cam5 = new Camera(Vector3f(-350,-350,1000), Vector3f(750,750,0), 700, 1200, 800);
    scene.addC(cam5);*/
    addParcoursCamera(scene, noise);

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

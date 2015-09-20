#ifndef SCENE_H
#define SCENE_H

#include "./mesh/terrain/terrain.h"
#include "camera.h"
#include "lumiere/lumiere.h"

/**
    @author Aurelien Argoud
*/
class Scene
{
public:


    Scene();

    void addT(Terrain* t){
        terrains.push_back(t);
    }
    void addM(Mesh* m){
        meshs.push_back(m);
    }

    void addL(Lumiere* l){
        lights.push_back(l);
    }

    void addC(Camera* c){
        cameras.push_back(c);
    }

    bool rendu();
    QColor render(const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray);



private:
    std::vector<Terrain*> terrains;//liste des objets dans la scene
    std::vector<Mesh*> meshs;
    std::vector<Lumiere*> lights;

    std::vector<Camera*> cameras;


    const QColor default_color = QColor(116, 208, 241);
};

#endif // SCENE_H

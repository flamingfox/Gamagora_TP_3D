#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "./terrain/terrain.h"

/**
    @author Aurelien Argoud
*/
class Scene
{
public:


    Scene();

    void addO(Terrain* obj){
        objects.push_back(obj);
    }

    void addC(Camera* c){
        cameras.push_back(c);
    }

    bool rendu();
    QColor render(const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray);

    void addParcoursCamera(Terrain* noise);



private:
    std::vector<Terrain*> objects;//liste des objets dans la scene
    std::vector<Camera*> cameras;


    const QColor default_color = QColor(116, 208, 241); //couleur si aucun objet touché
};

#endif // SCENE_H

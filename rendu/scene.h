#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "./lumiere/lumiere.h"
#include "./object.h"
#include "./terrain/terrain2.h"

/**
    @author Aurelien Argoud
*/
class Scene
{
public:


    Scene();

    void addO(Object* obj){
        objects.push_back(obj);
    }
    void addL(Lumiere* l){
        lights.push_back(l);
    }
    void addC(Camera* c){
        cameras.push_back(c);
    }

    bool rendu();
    QColor render(const Eigen::Vector3f& pointImpact, const Object& objleplusproche, const Rayon& ray);

    void addParcoursCamera(Terrain2* noise);



private:
    std::vector<Object*> objects;//liste des objets dans la scene
    std::vector<Lumiere*> lights;
    std::vector<Camera*> cameras;


    const QColor default_color = QColor(116, 208, 241); //couleur si aucun objet touché
};

#endif // SCENE_H

#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "./terrain/terrain.h"

/**
    @author Aurelien Argoud
    @brief Correspond à un scène dans sa globalité.
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

    /**
     * @brief Créer la visualisation de la scène dans une image. Utilise le procédé de lancé de rayons. \n
     * Utilise render().
     * @return Si le rendu c'est bien déroulé.
     */
    bool rendu();

    void addParcoursCamera(Terrain* noise);

private:

    /**
     * @brief Liste des objets dans la scène.
     */
    std::vector<Terrain*> objects;

    /**
     * @brief Liste des caméras dans la scène.
     */
    std::vector<Camera*> cameras;

    /**
     * @brief Couleur de fond
     */
    const QColor default_color = QColor(116, 208, 241);


    /**
     * @brief Détermine la couleur d'un pixel.
     * @param pointImpact Position d'un point d'impact entre un rayon (ray) et un objet (objleplusproche).
     * @param objleplusproche Premier objet touché par le rayon ray. (cf. Object)
     * @param ray un rayon. (cf. Rayon)
     * @return
     */
    QColor render(const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray);


};

#endif // SCENE_H

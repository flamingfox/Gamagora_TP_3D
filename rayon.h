#ifndef RAYON_H
#define RAYON_H
#include <Eigen/Core>
#include "mesh.h"

class Rayon
{
protected:
    Eigen::Vector3f _origine; //position du rayon
    Eigen::Vector3f _direction; //direction du rayon
public:
    Rayon();

    void Reflect();
    Eigen::Vector3f getOrigine();
    Eigen::Vector3f getDirection();


};

#endif // RAYON_H

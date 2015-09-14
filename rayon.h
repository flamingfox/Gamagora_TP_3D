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
    Rayon(Eigen::Vector3f or,Eigen::Vector3f dr);

signals:

public slots:
};

#endif // RAYON_H

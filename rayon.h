#ifndef RAYON_H
#define RAYON_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <math.h>

class Rayon
{
protected:
    Eigen::Vector3f _origine; //position du rayon
    Eigen::Vector3f _direction; //direction du rayon
public:
    Rayon();

    Eigen::Vector3f Reflect(Eigen::Vector3f normal);

    inline Eigen::Vector3f getOrigine()const{return _origine;}
    inline Eigen::Vector3f getDirection()const{return _direction;}

};

#endif // RAYON_H

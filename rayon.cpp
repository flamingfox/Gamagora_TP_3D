#include "rayon.h"

Rayon::Rayon()
{

}

Rayon::Rayon(Eigen::Vector3f origine, Eigen::Vector3f direction){
    _origine = origine;
    _direction = direction;
}

Eigen::Vector3f Rayon::Reflect(Eigen::Vector3f normal){
    const double cosI = normal.dot(_direction);
    return _direction + 2*cosI * normal;
}

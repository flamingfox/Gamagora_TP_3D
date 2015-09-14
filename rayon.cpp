#include "rayon.h"

Rayon::Rayon()
{

}

Eigen::Vector3f Rayon::getOrigine() const{
    return _origine;
}

Eigen::Vector3f Rayon::getDirection() const{
   return _direction;
}


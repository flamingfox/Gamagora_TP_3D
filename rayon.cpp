#include "rayon.h"

Rayon::Rayon()
{

}

Rayon::Rayon(const Eigen::Vector3f& origine, const Eigen::Vector3f& direction):
        _origine(origine),  _direction(direction)
{
}

Eigen::Vector3f Rayon::Reflect(const Eigen::Vector3f& normal) {
    const double cosI = normal.dot(_direction);
    return _direction + 2*cosI * normal;
}

#include "lumiere.h"

Lumiere::Lumiere():
        Lumiere(Eigen::Vector3f(0,0,0))
{
}

Lumiere::Lumiere(const Eigen::Vector3f& _origine, float _rayon, float _puissance):
        origine(_origine),  rayon(_rayon),  puissance(_puissance)
{
}


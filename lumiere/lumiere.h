#ifndef LUMIERE_H
#define LUMIERE_H

#include <Eigen/Geometry>

#include "phong.h"

/**
    @author Aurelien Argoud
*/
class Lumiere
{
public:
    Eigen::Vector3f origine;
    float rayon;        //rayon d'action de la lumière
    float puissance;    //puissance de la lumière qui diminue en fonction du rayon.

    Lumiere();
    Lumiere(const Eigen::Vector3f& _origine, float _rayon = 500.f, float _puissance = 1.f);

    Phong phong = {0.2f, 0.9f, 0.8f, 100};
};

#endif // LUMIERE_H

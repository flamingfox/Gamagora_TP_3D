#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <Eigen/Core>
#include <QList>
#include <rayon.h>
#include <terrain.h>
#include "float.h"  //FLT_MIN FLT_MAX

class Camera
{
protected:
    Eigen::Vector3f _origine; //position de la camera
    Eigen::Vector3f _u, _v, _w;
    int _lu, _lv;
    float _lw; //lv et lw constants    exemple : lu = 16.0 / 20.0
                         //                                lv = 9.0 / 20.0
    QList<Terrain> _t;
    // un vecteur u, un vecteur v et un vecteur w  (u,v,w)
    // une distance dw
    // une distance lu et lv
public:
    Camera();
    Camera(Eigen::Vector3f pOr, Eigen::Vector3f pAt, int l, int h);

    Eigen::Vector3f vecScreen(int &i, int &j) const;
    Eigen::Vector3f pointScreen(int &i, int &j) const;

    bool rendu();

};

#endif // CAMERA_H

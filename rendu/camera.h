#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <Eigen/Core>
#include <QList>
#include <terrain/terrain.h>
#include "float.h"  //FLT_MIN FLT_MAX
#include <QColor>
#include <QDebug>
#include <QLinearGradient>
#include <QBrush>

using namespace Eigen;

class Camera
{
protected:
    Vector3f _origine; //position de la camera
    Vector3f _u, _v, _w;
    int _lu, _lv;
    float _lw; //lv et lw constants    exemple : lu = 16.0 / 20.0
                         //                                lv = 9.0 / 20.0
    // un vecteur u, un vecteur v et un vecteur w  (u,v,w)
    // une distance dw
    // une distance lu et lv


public:
    Camera();
    Camera(const Vector3f& pOr, const Vector3f& pAt, const float& distance, int l, int h);
    //Camera(const Vector3f& pOr, const Vector3f& vAtUnit, const float& distance, int l, int h, const std::vector<Terrain*>& listTerrain);

    Vector3f vecScreen(int i, int j) const;
    Vector3f pointScreen(int i, int j) const;

    int getLu() const{
        return _lu;
    }
    int getLv() const{
        return _lv;
    }
    Vector3f getOrigine() const{
        return _origine;
    }

};
#endif // CAMERA_H

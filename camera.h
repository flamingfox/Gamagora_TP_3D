#ifndef CAMERA_H
#define CAMERA_H
#include <Eigen/Core>

class Camera
{
protected:
    Eigen::Vector3f _origine; //position de la camera
    Eigen::Vector3f _u, _v, _w;
    float _lu, _lv, _lw; //lv et lw constants    exemple : lu = 16.0 / 20.0
                         //                                lv = 9.0 / 20.0
    // un vecteur u, un vecteur v et un vecteur w  (u,v,w)
    // une distance dw
    // une distance lu et lv
public:
    Camera();

signals:

public slots:
};

#endif // CAMERA_H

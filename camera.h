#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <Eigen/Core>
#include <QList>
#include <rayon.h>
#include <terrain.h>
#include "float.h"  //FLT_MIN FLT_MAX
#include <QColor>
#include <QDebug>

using namespace Eigen;

class Camera
{
protected:
    Vector3f _origine; //position de la camera
    Vector3f _u, _v, _w;
    int _lu, _lv;
    float _lw; //lv et lw constants    exemple : lu = 16.0 / 20.0
                         //                                lv = 9.0 / 20.0
    QList<Terrain> _t;
    // un vecteur u, un vecteur v et un vecteur w  (u,v,w)
    // une distance dw
    // une distance lu et lv
public:
    Camera();
    Camera(Vector3f pOr,Vector3f pAt, int l, int h);

    Vector3f vecScreen(int &i, int &j) const;
    Vector3f pointScreen(int &i, int &j) const;

    bool SetPixel(QImage *img, int x, int y, QColor color);//permet de mettre une couleur au pixel de coordonnées x,y

    bool rendu();
    QImage generateImage(int largeur, int hauteur);//fonction de test à ne pas utiliser


};
#endif // CAMERA_H
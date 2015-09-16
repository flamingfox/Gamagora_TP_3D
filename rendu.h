#ifndef RENDU_H
#define RENDU_H

#include <QImage>
#include <Eigen/Geometry>
#include "rayon.h"
#include "camera.h"


QImage renduImage(int nbWidth, int nbHeight)
{
    QImage img(nbWidth, nbHeight);
    for(int j = 0;  j < nbHeight;   j++)    {
        for(int i = 0;  i < nbWidth;    i++)    {
            //Eigen::Vector3i color;

            //if()
        }
    }

}



QColor renduHors(int x, int y)
{
    Qt::blue;
}

QColor rendu(const Eigen::Vector3f& ray, const Eigen::Vector3f& n)
{
    Eigen::Vector3f diff = ray.normalized() - n;
    double norm = diff.squareNorm();    //si le rayon va dans le sens inverse de la normal du triangle qu'il touche,
    norm = 4-norm;

    qRgb color;
    if(norm >= 2)
        color = Qt::black;
    else if(norm == 0)
        color = Qt::white;
    else
    {
        int c = 255*(norm/2);
        color = qRgb(c, c, c);
    }
    return color;
}


QColor rendu(const Rayon& ray, const Eigen::Vector3f& n)
{
    return rendu(ray.getDirection(), n);
}



#endif // RENDU_H

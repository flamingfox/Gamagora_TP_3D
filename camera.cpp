#include "camera.h"

Camera::Camera()
{

}

Camera::Camera(Eigen::Vector3f pOr, Eigen::Vector3f pAt, int l, int h) :
    _origine(pOr), _lu(l/2), _lv(h/2)
{
    _w = pAt - pOr;

    _lw = _w.norm();

    _w.normalize();

    _u = - ( _w.cross(Eigen::Vector3f(0,0,1)) );
    _u.normalize();

    _v = _w.cross(_u);
    _v.normalize();
}

Eigen::Vector3f Camera::vecScreen(int& i, int& j) const
{
    if(i >= _lu*2 || j >= _lv*2){
        std::cerr << "i or j is incorrect" << std::endl;
        exit(-1);
    }

    float ti = i / (_lu*2-1);
    float tj = j / (_lv*2-1);


    return _w*_lw + ( (1 - ti)*(-_lu)+(ti*_lu) )*_u + ( (1-tj)*(-_lv)+(tj*_lv) )*_v;
}

Eigen::Vector3f Camera::pointScreen(int &i, int &j) const
{
    return _origine + vecScreen(i, j);
}


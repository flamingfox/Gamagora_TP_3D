#include "camera.h"

Camera::Camera()
{

}

/**
@pOr origine de la caméra
@pAt vecteur direction de la caméra (comprenant distance)
*/
Camera::Camera(const Vector3f& pOr, const Vector3f& pAt, int l, int h) :
    _origine(pOr), _lu(l), _lv(h)
{
    _w = pAt.normalized();

    _lw = pAt.norm();

    if(_w == Vector3f(0,0,1) || _w == Vector3f(0,0,-1))
    {
        _u = Vector3f(_w(2),0,0);
        _v = Vector3f(0,_w(2),0);
    }
    else
    {
        _u = - ( _w.cross(Vector3f(0,0,1)) );
        _u.normalize();

        _v = _w.cross(_u);
        _v.normalize();
    }
}

Vector3f Camera::vecScreen(int i, int j) const
{
    if(i >= _lu || j >= _lv){
        std::cerr << "i or j is incorrect" << std::endl;
        exit(-1);
    }

    float ti = ((float)i) / (_lu - 1);
    float tj = ((float)j) / (_lv - 1);


    return _w*_lw + ( (1.0 - ti)*(-_lu/2)+(ti*_lu/2) )*_u + ( (1.0-tj)*(_lv/2)+(tj*(-_lv/2)) )*_v;
}

Vector3f Camera::pointScreen(int i, int j) const
{
    return _origine + vecScreen(i, j);
}

/*bool Camera::SetPixel(QImage *img, int x, int y, const QColor& color){
    //qDebug()<<color;
    img->setPixel(x,y,color.rgba());

    return true;
}

int Camera::getrouge(const QRgb& couleur){
    QColor tmp(QColor::fromRgb(couleur));
    return tmp.red();
}




QImage Camera::generateImage(int largeur, int hauteur){
    QImage img(largeur, hauteur, QImage::Format_RGB888);
    img.fill(QColor(Qt::white).rgb());
    for (int x = 0; x < 100; ++x) {
        for (int y = 0; y < 100; ++y) {
          img.setPixel(x, y, qRgb(rand()%255,rand()%255,rand()%255));
        }
    }
    return img;
}*/




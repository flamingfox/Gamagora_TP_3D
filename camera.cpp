#include "camera.h"

Camera::Camera()
{

}

Camera::Camera(Vector3f pOr, Vector3f pAt, int l, int h) :
    _origine(pOr), _lu(l/2), _lv(h/2)
{
    _w = pAt - pOr;

    _lw = _w.norm();

    _w.normalize();

    _u = - ( _w.cross(Vector3f(0,0,1)) );
    _u.normalize();

    _v = _w.cross(_u);
    _v.normalize();
}

Vector3f Camera::vecScreen(int& i, int& j) const
{
    if(i >= _lu*2 || j >= _lv*2){
        std::cerr << "i or j is incorrect" << std::endl;
        exit(-1);
    }

    float ti = i / (_lu*2-1);
    float tj = j / (_lv*2-1);


    return _w*_lw + ( (1 - ti)*(-_lu)+(ti*_lu) )*_u + ( (1-tj)*(-_lv)+(tj*_lv) )*_v;
}

Vector3f Camera::pointScreen(int &i, int &j) const
{
    return _origine + vecScreen(i, j);
}

bool Camera::SetPixel(QImage *img, int x, int y, QColor color){
    //qDebug()<<color;
    img->setPixel(x,y,color.rgb());
    return true;
}

bool Camera::rendu(){
    QImage *img = new QImage(_lu, _lv, QImage::Format_RGB888);
    img->fill(QColor(Qt::white).rgb());
    for(int x = 0; x < _lu ; x++){
        for(int y = 0; y < _lv ; y++){
            Rayon r(_origine,Vector3f(0,0,0));
            float coefdisttmp = FLT_MAX;
            float coefdistfinal = FLT_MAX;
            Vector3f zonetouchee;
            Terrain objleplusproche;
            r.setDirection(vecScreen(x,y));
            for(int z=0;z<_t.size();z++){
                /*if(_t.at(z).interesct(r,&coefdisttmp)){//si on touche
                    if(coefdisttmp < coefdistfinal){//on sélectionne l'objet touché le plus proche
                        coefdistfinal = coefdisttmp;
                        objleplusproche = _t.at(z);
                        zonetouchee = _origine + (coefdistfinal*_w);
                    }
                }*/
            }
            //RENDU::rendu(x,y,zonetouchee,objleplusproche,r);
            //qDebug()<<x;
            SetPixel(img, x, y,QColor(rand()%255,rand()%255,rand()%255));
        }
    }
    img->save("test.png");
    return true;
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
}




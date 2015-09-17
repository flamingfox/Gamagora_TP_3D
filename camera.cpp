#include "camera.h"

Camera::Camera()
{

}

/**
@pOr origine de la caméra
@pAt point à regarder avec la caméra
*/
Camera::Camera(const Vector3f& pOr, const Vector3f& pAt, int l, int h, const std::vector<Terrain*>& listTerrain) :
    _origine(pOr), _lu(l/2), _lv(h/2), _t(listTerrain)
{
    _w = pAt - pOr;

    _lw = _w.norm();
    _w.normalize();

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
    if(i >= _lu*2 || j >= _lv*2){
        std::cerr << "i or j is incorrect" << std::endl;
        exit(-1);
    }

    float ti = ((float)i) / (_lu*2 - 1);
    float tj = ((float)j) / (_lv*2 - 1);


    return _w*_lw + ( (1.0 - ti)*(-_lu)+(ti*_lu) )*_u + ( (1.0-tj)*(-_lv)+(tj*_lv) )*_v;
}

Vector3f Camera::pointScreen(int i, int j) const
{
    return _origine + vecScreen(i, j);
}

bool Camera::SetPixel(QImage *img, int x, int y, const QColor& color){
    //qDebug()<<color;
    img->setPixel(x,y,color.rgba());

    return true;
}

int Camera::getrouge(const QRgb& couleur){
    QColor tmp(QColor::fromRgb(couleur));
    return tmp.red();
}

QImage* Camera::antialiasing(QImage *img){
    QImage *img2 = new QImage(_lu, _lv, QImage::Format_RGB888);
    img2->fill(QColor(Qt::white).rgb());
    for(int i=1;i<img->width()-1;i++){
        for(int y=1;y<img->height()-1;y++){
            int newcolor;
            newcolor = getrouge(img->pixel(i,y))-getrouge(img->pixel(i-1,y-1))/100-getrouge(img->pixel(i,y-1))/100-getrouge(img->pixel(i+1,y-1))/100
                                           -getrouge(img->pixel(i-1,y))/100-getrouge(img->pixel(i+1,y))/100
                                           -getrouge(img->pixel(i-1,y+1))/100-getrouge(img->pixel(i,y+1))/100-getrouge(img->pixel(i+1,y+1))/100;
            if(newcolor<0)newcolor = 0;
            QColor tutu(newcolor,newcolor,newcolor);
            img2->setPixel(i,y,tutu.rgba());
        }
    }
    return img2;
}

bool Camera::rendu(){
    QImage *img = new QImage(_lu, _lv, QImage::Format_RGB888);
    img->fill(QColor(Qt::white).rgb());
    for(int x = 0; x < _lu ; x++){
        for(int y = 0; y < _lv ; y++){
            bool touche = false;
            Rayon r(_origine,Vector3f(0,0,0));
            float coefdisttmp = FLT_MAX;
            float coefdistfinal = FLT_MAX;
            Vector3f zonetouchee;
            const Terrain* objleplusproche;
            r.setDirection(vecScreen(x,y));
            for(const Terrain* terrain: _t){
                if(terrain->intersect(r,coefdisttmp)){//si on touche
                    touche = true;
                    if(coefdisttmp < coefdistfinal){//on sélectionne l'objet touché le plus proche
                        coefdistfinal = coefdisttmp;
                        objleplusproche = terrain;
                        zonetouchee = _origine + (coefdistfinal*_w);
                    }
                }
            }
            //QColor render(bool toucheoupas, zonetouchee,objleplusproche,r);
            //qDebug()<<x;
            //uint gris = rand()%255;
            //uint alpha = rand()%255;
            //QColor toto(gris,gris,gris,alpha);
            //SetPixel(img, x, y,toto);
            SetPixel(img, x, y, render(touche, zonetouchee, *objleplusproche, r));
        }
    }
    antialiasing(img)->save("testaliasing.png");
    img->save("test.png");
    delete img;
    return true;
}

QColor Camera::renderHors()
{
    return QColor(116, 208, 241);
}

QColor Camera::render(const bool toucheoupas, const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray)
{
    if(!toucheoupas)
        return renderHors();

    Eigen::Vector3f dRay = ray.getDirection();
    dRay.normalize();

    Eigen::Vector3f n = objleplusproche.getNormal(pointImpact);

    Eigen::Vector3f diff = dRay - n;
    double norm = diff.squaredNorm();    //si le rayon va dans le sens inverse de la normal du triangle qu'il touche,
    norm = 4-norm;
    //qDebug()<<norm;
    QColor color;
    if(norm >= 2)
        color = QColor(0,0,0); //Black
    else if(norm == 0)
        color = QColor(255,255,255); // White
    else
    {
        int c = 255-round((255*norm)/2);
        color = QColor(c, c, c); // Grey
    }
    return color;
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




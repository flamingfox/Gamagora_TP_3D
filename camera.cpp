#include "camera.h"

Camera::Camera()
{

}

/**
@pOr origine de la caméra
@pAt vecteur direction de la caméra (comprenant distance)
*/

Camera::Camera(const Vector3f &pOr, const Vector3f &pAt, const float &distance, int l, int h, const std::vector<Terrain *> &listTerrain) :
    _origine(pOr), _lu(l), _lv(h), _t(listTerrain), _lw(distance)
{
    Eigen::Vector3f tmp(pAt - pOr);
    _w = tmp.normalized();

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
    heatMapGradient.createDefaultHeatMapGradient();
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

bool Camera::SetPixel(QImage *img, int x, int y, const QColor& color){
    //qDebug()<<color;
    img->setPixel(x,y,color.rgba());

    return true;
}

int Camera::getrouge(const QRgb& couleur){
    QColor tmp(QColor::fromRgb(couleur));
    return tmp.red();
}

///
/// \brief Camera::rendu
/// \return
///
bool Camera::rendu(){
    QImage eric(_lu, _lv, QImage::Format_RGB888);
    QImage *img = new QImage(_lu, _lv, QImage::Format_RGB888);
    #pragma omp parallel for schedule(dynamic,1)
    for(int x = 0; x < _lu ; x++){ // pour chaque pixel de l'image
        std::cerr << "\rRendering: " << 100 * x / (_lu - 1) << "%"; // barre de progression
        for(int y = 0; y < _lv ; y++){
            bool touche = false;
            Rayon r(_origine,vecScreen(x,y));   //rayon correspondant au pixel
            float coefdisttmp = FLT_MAX;        //variables pour déterminer la distance du rayon le plus court
            float coefdistfinal = FLT_MAX;      //
            Vector3f zonetouchee;
            const Terrain* objleplusproche;
            int tmp;
            for(const Terrain* terrain: _t){ //parcours les terrains de la scene
                if(terrain->intersectWithFunction(r,coefdisttmp, tmp)){//si on touche
                    touche = true;
                    if(coefdisttmp < coefdistfinal){//on sélectionne l'objet touché le plus proche
                        coefdistfinal = coefdisttmp;
                        objleplusproche = terrain;
                        zonetouchee = _origine + (coefdistfinal*r.getDirection());
                    }
                }
            }
            SetPixel(img, x, y, render(touche, zonetouchee, *objleplusproche, r));
            eric.setPixel(x,y,qRgb(tmp,tmp,tmp));
        }
    }
    img->save("test.png");  eric.save("eric.png");
    delete img;
    return true;
}

QColor Camera::renderHors()
{
    return QColor(116, 208, 241);
}

QColor Camera::render(const bool toucheoupas, const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray)
{
    if(!toucheoupas)return renderHors();

    Eigen::Vector3f dRay = ray.getDirection();
    dRay.normalize();

    Eigen::Vector3f n = objleplusproche.getNormal(pointImpact(0),pointImpact(1));

    Eigen::Vector3f diff = dRay - n;
    double norm = diff.squaredNorm();    //si le rayon va dans le sens inverse de la normal du triangle qu'il touche,
    norm = 4-norm;
    QColor color;

    float hauteur = objleplusproche.getHauteur(pointImpact(0),pointImpact(1));
    hauteur = (hauteur/objleplusproche.getElevationMax());

    float r,g,b;
    heatMapGradient.getColorAtValue(hauteur, r,g,b);

    color.setRed(r*255);
    color.setGreen(g*255);
    color.setBlue(b*255);

    if(norm >= 2)
        color = QColor(0,0,0); //Black
    else if(norm == 0)
            color = QColor(255,255,255); // White

    else
    {
            int c = 255-round((255*norm)/2);
            color = QColor(color.red()*c/255,color.green()*c/255, color.blue()*c/255); // Grey
    }
    return color;
}





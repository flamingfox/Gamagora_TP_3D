﻿#include "scene.h"


Scene::Scene()
{
}


/*QImage* Scene::antialiasing(QImage *img){
    QImage *img2 = new QImage(img, _lv, QImage::Format_RGB888);
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
}*/

QColor sommeQColor(const QColor& c1, int tmp)
{
    float r = c1.red()+tmp,
          v = c1.green()+tmp,
          b = c1.blue()+tmp;
    if(r > 255) r = 255;
    if(v > 255) v = 255;
    if(b > 255) b = 255;
    return QColor(r,v,b);
}

bool Scene::rendu(){
    QTime timer;
    int nbpixrouge =0;
    for(unsigned int ic = 0;  ic < cameras.size(); ic++)
    {
        Camera* c = cameras[ic];
        int _lu = c->getLu(), _lv = c->getLv();
        int pourcent2 = -1;
        QImage *img = new QImage(_lu, _lv, QImage::Format_RGB888);
        QImage eric(_lu, _lv, QImage::Format_RGB888);
        timer.start();
        #pragma omp parallel for schedule(dynamic,1)
        for(int y = 0; y < _lv ; y++){      // pour chaque ligne de l'image
            int pourcent = 100 * y / (_lv - 1);
            if(pourcent != pourcent2)            {
                pourcent2 = pourcent;
                std::cout << "\r" << ic << " Rendering: " << pourcent << "% ";  // barre de progression
            }
            for(int x = 0; x < _lu ; x++){  // pour chaque pixel de la ligne
                Rayon r(c->getOrigine(),c->vecScreen(x,y));   //rayon correspondant au pixel
                int tmp = 0;

                bool touche = false, toucheBox = true;
                float coefdisttmp = FLT_MAX;        //variables pour déterminer la distance du rayon le plus court
                float coefdistfinal = FLT_MAX;
                Vector3f zonetouchee;
                const Terrain* objleplusproche = nullptr;

                for(const Terrain* obj: objects){    //parcours tous les objets de la scene
                    float tmp1, tmp2;
                    if(!obj->box.intersect(r,tmp1,tmp2))
                        toucheBox = false;
                    else if(obj->intersect(r,coefdisttmp, tmp)){//si on touche
                        touche = true;
                        if(coefdisttmp < coefdistfinal){//on sélectionne l'objet touché le plus proche
                            coefdistfinal = coefdisttmp;
                            objleplusproche = obj;
                            zonetouchee = c->getOrigine() + (coefdistfinal*r.getDirection());
                        }
                    }
                }

                if(!toucheBox){
                     img->setPixel(x, y, (QColor(255, 0, 0)).rgba());
                     nbpixrouge++;
                }
                else if(!touche)
                    img->setPixel(x, y, default_color.rgba());
                else
                {
                    img->setPixel(x, y, sommeQColor(render(zonetouchee, *objleplusproche, r),tmp*2).rgba());
                    float r,v,b;
                    ColorGradient grad;
                    grad.createDefaultHeatMapGradient();
                    grad.getColorAtValue((float)tmp/50.0f,r,v,b);
                    eric.setPixel(x,y,qRgb(r*255,v*255,b*255));
                }
            }
        }
        if(ic<10)        {
            std::cout << std::endl;
            int time = timer.elapsed();
            std::cout << time/1000 << "." << time%1000<< " secondes" << std::endl;
            std::cout << (100.f*nbpixrouge) / (_lu*_lv) << "%" << std::endl;
            img->save(("test000" + std::to_string(ic) + ".png").c_str());
            eric.save(("eric" + std::to_string(ic) + ".png").c_str());


            std::cout << ("test000" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else if(ic<100)        {
            img->save(("test00" + std::to_string(ic) + ".png").c_str());
            std::cout << ("test00" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else if(ic<1000)        {
            img->save(("test0" + std::to_string(ic) + ".png").c_str());
            std::cout << ("test0" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else    {
            img->save(("test" + std::to_string(ic) + ".png").c_str());
            std::cout << ("test" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        delete img;
    }
    return true;
}


//possibilité d'amélioration:
//placer dans terrain pour plus tard, et faire un render pour les classes Boite, Sphere, Cylindre, Cone, ...
//demande aussi de passer la scène en argument (lumières, objets pouvant s'interposer avec les lumières)
QColor Scene::render(const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray)
{
    Eigen::Vector3f dRay = ray.getDirection();
    dRay.normalize();

    Eigen::Vector3f n = objleplusproche.getNormal(pointImpact(0),pointImpact(1));   //la normal pointe vers le haut

    //Eigen::Vector3f diff = dRay+ n;    //-n;   //mais dRay pointe vers le terrain et n vers le haut.
    double norm = dRay.dot(n);    //le rayon va normalement dans le sens inverse de la normal du triangle qu'il touche,

    if(norm >= 0)
        return QColor(0,0,0); //Black
    else
    {
        float hauteur = objleplusproche.getHauteur(pointImpact);
        hauteur /= objleplusproche.getMaxElevation();

        float r,g,b;
        objleplusproche.getColor(r,g,b, pointImpact(0), pointImpact(1));

        float c = (-255)*norm;
        return QColor(roundf(r*c),roundf(g*c), roundf(b*c)); // Grey
    }
}


/**simule le parcours d'une camera sur le terrain de la scène
*  experimental*/
void Scene::addParcoursCamera(Terrain* noise)
{
    double x = NoiseGenerator::random(noise->largeur);
    double y = NoiseGenerator::random(noise->longueur);

    double dirMax = 5;
    double dMax = 2;
    Vector2f dir(NoiseGenerator::random2(dirMax),
                 NoiseGenerator::random2(dirMax));
    float d = dir.norm();
    if(d > dirMax){
        dir /= d;
        dir *= dirMax;
    }

    for(int i = 0;  i < 120;    i++)    {
        Vector2f dev(NoiseGenerator::random2(dMax),
                     NoiseGenerator::random2(dMax));

        d = dev.norm();
        if(d > dMax)   {
            dev /= d;
            dev *= dMax;
        }
        dir += dev;

        d = dir.norm();
        if(d > dirMax) {
            dir /= d;
            dir *= dirMax;
        }
        x += dir(0);
        y += dir(1);

        float x2 = x + dir(0),
              y2 = y + dir(1);

        //la camera regarde dans la direction du prochain déplacement
        Camera* cam = new Camera(Vector3f(x,y,noise->getHauteur(x,y)+10), Vector3f(x2,y2,noise->getHauteur(x2,y2)+10), 300, 720, 400);
        addC(cam);
    }
}

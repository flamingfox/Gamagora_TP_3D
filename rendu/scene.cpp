#include "scene.h"


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

bool Scene::rendu(){
    int ic = 1;     //indice pour sauvegarder sur des images de noms différents
    for(Camera* c : cameras)
    {
        int _lu = c->getLu(), _lv = c->getLv();
        QImage *img = new QImage(_lu, _lv, QImage::Format_RGB888);
        //img->fill(QColor(Qt::white).rgb());
        for(int x = 0; x < _lu ; x++){
            std::cerr << "\r" << ic << " Rendering: " << 100 * x / (_lu - 1) << "%";
            for(int y = 0; y < _lv ; y++){
                bool touche = false;
                Rayon r(c->getOrigine(),c->vecScreen(x,y));
                float coefdisttmp = FLT_MAX;
                float coefdistfinal = FLT_MAX;
                Vector3f zonetouchee;
                const Object* objleplusproche;
                for(const Object* obj: objects){
                    if(obj->intersect(r,coefdisttmp)){//si on touche
                        touche = true;
                        if(coefdisttmp < coefdistfinal){//on sélectionne l'objet touché le plus proche
                            coefdistfinal = coefdisttmp;
                            objleplusproche = obj;
                            zonetouchee = c->getOrigine() + (coefdistfinal*r.getDirection());
                        }
                    }
                }
                //for(const Mesh* mesh : meshs)

                if(!touche)
                    img->setPixel(x, y, default_color.rgba());
                else
                {
                    QColor color = render(zonetouchee, *objleplusproche, r);
                    img->setPixel(x,y,color.rgba());
                    //SetPixel(img, x, y, render(touche, zonetouchee, *objleplusproche, r));
                }
            }
        }
        //antialiasing(img)->save("testaliasing.png");
        img->save(("test" + std::to_string(ic) + ".png").c_str());
        std::cout << ("test" + std::to_string(ic) + ".png").c_str() << std::endl;
        ic++;
        delete img;
    }
    return true;
}

/*QColor Scene::renderHors()
{
    return default_color;
}*/

QColor Scene::render(const Eigen::Vector3f& pointImpact, const Object& objleplusproche, const Rayon& ray)
{
    Eigen::Vector3f dRay = ray.getDirection();
    dRay.normalize();

    Eigen::Vector3f n = objleplusproche.getNormal(pointImpact);

    Eigen::Vector3f diff = dRay+ n;    //-n;   //mais dRay pointe vers le terrain et n vers le haut.
    double norm = diff.squaredNorm();    //si le rayon va dans le sens inverse de la normal du triangle qu'il touche,
    //norm = 4-norm;

            //heatMapGradient.getColorAtValue(hauteur, r,g,b);

    /*color.setRed(r*255);
    color.setGreen(g*255);
    color.setBlue(b*255);*/

    if(norm >= 2)
        return QColor(0,0,0); //Black
    else if(norm == 0)
        return QColor(255,255,255); // White
    else
    {
        float hauteur = objleplusproche.getVal(pointImpact);
        hauteur /= objleplusproche.box.diffZ();

        float r,g,b;
        objleplusproche.getColor(r,g,b, hauteur);

        float c = 255-(255*norm)/2;
        return QColor(roundf(r*c),roundf(g*c), roundf(b*c)); // Grey
    }
}

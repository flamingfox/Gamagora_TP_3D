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
                const Terrain* objleplusproche;
                for(const Terrain* terrain: terrains){
                    if(terrain->intersect(r,coefdisttmp)){//si on touche
                        touche = true;
                        if(coefdisttmp < coefdistfinal){//on sélectionne l'objet touché le plus proche
                            coefdistfinal = coefdisttmp;
                            objleplusproche = terrain;
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

QColor Scene::render(const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray)
{
    Eigen::Vector3f dRay = ray.getDirection();
    dRay.normalize();

    Eigen::Vector3f n = objleplusproche.getNormal(pointImpact);

    /*Eigen::Vector3f diff = dRay - n;
    double norm = diff.squaredNorm();    //si le rayon va dans le sens inverse de la normal du triangle qu'il touche,
    norm = 4-norm;*/
    //qDebug()<<norm;
    Vector3f color = objleplusproche.getColor(pointImpact); //défini la couleur entre herbe, neige et roche.

    /*if(norm >= 2)
        color = QColor(0,0,0); //Black
    else if(norm == 0)
        color = QColor(255,255,255); // White
    else
    {
        int c = 255-round((255*norm)/2);
        color = QColor(c, c, c); // Grey
    }*/

    //float puissanceMax = 0.f;
    Eigen::Vector3f sumColor(0,0,0);
    //float min = 1000;
    for(Lumiere* l : lights)
    {
        //puissanceMax += l->puissance;
        Eigen::Vector3f dirL = pointImpact-l->origine;
        float distL = dirL.norm();
        Eigen::Vector3f dL = dirL.normalized();

        if(distL < l->rayon)
        {
            float div = distL/l->rayon;
            float ambiant = objleplusproche.getPhong().ambiant*l->phong.ambiant;
            float puissance = l->puissance*(1-div*div);

            Vector3f color2(color*ambiant);

            bool b1 = (dL-n).squaredNorm() <= 2.0f, b2 = (dL-dRay).squaredNorm() <= 2.0f;
            if(b1 == b2)  //si la lumière touche l'objet du même coté qu'on le regarde
            {
                bool touche = true;
                Rayon r(l->origine,dL);

                float coefdisttmp;
                for(const Terrain* terrain: terrains){
                    if(terrain->intersect(r,coefdisttmp)){//si on touche
                        if(distL*0.99f > coefdisttmp) {   //si la lumière parcours une distance plus courte que voulu.
                            touche= false;
                            break;
                        }
                    }
                }
                if(touche)
                {
                    Vector3f dirL2 = -dL;
                    float diffuse = objleplusproche.getPhong().diffuse*l->phong.diffuse * fabsf(dirL2.dot(n));

                    Vector3f ref = (n * (fabsf(n.dot(dirL2))) * 2.f - dirL2).normalized();
                    float specular = objleplusproche.getPhong().specular*l->phong.specular * fabsf(powf(ref.dot(-dRay),objleplusproche.getPhong().brillance));

                    color2 += color * (diffuse + specular);
                }
            }
            sumColor += puissance * color2;
        }
    }
    //std::cout << min << std::endl;
    color = sumColor;
    for(int i = 0;  i < 3;  i++)
    {
        if(color(i) > 255)
            color(i) = 255;
        else
            color(i) = roundf(color(i));
    }
    return QColor(color(0), color(1), color(2));
}

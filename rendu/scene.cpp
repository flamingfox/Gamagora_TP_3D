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
    //for(Camera* c : cameras)  //marche pas avec openMP
    #pragma omp parallel for
    for(unsigned int ic = 0;  ic < cameras.size(); ic++)
    {
        Camera* c = cameras[ic];
        int _lu = c->getLu(), _lv = c->getLv();
        int pourcent2 = -1;
        QImage *img = new QImage(_lu, _lv, QImage::Format_RGB888);

        #pragma omp parallel for
        for(int y = 0; y < _lv ; y++){      // pour chaque ligne de l'image
            int pourcent = 100 * y / (_lv - 1);
            if(pourcent != pourcent2)            {
                pourcent2 = pourcent;
                std::cout << "\r" << ic << " Rendering: " << pourcent << "% ";  // barre de progression
            }

            //#pragma omp parallel for
            for(int x = 0; x < _lu ; x++){  // pour chaque pixel de la ligne
                Rayon r(c->getOrigine(),c->vecScreen(x,y));   //rayon correspondant au pixel

                //boucle for pour l'antialiasing ici
                bool touche = false;
                float coefdisttmp = FLT_MAX;        //variables pour déterminer la distance du rayon le plus court
                float coefdistfinal = FLT_MAX;
                Vector3f zonetouchee;
                const Object* objleplusproche = nullptr;
                for(const Object* obj: objects){    //parcours tous les objets de la scene
                    if(obj->intersect(r,coefdisttmp)){//si on touche
                        touche = true;
                        if(coefdisttmp < coefdistfinal){//on sélectionne l'objet touché le plus proche
                            coefdistfinal = coefdisttmp;
                            objleplusproche = obj;
                            zonetouchee = c->getOrigine() + (coefdistfinal*r.getDirection());
                        }
                    }
                }

                if(!touche)
                    img->setPixel(x, y, default_color.rgba());
                else
                {
                    QColor color = render(zonetouchee, *objleplusproche, r);
                    img->setPixel(x,y,color.rgba());
                }
            }
        }
        if(ic<10)        {
            img->save(("test000" + std::to_string(ic) + ".png").c_str());
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

/*QColor Scene::renderHors()
{
    return default_color;
}*/

QColor Scene::render(const Eigen::Vector3f& pointImpact, const Object& objleplusproche, const Rayon& ray)
{
    Eigen::Vector3f dRay = ray.getDirection();
    dRay.normalize();

    Eigen::Vector3f n = objleplusproche.getNormal(pointImpact);

    //Eigen::Vector3f diff = dRay+ n;    //-n;   //mais dRay pointe vers le terrain et n vers le haut.
    double norm = dRay.dot(n);    //si le rayon va dans le sens inverse de la normal du triangle qu'il touche,


    if(norm >= 0)
        return QColor(0,0,0); //Black
    //else if(norm == -1)
        //return QColor(0,0,0); //Black
    //    return QColor(255,255,255); // White
    else
    {
        float hauteur = objleplusproche.getVal(pointImpact);
        hauteur /= objleplusproche.getMaxElevation();

        float r,g,b;
        objleplusproche.getColor(r,g,b, pointImpact(0), pointImpact(1));

        float c = (-255)*norm;
        return QColor(roundf(r*c),roundf(g*c), roundf(b*c)); // Grey
    }
}


/**simule le parcours d'une camera sur le terrain de la scène*/
void Scene::addParcoursCamera(Terrain2* noise)
{
    int x = NoiseGenerator::perlinNoiseGradiant(rand()&255,rand()&255,1+(rand()&255))*noise->largeur;
    int y = NoiseGenerator::perlinNoiseGradiant(rand()&255,rand()&255,1+(rand()&255))*noise->longueur;

    int dirMax = 10;
    int dMax = 2;
    Vector2f dir(NoiseGenerator::perlinNoiseGradiant(rand()&255,rand()&255,1+(rand()&255))*dirMax,
                 NoiseGenerator::perlinNoiseGradiant(rand()&255,rand()&255,1+(rand()&255))*dirMax);
    float d = dir.norm();
    if(d > dirMax){
        dir /= d;
        dir *= dirMax;
    }

    for(int i = 0;  i < 120;    i++)    {
        Vector2f dev(NoiseGenerator::perlinNoiseGradiant2(rand()&255,rand()&255,1+(rand()&255))*dMax,
                     NoiseGenerator::perlinNoiseGradiant2(rand()&255,rand()&255,1+(rand()&255))*dMax);

        d = dev.norm();
        if(d > dMax)   {
            dev /= d;
            dev *= dMax;
        }
        dir += dev;

        d = dev.norm();
        if(d > dirMax) {
            dir /= d;
            dir *= dirMax;
        }
        x += dir(0);
        y += dir(1);

        float x2 = x + dir(0),
              y2 = y + dir(1);

        //if(i == 21)
        {
            Camera* cam = new Camera(Vector3f(x,y,noise->getHauteur(x,y)+10), Vector3f(x2,y2,noise->getHauteur(x2,y2)+10), 300, 720, 400);
            addC(cam);
        }
    }
}

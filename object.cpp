#include "object.h"

Object::Object()
{
}

Object::Object(const std::vector<Object*>& objects):
        objects(objects)
{
    if(objects.empty())
        return;
    //mettre à jour la boite
    /*std::vector<Object*>::const_iterator it = objects.begin();
    box = it->box;
    ++it;
    for(;   it != objects.end();    ++it)
        box.merge(it->box);*/
    for(const Object* obj: objects)
        box.merge(obj->box);
}


void Object::translate(const Vector3f& t)
{
    if(t == Vector3f(0,0,0))
        return;
    box.min += t;
    box.max += t;
    if(objects.empty())
        translate2(t);
    else
        for(Object* obj: objects)
            obj->translate(t);
}


const Object* Object::intersect(const Rayon& rayon, float &coeffDistance) const
{
    if(objects.empty())
    {
        if(intersect2(rayon, coeffDistance))
            return this;
    }
    else
    {
        float min, max;
        if(!box.intersect(rayon,min, max))  //si le rayon n'intersecte pas la boite englobante de l'ensemble des objets
            return nullptr;
        coeffDistance = FLT_MAX;
        const Object* objProche = nullptr;
        for(const Object* obj: objects)        {
            float coeff;
            obj->intersect(rayon, coeff);
            if(coeff < coeffDistance){
                coeffDistance = coeff;
                objProche = obj;
            }
        }
        return objProche;
    }
    return nullptr; //aucun objet intersecté
}


void Object::getColor(float& r, float& g, float& b, float hauteur) const
{
    if(!objects.empty())
        std::cerr << "/!\\ l'object ne devrait pas contenir une liste d'objects /!\\" << std::endl;
    getColor2(r, g, b, hauteur);
}

/******************************************************************/

Object* Object::addObject(Object* obj)
{
    if(objects.empty())
    {
        Object* listeObject = new Object();
        listeObject->objects.push_back(this);
        listeObject->box = this->box;
        listeObject->objects.push_back(obj);
        listeObject->box.merge(obj->box);
        return listeObject;
    }
    else
    {
        this->objects.push_back(obj);
        this->box.merge(obj->box);
        return this;
    }
}

Object* Object::addObjectStrict(Object* obj, bool verif)
{
    if(verif && objects.empty())
        std::cerr << "/!\\ l'object ne devrait pas contenir une liste d'objects /!\\" << std::endl;

    this->objects.push_back(obj);
    this->box.merge(obj->box);
    return this;
}


/*******************************************************************/
///////////////////////////////virtual///////////////////////////////

//ajouté après un problème avec "new Object" dans "addObject(Object obj)" car on ne peut pas

float Object::getVal(const Vector3f& p) const
{
    std::cerr << "/!\\ ne devrait pas entrer dans getVal virtual de Object /!\\" << std::endl;
    (void) p;
    return -9999;
}
Vector3f Object::getNormal(const Vector3f& p) const
{
    std::cerr << "/!\\ ne devrait pas entrer dans getNormal virtual de Object /!\\" << std::endl;
    (void) p;
    return Vector3f(0,0,0);
}

void Object::translate2(const Eigen::Vector3f& t)
{
    std::cerr << "/!\\ ne devrait pas entrer dans translate2 virtual de Object /!\\" << std::endl;
    box += t;
}
bool Object::intersect2(const Rayon& rayon, float &coeffDistance) const
{
    std::cerr << "/!\\ ne devrait pas entrer dans translate2 virtual de Object /!\\" << std::endl;
    (void) rayon;
    coeffDistance = -9999;
    return false;
}
void Object::getColor2(float& r, float& g, float& b, float hauteur) const
{
    std::cerr << "/!\\ ne devrait pas entrer dans getColor2 virtual de Object /!\\" << std::endl;
    (void) hauteur;
    r=0;
    g=0;
    b=0;
}

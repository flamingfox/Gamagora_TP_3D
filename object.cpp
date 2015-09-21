#include "object.h"

Object::Object()
{
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

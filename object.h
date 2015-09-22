#ifndef OBJECT_H
#define OBJECT_H

#include "box.h"

/**un Object peut être un terrain, un mesh, une surface implicite ou toutes autres choses à afficher*/
class Object
{
public:
    Object();
    Object(const std::vector<Object*>& objects);
    Object(float longueur, float largeur, float hauteur):   box(Box(Vector3f(0,0,0),Vector3f(largeur,longueur, hauteur)))
    {}

    Box box;

    void translate(const Eigen::Vector3f& t);
    const Object* intersect(const Rayon& rayon, float &coeffDistance) const;
    void getColor(float& r, float& g, float& b, float hauteur = 0) const;


    virtual float getVal(const Vector3f& p) const;
    virtual Vector3f getNormal(const Vector3f& p) const;

    Object* addObject(Object* obj);
    Object* addObjectStrict(Object* obj, bool verif = true);

protected:

    virtual void translate2(const Eigen::Vector3f& t);
    virtual bool intersect2(const Rayon& rayon, float &coeffDistance) const;
    virtual void getColor2(float& r, float& g, float& b, float hauteur = 0) const;


private:
    std::vector<Object*> objects;
};

#endif // OBJECT_H

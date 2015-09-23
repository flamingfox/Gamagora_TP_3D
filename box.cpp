#include "box.h"


Box::Box()  :   min(Vector3f(FLT_MAX,FLT_MAX,FLT_MAX)), max(Vector3f(FLT_MIN,FLT_MIN,FLT_MIN))
{
}

Box::Box(const Vector3f& _min, const Vector3f& _max)    :   min(_min),  max(_max)
{
}

/*Box::Box(const Mesh& mesh)
{
    parcourtPoints(mesh.getGeom());
}*/

Box::Box(const std::vector<Vector3f>& points)
{
    parcourtPoints(points);
}


void Box::updatePoint(const Vector3f& p)
{
    update(p);
}


bool Box::isIn(const Vector3f& p) const
{
    for(int i = 0;  i < 3;  i++)
    {
        if(p(i) < min(i))
            return false;
        if(p(i) > max(i))
            return false;
    }
    return true;
}

/*bool Box::intersect(const Rayon &r, float &distanceMin, float &distanceMax) const
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    float divx;

    if(r.getDirection()(0) != 0)
        divx = 1 / r.getDirection()(0);
    else
        divx = 0;

    if(r.getDirection()(0) >= 0)
    {
        tmin = (min(0) - r.getOrigine()(0)) * divx;
        tmax = (max(0) - r.getOrigine()(0)) * divx;
    }
    else
    {
        tmin = (max(0) - r.getOrigine()(0)) * divx;
        tmax = (min(0) - r.getOrigine()(0)) * divx;
    }

    float divy;

    if(r.getDirection()(1) != 0)
        divy = 1 / r.getDirection()(1);
    else
        divy = 0;

    if(r.getDirection()(1) >= 0)
    {
        tymin = (min(1) - r.getOrigine()(1)) * divy;
        tymax = (max(1) - r.getOrigine()(1)) * divy;
    }
    else
    {
        tymin = (max(1) - r.getOrigine()(1)) * divy;
        tymax = (min(1) - r.getOrigine()(1)) * divy;
    }
    if( (tmin > tymax) || (tymin > tmax) )
        return false;

    if(tymin > tmin)
        tmin = tymin;

    if(tymax < tmax)
        tmax = tymax;

    float divz;

    if(r.getDirection()(2) != 0)
        divz = 1 / r.getDirection()(2);
    else
        divz = 0;

    if (r.getDirection()(2) >= 0)
    {
        tzmin = (min(2) - r.getOrigine()(2)) * divz;
        tzmax = (max(2) - r.getOrigine()(2)) * divz;
    }
    else
    {
        tzmin = (max(2) - r.getOrigine()(2)) * divz;
        tzmax = (min(2) - r.getOrigine()(2)) * divz;
    }

    if( (tmin > tzmax) || (tzmin > tmax) )
        return false;

    if(tzmin > tmin)
        tmin = tzmin;

    if(tzmax < tmax)
        tmax = tzmax;

    if(tmin>0)
        distanceMin = tmin;

    distanceMin += 0.002;

    if(tmax>0)
        distanceMax = tmax;

    return true;
}*/


bool Box::intersect(const Rayon &r, float &distanceMin, float &distanceMax) const
{
    if(this->isIn(r.getOrigine()))  {
        distanceMin = 0;
        distanceMax = intersectIn(r);
        return true;
    }

    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    float div;

    if(r.getDirection()(0) == 0)    {
        tmin = FLT_MIN;
        tmax = FLT_MAX;
    }
    else if(r.getDirection()(0) > 0)    {
        div = 1 / r.getDirection()(0);
        tmin = (min(0) - r.getOrigine()(0)) * div;
        tmax = (max(0) - r.getOrigine()(0)) * div;
    }
    else    {
        div = 1 / r.getDirection()(0);
        tmin = (max(0) - r.getOrigine()(0)) * div;
        tmax = (min(0) - r.getOrigine()(0)) * div;
    }

    if(r.getDirection()(1) == 0)    {
        tymin = FLT_MIN;
        tymax = FLT_MAX;
    }
    else if(r.getDirection()(1) >= 0)    {
        div = 1 / r.getDirection()(1);
        tymin = (min(1) - r.getOrigine()(1)) * div;
        tymax = (max(1) - r.getOrigine()(1)) * div;
    }
    else    {
        div = 1 / r.getDirection()(1);
        tymin = (max(1) - r.getOrigine()(1)) * div;
        tymax = (min(1) - r.getOrigine()(1)) * div;
    }

    if( (tmin > tymax) || (tymin > tmax) )
        return false;

    if(tymin > tmin)
        tmin = tymin;

    if(tymax < tmax)
        tmax = tymax;


    if(r.getDirection()(2) == 0)    {
        tzmin = FLT_MIN;
        tzmax = FLT_MAX;
    }
    else if(r.getDirection()(2) > 0)    {
        div = 1 / r.getDirection()(2);
        tzmin = (min(2) - r.getOrigine()(2)) * div;
        tzmax = (max(2) - r.getOrigine()(2)) * div;
    }
    else    {
        div = 1 / r.getDirection()(2);
        tzmin = (max(2) - r.getOrigine()(2)) * div;
        tzmax = (min(2) - r.getOrigine()(2)) * div;
    }

    if( (tmin > tzmax) || (tzmin > tmax) )
        return false;

    if(tzmin > tmin)
        tmin = tzmin;

    if(tzmax < tmax)
        tmax = tzmax;

    if(tmin>=0)
        distanceMin = tmin;
    //else
    //    return false; //inutile apparament
    //distanceMin += 0.002;

    if(tmax>0)
        distanceMax = tmax;

    return true;
}



inline float Box::intersectIn(const Rayon& r) const
{
    float tmax, tymax, tzmax;

    if(r.getDirection()(0) == 0)
        tmax = FLT_MAX;
    else if(r.getDirection()(0) > 0)
        tmax = (max(0) - r.getOrigine()(0)) / r.getDirection()(0);
    else
        tmax = (min(0) - r.getOrigine()(0)) / r.getDirection()(0);

    if(r.getDirection()(1) == 0)
        tymax = FLT_MAX;
    else if(r.getDirection()(1) >= 0)
        tymax = (max(1) - r.getOrigine()(1)) / r.getDirection()(1);
    else
        tymax = (min(1) - r.getOrigine()(1)) / r.getDirection()(1);

    if(tymax < tmax)
        tmax = tymax;


    if(r.getDirection()(2) == 0)
        return tmax;
    else if(r.getDirection()(2) > 0)
        tzmax = (max(2) - r.getOrigine()(2)) / r.getDirection()(2);
    else
        tzmax = (min(2) - r.getOrigine()(2)) / r.getDirection()(2);

    if(tzmax < tmax)
        return tzmax;
    return tmax;
}


/**********************inline********************/

inline void Box::setDefaultBox()
{
    min = Vector3f(FLT_MAX,FLT_MAX,FLT_MAX);
    max = Vector3f(FLT_MIN,FLT_MIN,FLT_MIN);
}

inline void Box::update(const Vector3f& p)
{
    updateMin(p);
    updateMax(p);
}

inline void Box::updateMin(const Vector3f& p)
{
    for(int i = 0;  i < 3;  i++)
        if(p(i) < min(i))
            min(i) = p(i);

}

inline void Box::updateMax(const Vector3f& p)
{
    for(int i = 0;  i < 3;  i++)
        if(p(i) > max(i))
            max(i) = p(i);
}

inline void Box::parcourtPoints(const std::vector<Vector3f>& points)
{
    if(points.empty())
        setDefaultBox();
    else if(points.size() == 1)
    {
        min = points[0];
        max = points[0];
    }
    else
    {
        std::vector<Vector3f>::const_iterator it = points.begin();
        min = *it;
        max = points[points.size()-1];

        ++it;
        for(;  it != points.end()-1; ++it)
            update(*it);
    }
}

float Box::diffZ() const
{
    return max(2)-min(2);
}

void Box::merge(const Box& box2)
{
    updateMin(box2.min);
    updateMax(box2.max);
}


void Box::operator+=(const Vector3f& t)
{
    min += t;
    max += t;
}

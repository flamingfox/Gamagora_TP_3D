#include "box.h"


Box::Box()  :   min(Vector3f(FLT_MAX,FLT_MAX,FLT_MAX)), max(Vector3f(FLT_MIN,FLT_MIN,FLT_MIN))
{
}

Box::Box(const Vector3f& _min, const Vector3f& _max)    :   min(_min),  max(_max)
{
}

Box::Box(const Mesh& mesh)
{
    parcourtPoints(mesh.getGeom());
}

Box::Box(const std::vector<Vector3f>& points)
{
    parcourtPoints(points);
}


void Box::updatePoint(const Vector3f& p)
{
    update(p);
}


bool Box::isIn(const Vector3f& p)
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

bool Box::intersect(const Rayon &r, float &distanceMin, float &distanceMax) const
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

    distanceMin = tmin;
    distanceMax = tmax;
    return true;
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

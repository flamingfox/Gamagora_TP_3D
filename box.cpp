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

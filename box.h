#ifndef BOX_H
#define BOX_H

#include <Eigen/Geometry>
//#include "mesh.h"
#include "float.h"  //FLT_MIN FLT_MAX
#include <iostream>
#include "rendu/rayon.h"

using namespace Eigen;

class Box
{
public:
    Vector3f min, max;
    Box();
    Box(const Vector3f& _min, const Vector3f& _max);
    //Box(const Mesh& mesh);
    Box(const std::vector<Vector3f>& points);

    void updatePoint(const Vector3f& p);
    bool isIn(const Vector3f& p) const;

    bool intersect(const Rayon &r, float &distanceMin, float &distanceMax) const;

    float diffZ() const;

    void merge(const Box& box2);

    void operator+=(const Vector3f& t);

protected:

private:
    inline void setDefaultBox();
    inline void update(const Vector3f& p);
    inline void updateMin(const Vector3f& p);
    inline void updateMax(const Vector3f& p);

    inline void parcourtPoints(const std::vector<Vector3f>& points);

    inline float intersectIn(const Rayon& r) const;
};

#endif // BOX_H

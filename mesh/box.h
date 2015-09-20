#ifndef BOX_H
#define BOX_H

#include <Eigen/Geometry>
#include "float.h"  //FLT_MIN FLT_MAX
#include "./scene/rayon.h"


using namespace Eigen;
class Mesh;
#include "mesh.h"

class Box
{

public:

    Vector3f min, max;
    Box();
    Box(const Vector3f& _min, const Vector3f& _max);
    Box(const Mesh& mesh);
    Box(const std::vector<Vector3f>& points);

    void updatePoint(const Vector3f& p);
    bool isIn(const Vector3f& p) const;

    bool intersect(const Rayon &r, float &distanceMin, float &distanceMax) const;
    /**@pointDentrer le point par lequel est rentrer le rayon (évite les erreurs avec les float). Egal à r.getOrigine() si démarre dans la box
    */
    bool intersect(const Rayon &r, float &distanceMin, float &distanceMax, Eigen::Vector3f& pointDentrer) const;

protected:
    inline void setDefaultBox();
    inline void update(const Vector3f& p);
    inline void updateMin(const Vector3f& p);
    inline void updateMax(const Vector3f& p);

    inline void parcourtPoints(const std::vector<Vector3f>& points);

};

#endif // BOX_H

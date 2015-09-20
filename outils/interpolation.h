#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <math.h>
#include <Eigen/Geometry>


using namespace Eigen;

namespace interp
{

float pente(float val1, float val2, float p1 = 0, float p2 = 1);

float interp_linear1D(float abis, float val1, float val2, float a1 = 0, float a2 = 1);
float interp_cos1D(float abis, float val1, float val2, float a1 = 0, float a2 = 1);
float interp_hermite1D(float abis, float val1, float val2, float a1 = 0, float a2 = 1);

float interp_linear2D(float xb, float zb,
                       float x1, float z1,
                       float x2, float z2,
                       float val11, float val12, float val21, float val22);
float interp_linear2D(const Vector2f& vb,
                       const Vector2f& v1,
                       const Vector2f& v2,
                       float val11, float val12, float val21, float val22);

float interp_cos2D(float xb, float zb,
                    float x1, float z1,
                    float x2, float z2,
                    float val11, float val12, float val21, float val22);
float interp_cos2D(const Vector2f& vb,
                    const Vector2f& v1,
                    const Vector2f& v2,
                    float val11, float val12, float val21, float val22);

float interp_hermite2D(float xb, float zb,
                        float x1, float z1,
                        float x2, float z2,
                        float val11, float val12, float val21, float val22);
float interp_hermite2D(const Vector2f& vb,
                        const Vector2f& v1,
                        const Vector2f& v2,
                        float val11, float val12, float val21, float val22);

}   //end namespace interp

#endif // INTERPOLATION_H

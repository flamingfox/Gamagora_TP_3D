#include "interpolation.h"




using namespace Eigen;

namespace interp
{

float pente(float val1, float val2, float p1, float p2)
{
    if(val1 == val2)
        return 0;
    return (val2-val1)/(p2-p1);
}


float interp_linear1D(float abis, float val1, float val2, float a1, float a2)
{
    if(a1 == a2 || val1 == val2)
        return val1;
    else
        return pente(val1, val2, a1, a2)*(abis-a1)+val1;
}

float interp_linear2D(float xb, float zb,
                       float x1, float z1,
                       float x2, float z2,
                       float val11, float val12, float val21, float val22)
{
    if(z1 == z2)
        return interp_linear1D(xb, val11, val21, x1, x2);
    else
    {
        float y1 = interp_linear1D(xb, val11, val21, x1, x2),
                     y2 = interp_linear1D(xb, val12, val22, x1, x2);
        return interp_linear1D(zb, y1, y2, z1, z2);
    }
}
float interp_linear2D(const Vector2f& vb,
                       const Vector2f& v1,
                       const Vector2f& v2,
                       float val11, float val12, float val21, float val22)
{
    return interp_linear2D(vb(0), vb(1), v1(0), v1(1), v2(0), v2(1),
                    val11, val12, val21, val22);
}


/***************************************************************************************************************************/

float interp_cos1D(float abis, float val1, float val2, float a1, float a2)
{
    if(a1 == a2 || val1 == val2)
        return val1;
    else
    {
        float ft = (abis-a1)/(a2-a1)* M_PI,
                     f = (1-cos(ft))*0.5;
        return val1*(1-f)+val2*f;
    }
}

float interp_cos2D(float xb, float zb,
                    float x1, float z1,
                    float x2, float z2,
                    float val11, float val12, float val21, float val22)
{
    if(z1 == z2)
        return interp_cos1D(xb, val11, val21, x1,x2);
    else
    {
        float y1 = interp_cos1D(xb, val11, val21, x1, x2),
                     y2 = interp_cos1D(xb, val12, val22, x1, x2);
        return interp_cos1D(zb, y1, y2, z1, z2);
    }
}
float interp_cos2D(const Vector2f& vb,
                    const Vector2f& v1,
                    const Vector2f& v2,
                    float val11, float val12, float val21, float val22)
{
    return interp_cos2D(vb(0), vb(1), v1(0), v1(1), v2(0), v2(1),
                    val11, val12, val21, val22);
}

/**************************************************************************************************************************/
float interp_hermite1D(float abis, float val1, float val2, float a1, float a2)
{
    if(a1 == a2 || val1 == val2)
        return val1;
    else
    {
        float t = (abis-a1)/(a2-a1);
        return (val2-val1)*t*t*(3 - 2*t)+val1;
    }
}

float interp_hermite2D(float xb, float zb,
                    float x1, float z1,
                    float x2, float z2,
                    float val11, float val12, float val21, float val22)
{
    if(z1 == z2)
        return interp_hermite1D(xb, val11, val21, x1, x2);
    else
    {
        float y1 = interp_hermite1D(xb, val11, val21, x1, x2),
                     y2 = interp_hermite1D(xb, val12, val22, x1, x2);
        return interp_hermite1D(zb, y1, y2, z1, z2);
    }
}
float interp_hermite2D(const Vector2f& vb,
                        const Vector2f& v1,
                        const Vector2f& v2,
                        float val11, float val12, float val21, float val22)
{
    return interp_hermite2D(vb(0), vb(1), v1(0), v1(1), v2(0), v2(1),
                    val11, val12, val21, val22);
}

} //end namespace interp

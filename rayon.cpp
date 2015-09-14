#include "rayon.h"

Rayon::Rayon()
{

}

float interesct(Mesh mesh)
{
    float tmin = 0.0;
    float tmax = 1000.0;
    float t = tmin;
    for( int i=0; i<256; i++ )
    {
        vec3 pos = ro + t*rd;
        float h = pos.y - terrainM( pos.xz );
        if( h<(0.002*t) || t>tmax ) break;
        t += 0.5*h;
    }

    return t;
}


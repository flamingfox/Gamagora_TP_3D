#ifndef CUBE_H
#define CUBE_H

#include "../mesh.h"

class Cube : public Mesh
{
public:
    Cube();

private:
    float lo, la, h;
};

#endif // CUBE_H

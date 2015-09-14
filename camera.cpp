#include "camera.h"

Camera::Camera()
{

}

Camera(Eigen::Vector3f pos, Eigen::Vector3f at){
    o = o;
    v = (a-o)/||a-o||;
    u = - (w ^ (0,0,1)); //à normaliser       * ^ = vectoriel
    v = w ^ u; //à normaliser
}


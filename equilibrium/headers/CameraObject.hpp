#ifndef CAMERAOBJECT_HPP
#define CAMERAOBJECT_HPP

#include "SimpleObject.hpp"

class CameraObject : virtual public SimpleObject
{

public:
    vec3 position;
    vec3 target;
    vec3 up;

public:
    CameraObject(string id, vec3 target, vec3 up);
    CameraObject();
};

#endif

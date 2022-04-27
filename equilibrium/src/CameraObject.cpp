#include "../headers/CameraObject.hpp"

CameraObject::CameraObject(vec3 pos, vec3 target, vec3 up) {
    this->position = pos;
    this->target = target;
    this->up = up;
}

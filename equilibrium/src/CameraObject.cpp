#include "../headers/CameraObject.hpp"

CameraObject::CameraObject(string id, vec3 target, vec3 up)
{
    this->id = id;
    this->transform = new Transform();
    this->parent = nullptr;
    this->position = this->transform->getLocalTranslation();
    this->target = target;
    this->up = up;
}

CameraObject::CameraObject()
{
}

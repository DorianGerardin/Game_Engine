#include "../headers/CameraObject.hpp"

CameraObject::CameraObject(vec3 target, vec3 up) {
    this->transform = new Transform();
    this->parent = nullptr;
    this->position = this->transform->getLocalTranslation();
    this->target = target;
    this->up = up;
}

CameraObject::CameraObject() {
    
}

#include "SimpleObject.hpp"

class CameraObject : virtual public SimpleObject {

    public:
        vec3 position;
        vec3 target;
        vec3 up;

    public:
        CameraObject(vec3, vec3, vec3);

};

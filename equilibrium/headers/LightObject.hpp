#include "SimpleObject.hpp"


class LightObject: public SimpleObject {

    public:
        vec3 position;
        Material material;
        GLint shader;

    LightObject(GLuint);

    void draw();
    
};

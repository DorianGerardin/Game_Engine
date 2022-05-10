#include "SimpleObject.hpp"


class LightObject: public SimpleObject {

    public:
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        double shininess;
        GLint shader;

    LightObject(GLuint);

    void draw(int);
    
};

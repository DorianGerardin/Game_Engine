#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>
#include <memory>

#include "Mesh.hpp"
#include "Transform.hpp"
#include "SimpleObject.hpp"

class GameObject : virtual public SimpleObject
{

public:
    Mesh *mesh;
    GLuint shader;
    GLuint useHeightMap;
    bool hasTexture;

public:
    GameObject(string, GLint, GLuint);

    GameObject(string, float, GLint, GLuint);

    GameObject();

    ~GameObject();

    bool SameHalfPlane(vec3, vec3, vec3, vec3);

    bool pointInTriangle(vec3, vec3, vec3, vec3);

    int triangleIndexOnPosition(Mesh *);

    float heightInTriangle(Mesh *);

    void applyTexture(GLuint, GLuint);

    virtual void draw();
};

#endif
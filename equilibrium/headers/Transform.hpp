#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
// Include standard headers
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
#include <glm/gtx/io.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

class Transform
{

public:
    // Local space information
    vec3 scale;       // scale
    vec3 rotation;    // rotation
    vec3 translation; // translation
    // Global space information concatenate in matrix
    mat4 modelMatrix = mat4(1.0f);
    mat4 worldModelMatrix = mat4(1.0f);

    bool m_isDirty = true;

protected:
    mat4 getLocalModelMatrix();

public:
    Transform();
    Transform(vec3, vec3, vec3);
    ~Transform();

    void computeModelMatrix();

    void computeModelMatrix(const mat4 &);
    // Setters
    void setLocalTranslation(const vec3 &newPosition);

    void setLocalRotation(const glm::vec3 &);

    void setLocalScale(const glm::vec3 &);

    // Getters
    const glm::vec3 &getLocalTranslation() const;

    const vec3 getWorldTranslation() const;

    const glm::vec3 &getLocalRotation() const;

    const glm::vec3 &getLocalScale() const;

    const glm::mat4 &getModelMatrix() const;

    bool isDirty() const;
};
#endif // TRANSFORM_HPP
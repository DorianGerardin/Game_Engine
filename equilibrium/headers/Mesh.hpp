#ifndef MESH_HPP
#define MESH_HPP

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
#include <common/objloader.hpp>
#include <iostream>

using namespace glm;
using namespace std;

class Mesh
{

public:
    vector<vec3> indexed_vertices;
    vector<unsigned short> indices;
    vector<float> uv;
    float size;
    GLuint texture, textureID;
    GLint modelID;

public:
    Mesh(string, float, GLint);
    Mesh(string, GLint);
    ~Mesh();

private:
    void generateMesh(string);
    void generatePlane();
    void generateSphere();
};

#endif // MESH_HPP
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

#include <common/texture.hpp>

using namespace glm;
using namespace std;

enum Type
{
    MESH,
    SPHERE,
    CUBE,
    PLANE,
    INFINITE_PLANE
};

enum MaterialType
{
    BRICK,
    ICE,
    RUBBER,
    CRYSTAL
};

class Mesh
{

public:
    vector<vec3> indexed_vertices;
    vector<vec3> vericesNormals;
    vector<vec3> faceNormals;
    vector<unsigned short> indices;
    vector<float> uv;
    float size;
    GLuint texture, textureID;
    GLint modelID;

    int objectType = -1;

    bool isPlane = false;

public:
    Mesh(int, float, GLint);
    Mesh(string, GLint);
    ~Mesh();
    void computeNormals();

private:
    string getFileExt(const string &);
    void generateMesh();
    void generatePlane();
    void generateCube();
    void generateSphere();
    void computeFaceNormals();
    void computeVerticesNormals();
};

#endif // MESH_HPP
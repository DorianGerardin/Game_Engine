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

enum Type
{
    MESH,
    SPHERE,
    CUBE,
    PLANE
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    double shininess;

    float index_medium = 1.;
    float transparency = 0.;
};

class Mesh
{

public:
    vector<vec3> indexed_vertices;
    vector<vec3> normals;
    vector<unsigned short> indices;
    vector<float> uv;
    float size;
    GLuint texture, textureID;
    GLint modelID;

    int objectType = -1;

    Material material;

public:
    Mesh(int, float, GLint);
    Mesh(string, GLint);
    ~Mesh();

private:
    string getFileExt(const string &);
    void generateMesh();
    void generatePlane();
    void generateSphere();
    void calculate_normals();
    void initializeMaterial();
};

#endif // MESH_HPP
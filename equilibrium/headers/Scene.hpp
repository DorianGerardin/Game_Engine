#ifndef SCENE_HPP
#define SCENE_HPP
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
#include <iostream>

using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>
#include <memory>
#include "GameObject.hpp"

struct CAMERA
{
    vec3 position;
    vec3 target;
    vec3 up;
};

class Scene
{

private:
    CAMERA defaultCamera;
    //unique_ptr<SimpleObject> defaultCamera_ptr;

public:
    vector<GameObject *> objects;
    vector<CAMERA> cameras;
    //vector<SimpleObject*> cameras;

public:
    Scene();

    ~Scene();

    void addObject(GameObject *);

    void addCamera(CAMERA);

    //void addCamera(CameraObject* cam);
            
    void draw();
};

#endif // SCENE_HPP
#ifndef SCENE_HPP
#define SCENE_HPP
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm> // std::find

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
#include "CameraObject.hpp"
#include "LightObject.hpp"
#include "PhysicsObject.hpp"

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
    unique_ptr<CameraObject> defaultCamera_ptr;
    LightObject *light;

public:
    vector<GameObject *> objects;
    vector<CameraObject *> cameras2;
    vector<CAMERA> cameras;
    // vector<SimpleObject*> cameras;
    vector<PhysicsObject *> PhysicsObjectList;

public:
    Scene();

    ~Scene();

    // void addObject(GameObject *);

    void addObject(GameObject *);

    void addCamera(CAMERA);
    void addCamera2(CameraObject *cam);

    void addLight(LightObject *);

    void addPhysicsObject(PhysicsObject *);
    void removePhysicsObject(PhysicsObject *);
    void Step(float);

    void draw();
};

#endif // SCENE_HPP
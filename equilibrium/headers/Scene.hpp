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
    // CAMERA defaultCamera;
    unique_ptr<CameraObject> defaultCamera_ptr;
    LightObject *light;

public:
    vector<GameObject *> objects;
    vector<CameraObject *> cameras;
    vector<PhysicsObject *> PhysicsObjectList;
    vector<Solver *> SolverList;
    PhysicsObject *player;

public:
    Scene();

    ~Scene();

    // void addObject(GameObject *);

    void addObject(GameObject *);

    void addCamera(CameraObject *cam);

    void addLight(LightObject *);

    void addPhysicsObject(PhysicsObject *);
    void removePhysicsObject(PhysicsObject *);
    void addSolver(Solver *solver);
    void removeSolver(Solver *solver);
    void Step(float dt);
    void SendCollisionCallbacks(vector<Collision> &collisions, float dt);
    void ResolveCollisions(float dt);

    void update();
    void draw();

    GameObject *getGameObjectFromId(string);
    PhysicsObject *getPhysicsObjectFromId(string);
    void addPlayer(PhysicsObject *);
    PhysicsObject *getPlayer();
};

#endif // SCENE_HPP
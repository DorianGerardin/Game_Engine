#ifndef SIMPLEOBJECT_HPP
#define SIMPLEOBJECT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
//#include <glm/glm.hpp>
#include "glm/ext.hpp"
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

class SimpleObject
{

public:
    Transform *transform;
    SimpleObject *parent = nullptr;
    vector<unique_ptr<SimpleObject>> children;

public:
    SimpleObject();

    ~SimpleObject();

    void updateSelf();

    void forceUpdateSelf();

    void updateSelfAndChild();

    void forceUpdateSelfAndChild();

    void addChild(unique_ptr<SimpleObject>);

    SimpleObject *getParent();

};

#endif
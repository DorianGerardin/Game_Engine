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

#include "Transform.cpp"
#include "Mesh.cpp"

class Object2 {

	public:
        Transform *transform;
        Object* parent = nullptr;
        vector<unique_ptr<Object2>> children;

    public:
        Object() {
            transform = new Transform();
        }

        ~Object() {
            delete this;
        }

        void updateSelf() {
            if (!this->transform->isDirty())
                return;

            forceUpdateSelf();
        }

        void forceUpdateSelf() {
            this->transform->computeSelfModelMatrix();
        }

        void updateSelfAndChild()
        {
            if (!this->transform->isDirty())
                return;

            forceUpdateSelfAndChild();
        }

        void forceUpdateSelfAndChild()
        {
            if (parent) {
                this->transform->computeModelMatrix(parent->transform->getModelMatrix());
            }
            else
                this->transform->computeModelMatrix();

            for (auto&& child : children)
            {
                child->forceUpdateSelfAndChild();
            }
        }

        void addChild(unique_ptr<Object> o) {
            this->children.push_back(move(o));
            children.back()->parent = this;
        }

        Object* getParent() {
            return this->parent;
        }

};
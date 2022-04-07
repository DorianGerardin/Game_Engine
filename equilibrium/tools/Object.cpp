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

class Object {

	public:
        Mesh *mesh;
        Transform *transform;
        Object* parent = nullptr;
        vector<unique_ptr<Object>> children;
        GLuint shader;
        GLuint useHeightMap;
        bool hasTexture;

    public:
        Object(string filename, GLint modelID, GLuint shader) {
            transform = new Transform();
            this->mesh = new Mesh(filename, modelID);
            this->shader = shader;
            this->useHeightMap = GL_FALSE;
            this->hasTexture = false;
        }

        Object(string meshType, float size, GLint modelID, GLuint shader) {
            this->transform = new Transform();
            this->mesh = new Mesh(meshType, size, modelID);
            this->parent = nullptr;
            this->shader = shader;
            this->useHeightMap = GL_FALSE;
            this->hasTexture = false;
        }

        Object() {
            this->transform = new Transform();
            this->useHeightMap = GL_FALSE;
            this->parent = nullptr;
            this->hasTexture = false;
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


        void applyTexture(GLuint texture, GLuint textureID) {

            this->hasTexture = true;
            this->mesh->texture = texture;
            this->mesh->textureID = textureID;

        }

        void draw() {

            GLuint vertexbuffer;
            GLuint elementbuffer;
            GLuint uvbuffer;

            glUseProgram(this->shader);

            GLint useHeightMapID = glGetUniformLocation(this->shader, "useHeightMap");
            if(this->useHeightMap) glUniform1i(useHeightMapID, GL_TRUE);
            else glUniform1i(useHeightMapID, GL_FALSE);

            glUniformMatrix4fv(this->mesh->modelID, 1, GL_FALSE, &this->transform->getModelMatrix()[0][0]);

            // BUFFERS
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, this->mesh->indexed_vertices.size() * sizeof(glm::vec3), &this->mesh->indexed_vertices[0], GL_STATIC_DRAW);

            // Generate a buffer for the indices as well
            glGenBuffers(1, &elementbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->indices.size() * sizeof(unsigned short), &this->mesh->indices[0] , GL_STATIC_DRAW);

            //TEXTURES
            GLint hasTextureID = glGetUniformLocation(this->shader, "hasTexture");
            if(hasTexture) {

                glUniform1i(hasTextureID, GL_TRUE);
                

                glGenBuffers(1,&uvbuffer);
                glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
                glBufferData(GL_ARRAY_BUFFER,this->mesh->uv.size()*sizeof(float),&this->mesh->uv[0],GL_STATIC_DRAW);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->mesh->texture);
                glUniform1i(this->mesh->textureID,0);

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
                glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);
            } else glUniform1i(useHeightMapID, GL_FALSE);

            //DRAW
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
                        0,                  // attribute
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                        );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

            // Draw the triangles !
            glDrawElements(
                        GL_TRIANGLES,      // mode
                        this->mesh->indices.size(),    // count
                        GL_UNSIGNED_SHORT, // type
                        (void*)0           // element array buffer offset
                        );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
/*
            glDeleteBuffers(1, &vertexbuffer);
            glDeleteBuffers(1, &elementbuffer);*/

        }

};
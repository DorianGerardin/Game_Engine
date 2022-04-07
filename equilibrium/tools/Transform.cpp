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

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

class Transform {

	public:
        //Local space information
        vec3 scale; // scale
        vec3 rotation;  // rotation
        vec3 translation;  // translation
        //Global space information concatenate in matrix
        mat4 modelMatrix = mat4(1.0f);

        vec3 selfScale; // scale
        vec3 selfRotation;  // rotation
        vec3 selfTranslation;  // translation
        mat4 self_modelMatrix = mat4(1.0f);
        bool m_isDirty = true;

    protected:
        mat4 getLocalModelMatrix()
        {
            const mat4 transformX = rotate(mat4(1.0f), radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));

            const mat4 transformY = rotate(mat4(1.0f), radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));

            const mat4 transformZ = rotate(mat4(1.0f), radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));

            // Y * X * Z
            const mat4 rotationMatrix = transformY * transformX * transformZ;

            // translation * rotation * scale (also know as TRS matrix)
            return glm::translate(glm::mat4(1.0f), translation) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
        }

        mat4 calculSelfModelMatrix()
        {
            const mat4 transformX = rotate(mat4(1.0f), radians(selfRotation.x), vec3(1.0f, 0.0f, 0.0f));

            const mat4 transformY = rotate(mat4(1.0f), radians(selfRotation.y), vec3(0.0f, 1.0f, 0.0f));

            const mat4 transformZ = rotate(mat4(1.0f), radians(selfRotation.z), vec3(0.0f, 0.0f, 1.0f));

            // Y * X * Z
            const mat4 rotationMatrix = transformY * transformX * transformZ;

            // translation * rotation * scale (also know as TRS matrix)
            return glm::translate(glm::mat4(1.0f), selfTranslation) * rotationMatrix * glm::scale(glm::mat4(1.0f), selfScale);
        }

    public:
    	Transform(){
            scale = vec3(1.0f); rotation = vec3(1.0f); translation = vec3(0.0f);
            selfScale = vec3(1.0f); selfRotation = vec3(1.0f); selfTranslation = vec3(0.f);
        } 
        Transform(vec3 s, vec3 r, vec3 t) {scale = s; rotation = r; translation = t;}
    	~Transform() {delete this;}

        void computeModelMatrix()
        {
            this->modelMatrix = getLocalModelMatrix();
            this->m_isDirty = false;
        }

        void computeSelfModelMatrix()
        {
            this->self_modelMatrix = calculSelfModelMatrix();
            this->m_isDirty = false;
        }

        void computeModelMatrix(const mat4& parentGlobalModelMatrix)
        {
            this->modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
            this->m_isDirty = false;
        }

        // Setters
        void setLocalTranslation(const vec3& newPosition)
        {
            this->translation = newPosition;
            this->m_isDirty = true;
        }

        void setLocalRotation(const glm::vec3& newRotation)
        {
            this->rotation = newRotation;
            this->m_isDirty = true;
        }

        void setLocalScale(const glm::vec3& newScale)
        {
            this->scale = newScale;
            this->m_isDirty = true;
        }

        void setSelfTranslation(const vec3& newPosition)
        {
            this->selfTranslation = newPosition;
            this->m_isDirty = true;
        }

        void setSelfRotation(const glm::vec3& newRotation)
        {
            this->selfRotation = newRotation;
            this->m_isDirty = true;
        }

        void setSelfScale(const glm::vec3& newScale)
        {
            this->selfScale = newScale;
            this->m_isDirty = true;
        }


        // Getters
        const glm::vec3& getLocalTranslation() const
        {
            return this->translation;
        }

        const glm::vec3& getLocalRotation() const
        {
            return this->rotation;
        }

        const glm::vec3& getLocalScale() const
        {
            return this->scale;
        }
        

        const glm::vec3& getSelfTranslation() const
        {
            return this->selfTranslation;
        }

        const glm::vec3& getSelfRotation() const
        {
            return this->selfRotation;
        }

        const glm::vec3& getSelfScale() const
        {
            return this->selfScale;
        }

        const glm::mat4& getModelMatrix() const
        {
            return this->modelMatrix;
        }

        const glm::mat4& getSelfModelMatrix() const
        {
            return this->self_modelMatrix;
        }

        bool isDirty() const
        {
            return this->m_isDirty;
        }

};
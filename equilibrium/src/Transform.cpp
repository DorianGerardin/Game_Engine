#include "../headers/Transform.hpp"

mat4 Transform::getLocalModelMatrix()
{
    const mat4 transformX = rotate(mat4(1.0f), radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));

    const mat4 transformY = rotate(mat4(1.0f), radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));

    const mat4 transformZ = rotate(mat4(1.0f), radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), translation) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
}

mat4 Transform::calculSelfModelMatrix()
{
    const mat4 transformX = rotate(mat4(1.0f), radians(selfRotation.x), vec3(1.0f, 0.0f, 0.0f));

    const mat4 transformY = rotate(mat4(1.0f), radians(selfRotation.y), vec3(0.0f, 1.0f, 0.0f));

    const mat4 transformZ = rotate(mat4(1.0f), radians(selfRotation.z), vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), selfTranslation) * rotationMatrix * glm::scale(glm::mat4(1.0f), selfScale);
}

Transform::Transform()
{
    scale = vec3(1.0f);
    rotation = vec3(1.0f);
    translation = vec3(0.0f);
    selfScale = vec3(1.0f);
    selfRotation = vec3(1.0f);
    selfTranslation = vec3(0.f);
}
Transform::Transform(vec3 s, vec3 r, vec3 t)
{
    scale = s;
    rotation = r;
    translation = t;
}
Transform::~Transform() { delete this; }

void Transform::computeModelMatrix()
{
    this->modelMatrix = getLocalModelMatrix();
    this->worldModelMatrix = inverse(this->modelMatrix);
    this->m_isDirty = false;
}

void Transform::computeSelfModelMatrix()
{
    this->self_modelMatrix = calculSelfModelMatrix();
    this->m_isDirty = false;
}

void Transform::computeModelMatrix(const mat4 &parentGlobalModelMatrix)
{
    this->modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
    this->worldModelMatrix = inverse(this->modelMatrix);
    this->m_isDirty = false;
    // cout << "translation : " << this->translation[0] << " " << this->translation[1] << " " << this->translation[2] << endl;
    // cout << "model matrix : " << glm::to_string(this->modelMatrix) << endl;
}

// Setters
void Transform::setLocalTranslation(const vec3 &newPosition)
{
    this->translation = newPosition;
    this->m_isDirty = true;
}

void Transform::setLocalRotation(const glm::vec3 &newRotation)
{
    this->rotation = newRotation;
    this->m_isDirty = true;
}

void Transform::setLocalScale(const glm::vec3 &newScale)
{
    this->scale = newScale;
    this->m_isDirty = true;
}

void Transform::setSelfTranslation(const vec3 &newPosition)
{
    this->selfTranslation = newPosition;
    this->m_isDirty = true;
}

void Transform::setSelfRotation(const glm::vec3 &newRotation)
{
    this->selfRotation = newRotation;
    this->m_isDirty = true;
}

void Transform::setSelfScale(const glm::vec3 &newScale)
{
    this->selfScale = newScale;
    this->m_isDirty = true;
}

// Getters
const glm::vec3 &Transform::getLocalTranslation() const
{
    return this->translation;
}

const vec3 Transform::getWorldTranslation() const
{
    // cout << this->modelMatrix[3][0] << " " << this->modelMatrix[3][1] << " " << this->modelMatrix[3][2] << endl;
    vec4 nulVec = vec4(vec3(0.), 1);
    vec4 result = this->modelMatrix * nulVec;
    // cout << "modelMatrix : " << glm::to_string(this->modelMatrix) << endl;
    // cout << "result : " << glm::to_string(result) << endl;
    return vec3(result[0], result[1], result[2]);
}

const glm::vec3 &Transform::getLocalRotation() const
{
    return this->rotation;
}

const glm::vec3 &Transform::getLocalScale() const
{
    return this->scale;
}

const glm::vec3 &Transform::getSelfTranslation() const
{
    return this->selfTranslation;
}

const glm::vec3 &Transform::getSelfRotation() const
{
    return this->selfRotation;
}

const glm::vec3 &Transform::getSelfScale() const
{
    return this->selfScale;
}

const glm::mat4 &Transform::getModelMatrix() const
{
    return this->modelMatrix;
}

const glm::mat4 &Transform::getSelfModelMatrix() const
{
    return this->self_modelMatrix;
}

bool Transform::isDirty() const
{
    return this->m_isDirty;
}

#include "../headers/PhysicsObject.hpp"

// PhysicsObject ::PhysicsObject()
// {
//     Mass = 0;
//     Gravity = vec3(0.0f, -9.81f, 0.0f);
// }

PhysicsObject::PhysicsObject(string id, string filename, GLint modelID, GLuint shader, float mass, float gravity, bool is_static)
{
    this->id = id;
    this->transform = new Transform();
    this->mesh = new Mesh(filename, modelID);
    this->shader = shader;
    this->useHeightMap = GL_FALSE;
    this->hasTexture = false;

    this->mass = mass;
    this->is_static = is_static;

    this->gravity = vec3(0.0f, gravity, 0.0f);
}

PhysicsObject::PhysicsObject(string id, int meshType, float size, GLint modelID, GLuint shader, float mass, float gravity, bool is_static)
{
    this->id = id;
    this->transform = new Transform();
    this->mesh = new Mesh(meshType, size, modelID);
    this->parent = nullptr;
    this->shader = shader;
    this->useHeightMap = GL_FALSE;
    this->hasTexture = false;

    this->mass = mass;
    this->is_static = is_static;
    this->gravity = vec3(0.0f, gravity, 0.0f);
    if (this->mesh->objectType == SPHERE)
    {
        SphereCollider *sphereCollider = new SphereCollider();
        sphereCollider->Center = this->transform->getWorldTranslation();
        sphereCollider->Radius = this->mesh->size;
        this->collider = sphereCollider;
    }
    // if (this->mesh->objectType == PLANE)
    // {
    //     SphereCollider *sphereCollider = new SphereCollider();
    //     sphereCollider->Center = this->transform->getWorldTranslation();
    //     sphereCollider->Radius = this->mesh->size;
    //     this->collider = sphereCollider;
    // }
}
PhysicsObject ::~PhysicsObject() {}

bool PhysicsObject ::isStatic()
{
    return is_static;
}

bool PhysicsObject ::isDynamic()
{
    return !is_static;
}

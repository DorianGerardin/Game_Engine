#ifndef PHYSICSOBJECT_HPP
#define PHYSICSOBJECT_HPP

#include "GameObject.hpp"
#include "Collision.hpp"
#include "../headers/Collision.hpp"

struct Collider;
class PhysicsObject : virtual public GameObject
{
public:
    vec3 Position = this->transform->getLocalTranslation();
    vec3 Vitesse = vec3(0.0f, 0.0f, 0.0f);
    vec3 Force;

    float Mass;
    bool isStatic;
    vec3 Gravity;

    // SphereCollider *sphereCollider;
    Collider *collider;

public:
    PhysicsObject();
    PhysicsObject(string id, string filename, GLint modelID, GLuint shader, float mass, float gravity);
    PhysicsObject(string id, int meshType, float size, GLint modelID, GLuint shader, float mass, float gravity);
    ~PhysicsObject();
};

#endif
#ifndef PHYSICSOBJECT_HPP
#define PHYSICSOBJECT_HPP

#include "GameObject.hpp"
#include "Collision.hpp"
#include "../headers/Collision.hpp"

struct Collider;
class PhysicsObject : virtual public GameObject
{
public:
    vec3 position = this->transform->getLocalTranslation();
    vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
    vec3 force;

    float mass;
    vec3 gravity;
    bool is_static;

    Collider *collider;

    float staticFriction = 0.5f;  // Static friction coefficient
    float kineticFriction = 0.5f; // Kinetic friction coefficient
    float restitution = 0.5f;     // Elasticity of collisions (bounciness)

public:
    PhysicsObject();
    PhysicsObject(string id, string filename, GLint modelID, GLuint shader, float mass, float gravity, bool is_static);
    PhysicsObject(string id, int meshType, float size, GLint modelID, GLuint shader, float mass, float gravity, bool is_static);
    ~PhysicsObject();

    bool isStatic();
    bool isDynamic();
};

#endif
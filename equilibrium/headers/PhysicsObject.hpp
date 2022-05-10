#ifndef PHYSICSOBJECT_HPP
#define PHYSICSOBJECT_HPP

#include "GameObject.hpp"
#include "Collision.hpp"
#include <functional> //std::function

struct Collider;

enum TriggerType
{
    MOVE_TO_LAST_CHECKPOINT,
    UPDATE_LAST_CHECKPOINT,
    LEVEL_COMPLETE
};

class PhysicsObject : public GameObject
{
public:
    vec3 position = this->transform->getLocalTranslation();
    vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
    float maxVelocity = 4.75f;
    float maxVelocityZ = 5.0f;
    vec3 force;

    float mass;
    vec3 gravity;
    vec3 acceleration = vec3(0.0f, 0.0f, 0.0f);
    bool is_player = false;
    bool is_static = true;
    bool is_trigger = false;
    std::function<void(PhysicsObject *)> onCollision;

    vec3 lastCheckpoint = vec3(5.0f, 0.5f, 5.0f);

    Collider *collider;

    float staticFriction = 0.8f;  // Static friction coefficient
    float kineticFriction = 0.7f; // Kinetic friction coefficient
    float restitution = 0.8f;     // Elasticity of collisions (bounciness)

    MaterialType materialType;
    MaterialType materialTypeOnSurface;

    bool onSurface = false;             // Utile pour limiter le saut
    PhysicsObject *sphereOnPlaneObject; // Utile pour set les coefficients staticFriction et kineticFriction

public:
    PhysicsObject();
    PhysicsObject(string id, string filename, float size, GLint modelID, GLuint shader, float mass, float gravity, bool is_static);
    PhysicsObject(string id, int meshType, float size, GLint modelID, GLuint shader, float mass, float gravity, bool is_static);
    ~PhysicsObject();

    void addVisualSphereRotation();
    void setPhysicsCoeffs(float, float, float);

    bool isPlayer();
    bool isStatic();
    bool isDynamic();
    bool isTrigger();

    void setPlayer(bool);
    void setTrigger(bool);
    void setTriggerType(int);

    void moveToLastCheckpoint(PhysicsObject *);
    void updateLastCheckpoint(PhysicsObject *);
    void levelComplete(PhysicsObject *);
};

#endif
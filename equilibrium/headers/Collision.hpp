#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Transform.hpp"
#include "PhysicsObject.hpp"
struct CollisionPoints
{
    vec3 A;      // Furthest point of A into B
    vec3 B;      // Furthest point of B into A
    vec3 Normal; // B – A normalized
    float Depth; // Length of B – A
    bool HasCollision = false;
};

//-------------------------------------------------------------------------------------------------
class SphereCollider;
class PlaneCollider;
class AABBCollider;
class PhysicsObject;

//-------------------------------------------------------------------------------------------------

CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform *ta,
                                                const SphereCollider *b, const Transform *tb);
CollisionPoints FindSpherePlaneCollisionPoints(const SphereCollider *a, const Transform *ta,
                                               const PlaneCollider *b, const Transform *tb);
CollisionPoints FindSphereAABBCollisionPoints(const SphereCollider *a, const Transform *ta,
                                              const AABBCollider *b, const Transform *tb);
CollisionPoints FindAABBAABBCollisionPoints(const AABBCollider *a, const Transform *ta,
                                            const AABBCollider *b, const Transform *tb);

class Collider
{
public:
    virtual CollisionPoints TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const = 0;

    virtual CollisionPoints TestCollision(
        const Transform *transform,
        const SphereCollider *sphere,
        const Transform *sphereTransform) const = 0;

    virtual CollisionPoints TestCollision(
        const Transform *transform,
        const PlaneCollider *plane,
        const Transform *planeTransform) const = 0;

    virtual CollisionPoints TestCollision(
        const Transform *transform,
        const AABBCollider *aabb,
        const Transform *aabbTransform) const = 0;
};

class SphereCollider : public Collider
{
public:
    vec3 Center;
    float Radius;

    CollisionPoints TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const override
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const SphereCollider *sphere,
        const Transform *sphereTransform) const override
    {
        return FindSphereSphereCollisionPoints(this, transform, sphere, sphereTransform);
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const PlaneCollider *plane,
        const Transform *planeTransform) const override
    {
        return FindSpherePlaneCollisionPoints(this, transform, plane, planeTransform);
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const AABBCollider *aabb,
        const Transform *aabbTransform) const override
    {
        return FindSphereAABBCollisionPoints(this, transform, aabb, aabbTransform);
    }
};

class PlaneCollider : public Collider
{
public:
    vec3 Normal; // vecteur normal au plan
    float Distance;

    CollisionPoints TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const override
    {

        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const SphereCollider *sphere,
        const Transform *sphereTransform) const override
    {
        // //reuse sphere code
        // return FindPlaneSphereCollisionPoints(this, transform, sphere, sphereTransform);
        // return FindSpherePlaneCollisionPoints(sphere, sphereTransform, this, transform);

        CollisionPoints points = sphere->TestCollision(sphereTransform, this, transform);

        // swap
        glm::vec3 T = points.A;
        points.A = points.B;
        points.B = T;

        points.Normal = -points.Normal;

        return points;
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const PlaneCollider *plane,
        const Transform *planeTransform) const override
    {
        return {}; // No plane v plane
    }
    CollisionPoints TestCollision(
        const Transform *transform,
        const AABBCollider *aabb,
        const Transform *aabbTransform) const override
    {
        return {}; // No plane v aabb
    }
};

class AABBCollider : public Collider
{
public:
    vec3 minValue;
    vec3 maxValue;

    CollisionPoints TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const override
    {

        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const SphereCollider *sphere,
        const Transform *sphereTransform) const override
    {
        // CollisionPoints points = sphere->TestCollision(sphereTransform, this, transform);

        // // swap
        // glm::vec3 T = points.A;
        // points.A = points.B;
        // points.B = T;

        // points.Normal = -points.Normal;

        // return points;

        return FindSphereAABBCollisionPoints(sphere, transform, this, sphereTransform);
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const PlaneCollider *plane,
        const Transform *planeTransform) const override
    {
        return {}; // No AABB v plane
    }

    CollisionPoints TestCollision(
        const Transform *transform,
        const AABBCollider *aabb,
        const Transform *aabbTransform) const override
    {
        return FindAABBAABBCollisionPoints(this, transform, aabb, aabbTransform);
    }

    vec3 closestPointAABB(const vec3 point, const Transform *tb) const;
    float SqDistPointAABB(const vec3 point, const Transform *tb) const;
};

//-------------------------------------------------------------------------------------------------
struct Collision
{
    PhysicsObject *ObjA;
    PhysicsObject *ObjB;
    CollisionPoints Points;
};
//-------------------------------------------------------------------------------------------------
// Implémenter Impulse ou Postition Solver
class Solver
{
public:
    virtual void Solve(vector<Collision> &collisions, float dt) = 0;
    virtual string returnSolverID() = 0;
};

class PositionSolver : public Solver
{
public:
    // vec3 delta = vec3(0.1f, 0.1f, 0.1f);
    // float delta = 0.05f;
    // vec3 old_resolution = vec3(0.0f, 0.0f, 0.0f);

public:
    void Solve(vector<Collision> &collisions, float dt);
    string returnSolverID() { return "PositionSolver"; }
};

class ImpulseSolver : public Solver
{

public:
    void Solve(vector<Collision> &collisions, float dt);
    string returnSolverID() { return "ImpulseSolver"; }
};

#endif
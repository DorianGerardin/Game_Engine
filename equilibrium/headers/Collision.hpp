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
    bool HasCollision;
};

//-------------------------------------------------------------------------------------------------
class SphereCollider;
class PhysicsObject;

//-------------------------------------------------------------------------------------------------

// namespace algo
// {
CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform *ta, const SphereCollider *b, const Transform *tb);
// CollisionPoints FindSpherePlaneCollisionPoints(
//     const SphereCollider *a, const Transform *ta,
//     const PlaneCollider *b, const Transform *tb);

// CollisionPoints FindPlaneSphereCollisionPoints(
//     const PlaneCollider *a, const Transform *ta,
//     const SphereCollider *b, const Transform *tb);
// }

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

    // virtual CollisionPoints TestCollision(
    //     const Transform *transform,
    //     const PlaneCollider *plane,
    //     const Transform *planeTransform) const = 0;
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

    // CollisionPoints TestCollision(
    //     const Transform *transform,
    //     const PlaneCollider *plane,
    //     const Transform *planeTransform) const override
    // {
    //     return algo::FindSpherePlaneCollisionPoints(this, transform, plane, planeTransform);
    // }
};

// class PlaneCollider : Collider
// {
// public:
//     vec3 Plane; // vecteur
//     float Distance;

//     CollisionPoints TestCollision(
//         const Transform *transform,
//         const Collider *collider,
//         const Transform *colliderTransform) const override
//     {
//         return collider->TestCollision(colliderTransform, this, transform);
//     }

//     CollisionPoints TestCollision(
//         const Transform *transform,
//         const SphereCollider *sphere,
//         const Transform *sphereTransform) const override
//     {
//         // reuse sphere code
//         return algo::FindPlaneSphereCollisionPoints(this, transform, sphere, sphereTransform);
//     }

//     CollisionPoints TestCollision(
//         const Transform *transform,
//         const PlaneCollider *plane,
//         const Transform *planeTransform) const override
//     {
//         return {}; // No plane v plane
//     }
// };

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
};

#endif
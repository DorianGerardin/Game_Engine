#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Transform.hpp"
struct CollisionPoints
{
    vec3 A;      // Furthest point of A into B
    vec3 B;      // Furthest point of B into A
    vec3 Normal; // B – A normalized
    float Depth; // Length of B – A
    bool HasCollision;
};

struct Collider
{
    virtual CollisionPoints TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const = 0;

    // virtual CollisionPoints TestCollision(
    //     const Transform *transform,
    //     const SphereCollider *sphere,
    //     const Transform *sphereTransform) const = 0;

    // virtual CollisionPoints TestCollision(
    //     const Transform *transform,
    //     const PlaneCollider *plane,
    //     const Transform *planeTransform) const = 0;
};

// struct SphereCollider : Collider
// {
//     vec3 Center;
//     float Radius;

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
//         return algo::FindSphereSphereCollisionPoints(
//             this, transform, sphere, sphereTransform);
//     }

//     CollisionPoints TestCollision(
//         const Transform *transform,
//         const PlaneCollider *plane,
//         const Transform *planeTransform) const override
//     {
//         return algo::FindSpherePlaneCollisionPoints(
//             this, transform, plane, planeTransform);
//     }
// };

// struct PlaneCollider : Collider
// {
//     vec3 Plane;
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
//         return sphere->TestCollision(sphereTransform, this, transform);
//     }

//     CollisionPoints TestCollision(
//         const Transform *transform,
//         const PlaneCollider *plane,
//         const Transform *planeTransform) const override
//     {
//         return {}; // No plane v plane
//     }
// };
#endif
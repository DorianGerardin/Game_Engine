#include "../headers/Collision.hpp"

CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform *ta, const SphereCollider *b, const Transform *tb)
{
    vec3 A = a->Center + ta->getWorldTranslation();
    vec3 B = b->Center + tb->getWorldTranslation();

    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;
    float Br = b->Radius * tb->getLocalScale().x;

    vec3 AtoB = B - A;
    vec3 BtoA = A - B;

    if (AtoB.length() > Ar + Br)
    {
        return {
            vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 0.0f, 0.0f),
            0,
            false};
    }

    A += normalize(AtoB) * Ar;
    B += normalize(BtoA) * Br;

    AtoB = B - A;

    return {
        A, B,
        normalize(AtoB),
        AtoB.length(),
        true};
}
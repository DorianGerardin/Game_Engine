#include "../headers/Collision.hpp"

CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform *ta, const SphereCollider *b, const Transform *tb)
{
    vec3 A = a->Center + ta->getWorldTranslation();
    vec3 B = b->Center + tb->getWorldTranslation();

    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;
    float Br = b->Radius * tb->getLocalScale().x;

    // cout << ta->getLocalScale().x << endl;

    vec3 AtoB = B - A;
    vec3 BtoA = A - B;

    if (length(AtoB) > Ar + Br)
    {
        // cout << "PAS DE COLLISION" << endl;
        return {
            vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 0.0f, 0.0f),
            0,
            false};
    }

    // cout << "A de centre " << A << " et de rayon " << Ar << endl;
    // cout << "B de centre " << B << " et de rayon " << Br << endl;
    // cout << "\n"
    //      << endl;
    A += normalize(AtoB) * Ar;
    B += normalize(BtoA) * Br;

    AtoB = B - A;

    return {
        A, B,
        normalize(AtoB),
        AtoB.length(),
        true};
}
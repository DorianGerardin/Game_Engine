#include "../headers/Collision.hpp"

// ----------------------------------------------------------------------------------------------------
// |                                      COLLISION POINTS                                            |
// ----------------------------------------------------------------------------------------------------
CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform *ta,
                                                const SphereCollider *b, const Transform *tb)
{
    // Attention :  doit être la translation MONDE
    vec3 A = a->Center + ta->getLocalTranslation();
    vec3 B = b->Center + tb->getLocalTranslation();

    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;
    float Br = b->Radius * tb->getLocalScale().x;

    // cout << "A de centre " << A << " et de rayon " << Ar << endl;
    // cout << "B de centre " << B << " et de rayon " << Br << endl;

    vec3 AtoB = B - A;
    vec3 BtoA = A - B;

    // Pas de collision
    if (length(AtoB) > Ar + Br)
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

    // Collision
    return {
        A, B,
        normalize(AtoB),
        length(AtoB),
        true};
}

CollisionPoints FindSpherePlaneCollisionPoints(const SphereCollider *a, const Transform *ta,
                                               const PlaneCollider *b, const Transform *tb)
{
    // Attention :  doit être la translation MONDE
    vec3 A = a->Center + ta->getLocalTranslation();

    // Utiliser des quaternions
    // vec3 N = b->Plane * tb->getLocalRotation();
    // N = normalize(N);
    vec3 N = normalize(b->Normal);

    vec3 P = N * b->Distance + tb->getLocalTranslation();

    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;

    // distance du centre de la sphere à la surface du plan
    float d = dot(A - P, N);

    // cout << "A de centre " << A << " et de rayon " << Ar << endl;
    // cout << "B de vecteur normal " << N << endl;

    // Pas de collision
    if (d > Ar)
    {
        return {
            vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 0.0f, 0.0f),
            0,
            false};
    }

    vec3 B = A - N * d;
    A = A - N * Ar;

    // Collision
    return {
        A, B,
        normalize(N),
        length(B - A),
        true};
}

// CollisionPoints FindPlaneSphereCollisionPoints(const PlaneCollider *a, const Transform *ta,
//                                                const SphereCollider *b, const Transform *tb)
// {
// }

// vec3 AABBCollider::closestPointAABB(vec3 point) const
// {
//     // For each coordinate axis, if the point coordinate value is outside box,
//     // clamp it to the box, else keep it as is
//     vec3 min = this->minValue;
//     vec3 max = this->maxValue;
//     vec3 q = vec3(0.0f, 0.0f, 0.0f);
//     float v = 0;
//     v = point.x;
//     v = std::max(v, min.x);
//     v = std::min(v, max.x);
//     q.x = v;
//     v = point.y;
//     v = std::max(v, min.y);
//     v = std::min(v, max.y);
//     q.y = v;
//     v = point.z;
//     v = std::max(v, min.z);
//     v = std::min(v, max.z);
//     q.z = v;

//     return q;
// }

// Game Physics Cookbook p.162
vec3 AABBCollider::closestPointAABB(vec3 point) const
{
    vec3 result = point;
    vec3 min = this->minValue;
    vec3 max = this->maxValue;
    // Clamp the closest point to the min point of the AABB:
    result.x = (result.x < min.x) ? min.x : result.x;
    result.y = (result.y < min.x) ? min.y : result.y;
    result.z = (result.z < min.x) ? min.z : result.z;
    // Clamp the closest point to the max point of the AABB:
    result.x = (result.x > max.x) ? max.x : result.x;
    result.y = (result.y > max.x) ? max.y : result.y;
    result.z = (result.z > max.x) ? max.z : result.z;
    return result;
}
float AABBCollider::SqDistPointAABB(vec3 point) const
{
    float result = length(this->closestPointAABB(point) - point);
    return result;
}
CollisionPoints FindSphereAABBCollisionPoints(const SphereCollider *a, const Transform *ta,
                                              const AABBCollider *b, const Transform *tb)
{
    float dist;

    vec3 A = a->Center + ta->getLocalTranslation();
    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;

    // Find point (p) on AABB closest to Sphere center
    vec3 p = b->closestPointAABB(A);

    // Sphere and AABB intersect if the (squared) distance from sphere center to point (p)
    // is less than the (squared) sphere radius
    vec3 v = A - p;

    if (dot(v, v) <= Ar * Ar)
    {
        // cout << "COLLISION" << endl;
        dist = b->SqDistPointAABB(A);

        // Calculate normal using sphere center a closest point on AABB
        vec3 normal = normalize(p - A);

        vec3 B = p - normal * Ar;
        vec3 AtoB = B - A;

        if (AtoB == vec3(0.0f, 0.0f, 0.0f))
        {
            // Sphere is inside AABB
            AtoB = vec3(0.0f, 1.0f, 0.0f);
        }

        float dsqrd = length(AtoB);

        return {
            a->Center + ta->getLocalTranslation(), B,
            normalize(AtoB),
            length(AtoB),
            true};
    }
    else
    {
        // cout << "PAS COLLISION" << endl;
        // No intersection
        return {
            vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 0.0f, 0.0f),
            0,
            false};
    }
}

// ------------------------------------------------------------------------------------------
// |                                      SOLVER                                            |
// ------------------------------------------------------------------------------------------
void PositionSolver::Solve(vector<Collision> &collisions, float dt)
{
    for (Collision &collision : collisions)
    {
        // cout << "PositionSolver" << endl;
        PhysicsObject *aBody = collision.ObjA;
        PhysicsObject *bBody = collision.ObjB;

        vec3 resolution = collision.Points.B - collision.Points.A;
        // cout << this->old_resolution - resolution << endl;
        // if (resolution.length() < 0.1)
        // {
        if (aBody->isDynamic())
        {
            // aBody->velocity -= resolution;
            vec3 aBodyNewTranslation = aBody->transform->getLocalTranslation() - resolution;
            aBody->transform->setLocalTranslation(aBodyNewTranslation);
        }

        if (bBody->isDynamic())
        {
            vec3 bBodyNewTranslation = bBody->transform->getLocalTranslation() + resolution;
            bBody->transform->setLocalTranslation(bBodyNewTranslation);
        }
        // }
        // else
        // {
        //     cout << "trop petit déplacement" << endl;
        // }
    }
}

void ImpulseSolver::Solve(vector<Collision> &collisions, float dt)
{
    for (Collision &collision : collisions)
    {
        // cout << "ImpulseSolver" << endl;
        // cout << collision.ObjA->id << endl;
        // Test for is each objects is dynamic or not
        PhysicsObject *aBody, *bBody;
        if (collision.ObjA->isDynamic())
        {
            aBody = (PhysicsObject *)collision.ObjA;
            // cout << aBody->id << " :  ";
        }
        else
            aBody = nullptr;
        if (collision.ObjB->isDynamic())
            bBody = (PhysicsObject *)collision.ObjB;
        else
            bBody = nullptr;

        // Calcule les Vitesses des objets A et B et leur Vitesses relatives
        vec3 aVitesse, bVitesse, rVitesse;
        if (aBody)
        {
            // cout << "aBody->velocity = " << aBody->velocity << endl;
            aVitesse = aBody->velocity;
            // cout << "aVitesse = " << aVitesse << endl;
        }
        else
            aVitesse = vec3(0.0f, 0.0f, 0.0f);
        if (bBody)
            bVitesse = bBody->velocity;
        else
            bVitesse = vec3(0.0f, 0.0f, 0.0f);

        rVitesse = bVitesse - aVitesse;
        // cout << "bVitesse = " << bVitesse << ", aVitesse = " << aVitesse << endl;
        float nVitesse = dot(rVitesse, collision.Points.Normal);
        // cout << "rVitesse = " << rVitesse << ", collision.Points.Normal = " << collision.Points.Normal << endl;
        float aInvMasse, bInvMasse;
        if (aBody)
            aInvMasse = (float)(1.0f / aBody->mass);
        else
            aInvMasse = 0.0f;
        if (bBody)
            bInvMasse = (float)(1.0f / bBody->mass);
        else
            bInvMasse = 0.0f;

        //-------------------------------------------------------------------------------------------------
        // Impluse

        if (nVitesse >= 0)
            continue;
        float e, j;
        if (aBody && bBody)
            e = aBody->restitution * bBody->restitution;
        else
            e = 0.0f;

        j = -(1.0f + e) * nVitesse / (aInvMasse + bInvMasse);

        vec3 impluse = j * collision.Points.Normal;

        if (aBody ? aBody->isDynamic() : false)
        {
            aVitesse -= impluse * aInvMasse;
        }

        if (bBody ? bBody->isDynamic() : false)
        {
            bVitesse += impluse * bInvMasse;
        }

        //-------------------------------------------------------------------------------------------------
        // Friction

        rVitesse = bVitesse - aVitesse;
        nVitesse = dot(rVitesse, collision.Points.Normal);

        vec3 tangent = normalize(rVitesse - nVitesse * collision.Points.Normal);
        if (isnan(tangent.x) || isnan(tangent.y) || isnan(tangent.z))
            tangent = glm::vec3(0.0f);
        float fVitesse = dot(rVitesse, tangent);

        float aSF = aBody ? aBody->staticFriction : 0.0f;
        float bSF = bBody ? bBody->staticFriction : 0.0f;
        float aKF = aBody ? aBody->kineticFriction : 0.0f;
        float bKF = bBody ? bBody->kineticFriction : 0.0f;
        float mu = length(vec2(aSF, bSF));

        float f = -fVitesse / (aInvMasse + bInvMasse);

        vec3 friction;
        if (abs(f) < j * mu)
        {
            friction = f * tangent;
            // cout << "friction = " << friction << endl;
        }

        else
        {
            mu = length(vec2(aKF, bKF));
            friction = -j * tangent * mu;
            // cout << "friction = " << friction << endl;
            // cout << "tangent = " << tangent << endl;
        }

        if (aBody ? aBody->isDynamic() : false)
        {
            // cout << "aVitesse = " << aVitesse << ", friction = " << friction << ", aInvMasse = " << aInvMasse << endl;
            aBody->velocity = aVitesse - friction * aInvMasse;
        }

        if (bBody ? bBody->isDynamic() : false)
        {
            bBody->velocity = bVitesse + friction * bInvMasse;
        }
    }
}
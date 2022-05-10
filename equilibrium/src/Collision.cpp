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

    vec3 normal = vec3(0.0f, 0.0f, 1.0f);
    if (tb->getLocalRotation() == vec3(0.0f, 0.0f, 0.0f))
        normal = vec3(0.0f, 0.0f, 1.0f);
    if (tb->getLocalRotation().x == 90.0f)
        normal = vec3(0.0f, -1.0f, 0.0f);
    if (tb->getLocalRotation().y == 90.0f)
        normal = vec3(-1.0f, 0.0f, 0.0f);

    vec3 N = normalize(normal);

    vec3 P = N * b->Distance + tb->getLocalTranslation();

    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;

    // distance du centre de la sphere à la surface du plan
    float d = dot(A - P, N);

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

// Game Physics Cookbook p.162
vec3 AABBCollider::closestPointAABB(const vec3 point, const Transform *tb) const
{
    vec3 result = point;
    vec3 min = vec3(this->minValue.x * tb->getLocalScale().x, this->minValue.y * tb->getLocalScale().y, this->minValue.z * tb->getLocalScale().z) + tb->getLocalTranslation();
    vec3 max = vec3(this->maxValue.x * tb->getLocalScale().x, this->maxValue.y * tb->getLocalScale().y, this->maxValue.z * tb->getLocalScale().z) + tb->getLocalTranslation();

    // Clamp the closest point to the min point of the AABB:
    result.x = std::max(result.x, min.x);
    result.y = std::max(result.y, min.y);
    result.z = std::max(result.z, min.z);

    // Clamp the closest point to the max point of the AABB:
    result.x = std::min(result.x, max.x);
    result.y = std::min(result.y, max.y);
    result.z = std::min(result.z, max.z);
    return result;
}

CollisionPoints FindSphereAABBCollisionPoints(const SphereCollider *a, const Transform *ta,
                                              const AABBCollider *b, const Transform *tb)
{
    vec3 A = a->Center + ta->getLocalTranslation();
    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;

    // Find point (p) on AABB closest to Sphere center
    vec3 p = b->closestPointAABB(A, tb);

    // Sphere and AABB intersect if the (squared) distance from sphere center to point (p)
    // is less than the (squared) sphere radius
    vec3 v = A - p;

    if (dot(v, v) <= Ar * Ar)
    {
        // Calculate normal using sphere center a closest point on AABB
        vec3 normal = normalize(p - A);

        vec3 B = p - normal * Ar;
        vec3 AtoB = B - A;

        if (AtoB == vec3(0.0f, 0.0f, 0.0f))
        {
            // Sphere is inside AABB
            AtoB = vec3(0.0f, 1.0f, 0.0f);
        }

        return {
            A, B,
            normalize(AtoB),
            length(AtoB),
            true};
    }
    else
    {
        // No intersection
        return {
            vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 0.0f, 0.0f),
            0,
            false};
    }
}

CollisionPoints FindAABBAABBCollisionPoints(const AABBCollider *a, const Transform *ta,
                                            const AABBCollider *b, const Transform *tb)
{
    // Find the min and max points of the first AABB
    vec3 aMin = a->minValue * ta->getLocalScale() + ta->getLocalTranslation();
    vec3 aMax = a->maxValue * ta->getLocalScale() + ta->getLocalTranslation();
    // 4. Find the min and max points of the second AABB
    vec3 bMin = b->minValue * tb->getLocalScale() + tb->getLocalTranslation();
    vec3 bMax = b->maxValue * tb->getLocalScale() + tb->getLocalTranslation();
    // 5. Check for overlap with the min and max points of the rectangles
    if ((aMin.x < bMax.x && aMax.x > bMin.x) && (aMin.y < bMax.y && aMax.y > bMin.y) && (aMin.z < bMax.z && aMax.z > bMin.z))
    {
        vec3 A = vec3(0.0f, 0.0f, 0.0f);
        vec3 B = vec3(0.0f, 0.0f, 0.0f);
        vec3 AtoB = vec3(0.0f, 0.0f, 0.0f);
        if (bMax.z > aMax.z)
        {
            A = vec3(aMin.x, bMax.y, bMax.z);
            B = bMin;
            A = vec3(bMin.x, bMin.y, aMax.z);
        }
        else if (bMax.z < aMax.z && bMin.x < aMin.x)
        {
            B = bMax;
            A = vec3(aMin.x, bMax.y, bMax.z);
        }
        else if (bMax.z < aMax.z && aMax.x > bMin.x)
        {
            B = bMin;
            A = vec3(aMax.x, bMin.y, bMin.z);
        }
        else
        {
            return {
                vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
                vec3(0.0f, 0.0f, 0.0f),
                0,
                false};
        }

        AtoB = B - A;

        return {
            A, B,
            normalize(AtoB),
            length(AtoB),
            true};
    }
    else
    {

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
        PhysicsObject *aBody = collision.ObjA;
        PhysicsObject *bBody = collision.ObjB;

        vec3 resolution = collision.Points.B - collision.Points.A;

        if (aBody->isDynamic())
        {
            vec3 aBodyNewTranslation = aBody->transform->getLocalTranslation() - resolution;
            aBody->transform->setLocalTranslation(aBodyNewTranslation);
        }

        if (bBody->isDynamic())
        {
            vec3 bBodyNewTranslation = bBody->transform->getLocalTranslation() + resolution;
            bBody->transform->setLocalTranslation(bBodyNewTranslation);
        }
    }
}

void ImpulseSolver::Solve(vector<Collision> &collisions, float dt)
{
    for (Collision &collision : collisions)
    {
        PhysicsObject *aBody, *bBody;
        if (collision.ObjA->isDynamic())
        {
            aBody = (PhysicsObject *)collision.ObjA;
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
            aVitesse = aBody->velocity;
        }
        else
            aVitesse = vec3(0.0f, 0.0f, 0.0f);
        if (bBody)
            bVitesse = bBody->velocity;
        else
            bVitesse = vec3(0.0f, 0.0f, 0.0f);

        rVitesse = bVitesse - aVitesse;
        float nVitesse = dot(rVitesse, collision.Points.Normal);
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
        }

        else
        {
            mu = length(vec2(aKF, bKF));
            friction = -j * tangent * mu;
        }

        if (aBody ? aBody->isDynamic() : false)
        {
            aBody->velocity = aVitesse - friction * aInvMasse;
        }

        if (bBody ? bBody->isDynamic() : false)
        {
            bBody->velocity = bVitesse + friction * bInvMasse;
        }
    }
}
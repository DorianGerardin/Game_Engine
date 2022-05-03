#include "../headers/Collision.hpp"

CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform *ta, const SphereCollider *b, const Transform *tb)
{
    vec3 A = a->Center + ta->getWorldTranslation();
    vec3 B = b->Center + tb->getWorldTranslation();

    // Attention :  doit être le scale MONDE
    float Ar = a->Radius * ta->getLocalScale().x;
    float Br = b->Radius * tb->getLocalScale().x;

    cout << "A de centre " << A << " et de rayon " << Ar << endl;
    cout << "B de centre " << B << " et de rayon " << Br << endl;

    vec3 AtoB = B - A;
    vec3 BtoA = A - B;

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

    return {
        A, B,
        normalize(AtoB),
        AtoB.length(),
        true};
}

void PositionSolver::Solve(vector<Collision> &collisions, float dt)
{
    for (Collision &collision : collisions)
    {
        // cout << "PositionSolver" << endl;
        PhysicsObject *aBody = collision.ObjA;
        PhysicsObject *bBody = collision.ObjB;

        vec3 resolution = collision.Points.B - collision.Points.A;
        // cout << resolution << endl;
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
        // cout << "ImpulseSolver" << endl;

        // Test for is each objects is dynamic or not
        PhysicsObject *aBody, *bBody;
        if (collision.ObjA->isDynamic())
            aBody = (PhysicsObject *)collision.ObjA;
        else
            aBody = nullptr;
        if (collision.ObjB->isDynamic())
            bBody = (PhysicsObject *)collision.ObjB;
        else
            bBody = nullptr;

        // Calcule les Vitesses des objets A et B et leur Vitesses relatives
        vec3 aVitesse, bVitesse, rVitesse;
        if (aBody)
            aVitesse = aBody->velocity;
        else
            aVitesse = vec3(0.0f, 0.0f, 0.0f);
        if (bBody)
            bVitesse = aBody->velocity;
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
        float fVitesse = dot(rVitesse, tangent);

        float aSF = aBody ? aBody->staticFriction : 0.0f;
        float bSF = bBody ? bBody->staticFriction : 0.0f;
        float aKF = aBody ? aBody->kineticFriction : 0.0f;
        float bKF = bBody ? bBody->kineticFriction : 0.0f;
        float mu = vec2(aSF, bSF).length();

        float f = -fVitesse / (aInvMasse + bInvMasse);

        vec3 friction;
        if (abs(f) < j * mu)
        {
            friction = f * tangent;
        }

        else
        {
            mu = vec2(aKF, bKF).length();
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
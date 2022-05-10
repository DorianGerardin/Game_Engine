#include "../headers/Scene.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
    delete this;
}

void Scene::addObject(GameObject *o)
{
    this->objects.push_back(o);
}

void Scene::addCamera(CameraObject *cam)
{
    this->cameras.push_back(cam);
}

void Scene::addLight(LightObject *light)
{
    this->LightList.push_back(light);
}

void Scene::addPlayer(PhysicsObject *player)
{
    player->setPlayer(true);
    this->player = player;
}

PhysicsObject *Scene::getPlayer()
{
    return this->player;
}

void Scene::addPhysicsObject(PhysicsObject *physicsObject)
{
    addObject(physicsObject);
    this->PhysicsObjectList.push_back(physicsObject);
}
void Scene::removePhysicsObject(PhysicsObject *physicsObject)
{
    if (!physicsObject)
        return;
    auto itr = find(PhysicsObjectList.begin(), PhysicsObjectList.end(), physicsObject);
    if (itr == PhysicsObjectList.end())
        return;
    PhysicsObjectList.erase(itr);
}
void Scene::addSolver(Solver *solver)
{
    this->SolverList.push_back(solver);
}
void Scene::removeSolver(Solver *solver)
{
    if (!solver)
        return;
    auto itr = find(SolverList.begin(), SolverList.end(), solver);
    if (itr == SolverList.end())
        return;
    SolverList.erase(itr);
}
void Scene::Step(float dt)
{
    ResolveCollisions(dt);
    float radiusPlayer = player->mesh->size * player->transform->getLocalScale().x;
    for (PhysicsObject *obj : PhysicsObjectList)
    {
        if (obj->isDynamic())
        {
            // Force de gravité : Poids : W = m*g
            // Force résultante :         F = m*a
            obj->force = obj->mass * (obj->gravity + obj->acceleration); // apply a force

            obj->velocity += (obj->force / obj->mass) * dt;

            // Clamp max velocity
            if (obj->velocity.x > obj->maxVelocity)
                obj->velocity.x = obj->maxVelocity;
            if (obj->velocity.x < -obj->maxVelocity)
                obj->velocity.x = -obj->maxVelocity;
            if (obj->velocity.y > obj->maxVelocity)
                obj->velocity.y = obj->maxVelocity;
            if (obj->velocity.y < -obj->maxVelocity)
                obj->velocity.y = -obj->maxVelocity;
            if (obj->velocity.z < -obj->maxVelocityZ)
                obj->velocity.z = -obj->maxVelocityZ;

            // x = x0 + vt
            obj->position = obj->transform->getLocalTranslation() + obj->velocity * dt;

            obj->transform->setLocalTranslation(obj->position);

            if (obj->hasRotationObject)
            {
                if ((abs(obj->velocity.x) > 0.002 || abs(obj->velocity.y) > 0.002 || abs(obj->velocity.z) > 0.002) && obj->id != this->player->id)
                {
                    vec3 actualRotation = obj->rotationObject->transform->getLocalRotation();
                    float radius = radiusPlayer / (obj->mesh->size * obj->transform->getLocalScale().x);
                    obj->rotationObject->transform->setLocalRotation(actualRotation - vec3(obj->velocity.y * dt * 150 * radius, -obj->velocity.x * dt * 150 * radius, 0));
                }
            }

            obj->acceleration = vec3(0, 0, 0); // réinitialise l'accélération
        }
    }
}

void Scene::SendCollisionCallbacks(vector<Collision> &collisions, float dt)
{
    for (Collision &collision : collisions)
    {
        auto &a = collision.ObjA->onCollision;
        auto &b = collision.ObjB->onCollision;

        if (a)
            a(collision.ObjB);
        if (b)
            b(collision.ObjA);
    }
}
void Scene::ResolveCollisions(float dt)
{
    // Generate List of collisions
    std::vector<Collision> collisions;
    std::vector<Collision> triggers;
    for (PhysicsObject *a : PhysicsObjectList)
    {
        for (PhysicsObject *b : PhysicsObjectList)
        {

            if (a == b) // unique pairs
                break;

            if (!a->collider || !b->collider) // both have colliders
            {
                continue;
            }
            CollisionPoints points = a->collider->TestCollision(a->transform, b->collider, b->transform);

            if (points.HasCollision)
            {
                Collision collision;
                collision.ObjA = a;
                collision.ObjB = b;
                collision.Points = points;

                if (a->is_trigger || b->is_trigger)
                {
                    triggers.emplace_back(collision);
                }
                else
                {
                    collisions.emplace_back(collision);

                    if (a->isPlayer() && (b->mesh->objectType == PLANE || b->mesh->objectType == CUBE))
                    {
                        a->onSurface = true;
                        a->sphereOnPlaneObject = b;
                        a->setPhysicsCoeffs(b->staticFriction, b->kineticFriction, b->restitution);
                    }
                    else if ((a->mesh->objectType == PLANE || a->mesh->objectType == CUBE) && b->isPlayer())
                    {
                        b->onSurface = true;
                        b->sphereOnPlaneObject = a;
                        b->setPhysicsCoeffs(a->staticFriction, a->kineticFriction, a->restitution);
                    }
                }
            }
        }
    }

    // Solve Collisions (not triggers)
    for (Solver *solver : SolverList)
    {
        solver->Solve(collisions, dt);
    }

    SendCollisionCallbacks(triggers, dt);
}

void Scene::update()
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (!objects[i]->parent)
        {
            objects[i]->updateSelfAndChild();
        }
    }

    for (int i = 0; i < PhysicsObjectList.size(); i++)
    {
        if (!PhysicsObjectList[i]->parent)
        {
            PhysicsObjectList[i]->updateSelfAndChild();
        }
    }
}

GameObject *Scene::getGameObjectFromId(string id)
{
    for (auto &object : this->objects) // access by reference to avoid copying
    {
        if (object->id == id)
        {
            return object;
        }
    }
    return NULL;
}

PhysicsObject *Scene::getPhysicsObjectFromId(string id)
{

    for (auto &object : this->PhysicsObjectList) // access by reference to avoid copying
    {
        if (object->id == id)
        {
            cout << "object->id " << object->id << endl;
            return object;
        }
    }
    return NULL;
}

void Scene::draw()
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->toDraw)
        {
            glUseProgram(LightList[0]->shader);
            glUniform1i(glGetUniformLocation(LightList[0]->shader, "nbLights"), LightList.size());
            for (int j = 0; j < LightList.size(); j++)
            {
                LightList[j]->draw(j);
            }

            objects[i]->draw();
        }
    }
}
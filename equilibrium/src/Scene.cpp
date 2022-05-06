#include "../headers/Scene.hpp"

Scene::Scene()
{
    vec3 defaultPosition = vec3(1.75f, 1.f, 4.5f);
    vec3 defaultTarget = vec3(0.0f, 0.0f, -1.0f);
    vec3 defaultUp = vec3(0.0f, 1.0f, 0.0f);

    defaultCamera.position = vec3(1.75f, 1.f, 4.5f);
    defaultCamera.target = vec3(0.0f, 0.0f, -1.0f);
    defaultCamera.up = vec3(0.0f, 1.0f, 0.0f);
    this->addCamera(defaultCamera);

    // defaultCamera_ptr = make_unique<CameraObject>(defaultTarget, defaultUp);
    // CameraObject *defaultCamera = defaultCamera_ptr.get();
    // this->addCamera2(defaultCamera);
}

Scene::~Scene()
{
    delete this;
}

void Scene::addObject(GameObject *o)
{
    this->objects.push_back(o);
}

void Scene::addCamera(CAMERA cam)
{
    this->cameras.push_back(cam);
}

void Scene::addCamera2(CameraObject *cam)
{
    this->cameras2.push_back(cam);
}

void Scene::addLight(LightObject *light)
{
    this->light = light;
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

    for (PhysicsObject *obj : PhysicsObjectList)
    {
        if (obj->isDynamic())
        {
            // Force de gravité = Poids : W = m*g
            // obj->force += obj->mass * obj->gravity; // apply a force
            obj->force += obj->mass * (obj->gravity + obj->acceleration); // apply a force
            // cout << obj->id << " :  "<< "force = " << obj->force << ", masse= " << obj->mass << ", gravité= " << obj->gravity << endl;

            // cout << "obj->velocity = " << obj->velocity << " + " << (obj->force / obj->mass) * dt << " = " << obj->velocity + (obj->force / obj->mass) * dt << endl;
            obj->velocity += (obj->force / obj->mass) * dt;
            // cout << (obj->force / obj->mass) * dt << endl;
            // cout << obj->id << " :  " << obj->velocity << endl;

            // Clamp max velocity
            if (obj->velocity.x > obj->maxVelocity)
                obj->velocity.x = obj->maxVelocity;
            if (obj->velocity.x < -obj->maxVelocity)
                obj->velocity.x = -obj->maxVelocity;
            if (obj->velocity.y > obj->maxVelocity)
                obj->velocity.y = obj->maxVelocity;
            if (obj->velocity.y < -obj->maxVelocity)
                obj->velocity.y = -obj->maxVelocity;
            if (obj->velocity.z > obj->maxVelocity)
                obj->velocity.z = obj->maxVelocity;
            if (obj->velocity.z < -obj->maxVelocity)
                obj->velocity.z = -obj->maxVelocity;

            obj->position = obj->transform->getLocalTranslation();
            // cout << obj->id << " :  "<< "obj->velocity = " << obj->velocity << endl;
            obj->position += obj->velocity * dt;
            // cout << obj->position << endl;
            obj->transform->setLocalTranslation(obj->position);

            obj->force = vec3(0, 0, 0); // réinitialise la force résultante à la fin
        }
    }
}

void Scene::ResolveCollisions(float dt)
{
    // Generate List of collisions
    std::vector<Collision> collisions;
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
                // cout << "COLLISION ENTRE " << a->id << " ET " << b->id << endl;

                Collision collision;
                collision.ObjA = a;
                collision.ObjB = b;
                collision.Points = points;
                collisions.emplace_back(collision);
            }
            else
            {
                // cout << "PAS DE COLLISION" << endl;
            }
        }
    }

    // Solve Collisions
    for (Solver *solver : SolverList)
    {
        // cout << "test avant " << solver->returnSolverID() << endl;
        solver->Solve(collisions, dt);
        // cout << "test après " << solver->returnSolverID() << endl;
    }
}

void Scene::draw()
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->toDraw)
        {
            // cout << "Draw " << objects[i]->id << endl;
            light->draw();
            objects[i]->draw();
        }
    }
    // light->draw();
}
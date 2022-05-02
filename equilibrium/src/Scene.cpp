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
void Scene::Step(float dt)
{
    for (PhysicsObject *obj : PhysicsObjectList)
    {
        if (obj->Mass > 0)
        {
            // Force de gravité = Poids : W = m*g
            obj->Force += obj->Mass * obj->Gravity; // apply a force

            obj->Vitesse += (obj->Force / obj->Mass) * dt;

            obj->Position = obj->transform->getLocalTranslation();
            obj->Position += obj->Vitesse * dt;
            // cout << obj->Position << endl;
            obj->transform->setLocalTranslation(obj->Position);

            obj->Force = vec3(0, 0, 0); // reset net force at the end}
        }
    }
}

void Scene::draw()
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->toDraw)
        {
            // cout << "Draw " << objects[i]->id << endl;
            objects[i]->draw();
        }
    }
    light->draw();
}
#include "../headers/Scene.hpp"

Scene::Scene()
{
    vec3 defaultPosition = vec3(1.75f, 1.f, 4.5f);
    vec3 defaultTarget = vec3(0.0f, 0.0f, -1.0f);
    vec3 defaultUp = vec3(0.0f, 1.0f, 0.0f);

    defaultCamera.position =vec3(1.75f, 1.f, 4.5f);
    defaultCamera.target = vec3(0.0f, 0.0f, -1.0f);
    defaultCamera.up = vec3(0.0f, 1.0f,  0.0f);
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

void Scene::addCamera2(CameraObject* cam) {
    this->cameras2.push_back(cam);
}

void Scene::addLight(LightObject *light){    
    this->light = light;   
}

void Scene::draw()
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->draw(); 
    }
    light->draw();
}
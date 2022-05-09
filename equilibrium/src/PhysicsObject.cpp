#include "../headers/PhysicsObject.hpp"

PhysicsObject::PhysicsObject(string id, string filename, GLint modelID, GLuint shader, float mass, float gravity, bool is_static)
{
    this->id = id;
    this->transform = new Transform();
    this->mesh = new Mesh(filename, modelID);
    this->shader = shader;
    this->useHeightMap = GL_FALSE;
    this->hasTexture = false;

    this->mass = mass;
    this->is_static = is_static;

    // this->gravity = vec3(0.0f, gravity, 0.0f);
    this->gravity = vec3(0.0f, 0.0f, gravity);
}

PhysicsObject::PhysicsObject(string id, int meshType, float size, GLint modelID, GLuint shader, float mass, float gravity, bool is_static)
{
    this->id = id;
    this->transform = new Transform();
    this->mesh = new Mesh(meshType, size, modelID);
    this->parent = nullptr;
    this->shader = shader;
    this->useHeightMap = GL_FALSE;
    this->hasTexture = false;

    this->mass = mass;
    this->is_static = is_static;
    // this->gravity = vec3(0.0f, gravity, 0.0f);
    this->gravity = vec3(0.0f, 0.0f, gravity);
    if (this->mesh->objectType == SPHERE)
    {
        SphereCollider *sphereCollider = new SphereCollider();
        sphereCollider->Center = this->transform->getWorldTranslation();
        sphereCollider->Radius = this->mesh->size;
        this->collider = sphereCollider;
    }
    if (this->mesh->objectType == PLANE)
    {
        AABBCollider *aabbCollider = new AABBCollider();
        aabbCollider->minValue = vec3(0.0f, 0.0f, 0.0f);
        aabbCollider->maxValue = vec3(this->mesh->size, this->mesh->size, 0.001f);
        this->collider = aabbCollider;
    }
    if (this->mesh->objectType == INFINITE_PLANE)
    {
        PlaneCollider *planeCollider = new PlaneCollider();

        planeCollider->Normal = vec3(0.0f, 0.0f, 1.0f);
        planeCollider->Distance = 0.000001f;
        this->collider = planeCollider;
    }
    if (this->mesh->objectType == CUBE)
    {
        AABBCollider *aabbCollider = new AABBCollider();
        float size_2 = this->mesh->size / 2.0f;
        aabbCollider->minValue = vec3(-size_2, -size_2, -size_2);
        aabbCollider->maxValue = vec3(size_2, size_2, size_2);
        this->collider = aabbCollider;
    }
}
PhysicsObject ::~PhysicsObject() {}

void PhysicsObject ::addVisualSphereRotation()
{
    // cout << "VisualSphereRotation added" << endl;
    if (this->mesh->objectType == SPHERE)
    {
        this->hasRotationObject = true;
        this->rotationObject = new GameObject(this->id + "_rotation", this->mesh->objectType, this->mesh->size, this->mesh->modelID, this->shader);
    }
}

void PhysicsObject::applyMaterial(Material *material)
{
    if (this->hasRotationObject)
    {
        this->rotationObject->PBRMaterials.albedo = loadBMP_custom(material->albedo);
        this->rotationObject->PBRMaterials.normal = loadBMP_custom(material->normal);
        this->rotationObject->PBRMaterials.metallic = loadBMP_custom(material->metallic);
        this->rotationObject->PBRMaterials.roughness = loadBMP_custom(material->roughness);
        this->rotationObject->PBRMaterials.ao = loadBMP_custom(material->ao);
    }
    else
    {
        this->PBRMaterials.albedo = loadBMP_custom(material->albedo);
        this->PBRMaterials.normal = loadBMP_custom(material->normal);
        this->PBRMaterials.metallic = loadBMP_custom(material->metallic);
        this->PBRMaterials.roughness = loadBMP_custom(material->roughness);
        this->PBRMaterials.ao = loadBMP_custom(material->ao);
    }
}

bool PhysicsObject::isPlayer()
{
    return this->is_player;
}

bool PhysicsObject::isStatic()
{
    return this->is_static;
}

bool PhysicsObject ::isDynamic()
{
    return !this->is_static;
}
bool PhysicsObject::isTrigger()
{
    return this->is_trigger;
}

void PhysicsObject::setPlayer(bool b)
{
    this->is_player = b;
}
void PhysicsObject::setTrigger(bool b)
{
    this->is_trigger = b;
}
void PhysicsObject::setTriggerType(int type)
{
    // this->is_trigger = true;
    using std::placeholders::_1;
    if (type == MOVE_TO_LAST_CHECKPOINT)
        this->onCollision = std::bind(&PhysicsObject::moveToLastCheckpoint, this, _1);

    if (type == UPDATE_LAST_CHECKPOINT)
        this->onCollision = std::bind(&PhysicsObject::updateLastCheckpoint, this, _1);
}

void PhysicsObject::moveToLastCheckpoint(PhysicsObject *obj)
{
    if (obj->isPlayer())
    {
        obj->velocity = vec3(0.0f, 0.0f, 0.0f);
        obj->transform->setLocalTranslation(obj->lastCheckpoint);
    }
    else
    {
        // cout << this->id << " is not player" << endl;
    }
}

void PhysicsObject::updateLastCheckpoint(PhysicsObject *obj)
{
    if (obj->isPlayer())
    {
        obj->lastCheckpoint = this->transform->getLocalTranslation() + vec3(0.0f, 0.0f, 0.1f);
        // cout << "update Last Checkpoint to " << this->transform->getLocalTranslation() + vec3(0.0f, 0.0f, 0.1f) << endl;
    }
    else
    {
        // cout << this->id << " is not player" << endl;
    }
}
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include "../headers/SimpleObject.hpp"

SimpleObject::SimpleObject()
{
    transform = new Transform();
}

SimpleObject::SimpleObject(string id)
{
    this->id = id;
    transform = new Transform();
}

SimpleObject::~SimpleObject()
{
    delete this;
}

void SimpleObject::updateSelfAndChild()
{
    if (!this->transform->isDirty())
        return;

    forceUpdateSelfAndChild();
}

void SimpleObject::forceUpdateSelfAndChild()
{
    if (parent)
    {
        this->transform->computeModelMatrix(parent->transform->getModelMatrix());
    }
    else
        this->transform->computeModelMatrix();

    for (auto &&child : children)
    {
        child->forceUpdateSelfAndChild();
    }
}

void SimpleObject::addChild(unique_ptr<SimpleObject> o)
{
    this->children.push_back(move(o));
    children.back()->parent = this;
}

SimpleObject *SimpleObject::getParent()
{
    return this->parent;
}

void SimpleObject::ToDraw(bool toDraw)
{
    this->toDraw = toDraw;
}
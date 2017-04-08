#include "component.h"
#include "sceneobject.h"
#include <iostream>

//TODO: If object is replaced it will still look in the list to remove itself
//TODO: When looping over component and deleting, iterator becomes unsafe
Component::Component(Component::Type type, SceneObject* sceneObject)
    : type(type), sceneObject(sceneObject)
{
    //Remove existing component
    auto it = sceneObject->components.find(type);
    if (it != sceneObject->components.end())
        delete it->second;

    //Add component
    sceneObject->components[type] = this;
}

Component::~Component()
{
    auto it = sceneObject->components.find(type);
    if (it->second == this)
        sceneObject->components.erase(it);
}

void Component::update(float deltaTime)
{
    (void)deltaTime;
    //Default: Nothing to update
}

void Component::draw() const
{
    //Default: Nothing to draw
}

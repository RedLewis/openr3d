#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <list>
#include <map>
#include "aligned.h"
#include "transform.h"
#include "component.h"

class SceneObject : public Aligned<16> {

public:

    Transform transform;
    SceneObject* parent;
    std::list<SceneObject*> children;
    std::map<Component::Type, Component*> components;

    bool enabled = true;

    void update();
    void draw() const;

};

#endif // SCENEOBJECT_H

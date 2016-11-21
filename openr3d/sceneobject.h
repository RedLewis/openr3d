#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <list>
#include <map>
#include <set>
#include "aligned.h"
#include "transform.h"
#include "component.h"
#include "matrix4.h"

class Scene;

class SceneObject : public Aligned<Alignment::SSE>
{

public:

    Scene* const scene;

    Transform transform;

    //TODO: Not use pointers? But reference and actual objects in containers (easy cleanup)
    SceneObject* parent;
    std::list<SceneObject*>::iterator containerIterator;
    std::list<SceneObject*> children;
    std::map<Component::Type, Component*> components;

    SceneObject(Scene* scene, SceneObject* parent = NULL);
    ~SceneObject();

    bool enabled = true;

    void update(float deltaTime);
    void draw(const Matrix4& viewProjectionMatrix) const;

};

#endif // SCENEOBJECT_H

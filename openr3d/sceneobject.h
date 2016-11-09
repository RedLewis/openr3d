#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <list>
#include <map>
#include "aligned.h"
#include "transform.h"
#include "component.h"
#include "matrix4.h"

class Scene;

class SceneObject : public Aligned<16> {

public:

    Scene* scene;
    Transform transform;
    SceneObject* parent;
    std::list<SceneObject*> children;
    std::map<Component::Type, Component*> components;

    SceneObject(Scene* scene, SceneObject* parent = NULL);
    virtual ~SceneObject();

    bool enabled = true;

    void update();
    void draw(const Matrix4& viewProjectionMatrix) const;

};

#endif // SCENEOBJECT_H

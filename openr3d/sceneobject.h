#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <list>
#include <map>
#include <string>
#include "aligned.h"
#include "transform.h"
#include "component.h"
#include "matrix4.h"

class Scene;

// TODO: Review pre and post update (unse lambda functions instead of overloading?)

class SceneObject : public Aligned<Alignment::SSE>
{

public:

    std::string name;

    Scene* const scene;
    SceneObject* const parent = NULL;

    Transform transform;

    std::list<SceneObject*>::iterator containerIterator;
    std::list<SceneObject*> children;
    std::map<Component::Type, Component*> components;

    SceneObject(Scene* scene);
    SceneObject(SceneObject* parent);
    virtual ~SceneObject();

    //TODO: Cleaner way of implementing rigidbody
    //(have the rigidbody be a component separated from collider?)
    bool isRigidbody = false;
    bool enabled = true;

    //Called before drawing
    virtual void preUpdate(float deltaTime);
    void update(float deltaTime);
    virtual void postUpdate(float deltaTime);

    void draw(const Matrix4& viewProjectionMatrix) const;

};

#endif // SCENEOBJECT_H

#ifndef COMPONENT_H
#define COMPONENT_H

#include "aligned.h"

class SceneObject;

class Component : public Aligned<Alignment::SSE>
{

public:

    enum Type {
        LIGHT,
        CAMERA,
        MESHRENDERER
    };

    const Component::Type type;

    //TODO: Not use pointers? But reference
    SceneObject* const sceneObject;

    bool enabled = true;

    virtual void update(float deltaTime);
    virtual void draw() const;

    virtual ~Component();
protected:
    Component(Component::Type type, SceneObject* sceneObject);

};

#endif // COMPONENT_H

#ifndef COMPONENT_H
#define COMPONENT_H

#include "aligned.h"

class SceneObject;

class Component : public Aligned<Alignment::SSE>
{

public:

    SceneObject* sceneObject;

    enum Type {
        LIGHT,
        CAMERA,
        MESHRENDERER
    };

    const Component::Type type;
    bool enabled = true;

    virtual void update();
    virtual void draw() const;

protected:

    Component(Component::Type type);
    virtual ~Component();

};

#endif // COMPONENT_H

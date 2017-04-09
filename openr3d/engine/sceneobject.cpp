#include "sceneobject.h"
#include "opengl.h"
#include "matrix4.h"
#include "shaderprogram.h"
#include "meshrenderer.h"
#include "scene.h"
#include "collider.h"

SceneObject::SceneObject(Scene* scene)
    : scene(scene), parent(NULL), transform(this)
{
    //Add sceneObject to scene
    containerIterator = scene->sceneObjects.insert(scene->sceneObjects.end(), this);
}

SceneObject::SceneObject(SceneObject* parent)
    : scene(parent->scene), parent(parent), transform(this)
{
    //Add sceneObject to parent
    containerIterator = parent->children.insert(parent->children.end(), this);
}

SceneObject::~SceneObject()
{
    for (auto it = components.begin(); it != components.end();)
        delete (it++)->second;
    for (auto it = children.begin(); it != children.end();)
        delete *(it++);

    //Remove sceneObject from scene or from parent
    if (parent == NULL)
        scene->sceneObjects.erase(containerIterator);
    else
        parent->children.erase(containerIterator);
}

void SceneObject::preUpdate(float deltaTime) {
}

void SceneObject::update(float deltaTime)
{
    /*
     * Apply Updates that affect transform
    */

    //Apply Script

    //Apply Animation

    //Apply Physics last because the physics engine need to be notified of changes in transform
    auto it = components.find(Component::COLLIDER);
    if (it != components.end()) {
        static_cast<Collider*>(it->second)->physicsUpdate(deltaTime);
    }

    /*
     * Apply Updates that require transfrom but do not affect it
    */

    //Update Components
    for (auto it = components.begin(); it != components.end();)
        (it++)->second->update(deltaTime);

    transform.clearFlags();

    //Update Children
    for (auto it = children.begin(); it != children.end();)
        (*(it++))->update(deltaTime);
}

void SceneObject::postUpdate(float deltaTime) {
}

//TODO: This parent to children draw call architecture leaves a lot of unsed Matrix4 modelViewProjectionMatrix on the stack as it goes down!
void SceneObject::draw(const Matrix4& viewProjectionMatrix) const
{
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->modelMatrixIndex, 1, GL_FALSE, this->transform.getLocalToWorldMatrix().ptr());
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->normalMatrixIndex, 1, GL_FALSE, this->transform.getLocalToWorldNormalMatrix().ptr());

    Matrix4 modelViewProjectionMatrix = viewProjectionMatrix * this->transform.getLocalToWorldMatrix();
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->modelViewProjectionMatrixIndex, 1, GL_FALSE, modelViewProjectionMatrix.ptr());

    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->draw();
    for (SceneObject* child : children)
        if (child->enabled) child->draw(viewProjectionMatrix);
}

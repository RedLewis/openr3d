#include "sceneobject.h"
#include "opengl.h"
#include "matrix4.h"
#include "shaderprogram.h"

SceneObject::SceneObject(Scene* scene, SceneObject* parent)
    : scene(scene), parent(parent)
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::update()
{
    //Update Transfrom
    Matrix4 rigidTransfromMatrix, scaleMatrix;
    rigidTransfromMatrix.makeRigidTransformation(this->transform.position, this->transform.rotation);
    scaleMatrix.makeScale(this->transform.scale);
    this->transform.localMatrix = scaleMatrix * rigidTransfromMatrix;
    if (this->parent != NULL)
        this->transform.modelMatrix = this->parent->transform.modelMatrix * this->transform.localMatrix;
    else
        this->transform.modelMatrix = this->transform.localMatrix;
    this->transform.normalMatrix = this->transform.modelMatrix.inversed().transposed();

    //Update Children
    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->update();
    for (SceneObject* child : children)
        if (child->enabled) child->update();
}

//TODO: This parent to children draw call architecture leaves a lot of unsed Matrix4 modelViewProjectionMatrix on the stack as it goes down!
void SceneObject::draw(const Matrix4& viewProjectionMatrix) const
{
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->modelMatrixIndex, 1, GL_FALSE, this->transform.modelMatrix.ptr());
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->normalMatrixIndex, 1, GL_FALSE, this->transform.normalMatrix.ptr());

    Matrix4 modelViewProjectionMatrix = viewProjectionMatrix * this->transform.modelMatrix;
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->modelViewProjectionMatrixIndex, 1, GL_FALSE, modelViewProjectionMatrix.ptr());

    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->draw();
    for (SceneObject* child : children)
        if (child->enabled) child->draw(viewProjectionMatrix);
}

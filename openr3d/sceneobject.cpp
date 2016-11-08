#include "sceneobject.h"
#include "opengl.h"
#include "matrix4.h"
#include "shaderprogram.h"

SceneObject::SceneObject(Scene* scene, SceneObject* parent)
    : scene(scene), parent(parent)
{
}

void SceneObject::update()
{
    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->update();
    for (SceneObject* child : children)
        if (child->enabled) child->update();
}

//TODO: Store the model->world matrix in the object's transform and calculate it a each update
//instead of calculating it at every draw
static float rot;
void SceneObject::draw(const Matrix4& mvpMatrix) const
{
    Matrix4 dirtyRot;
    dirtyRot.makeRotateY(rot);

    //Load transform matrix
    //TODO: Move transform matrix and it's calculation to Transform class
    Matrix4 rigidM;
    rigidM.makeRigidTransformation(transform.position, transform.rotation);
    Matrix4 scaleM;
    scaleM.makeScale(transform.scale);
    Matrix4 transM;
    transM = mvpMatrix * scaleM * rigidM * dirtyRot;

    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->mvpMatrixIndex, 1, GL_FALSE, transM.ptr());

    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->draw();
    for (SceneObject* child : children)
        if (child->enabled) child->draw(transM);
}

#include "sceneobject.h"
#include "opengl.h"
#include "matrix4.h"

void SceneObject::update()
{
    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->update();
    for (SceneObject* child : children)
        if (child->enabled) child->update();
}

//TODO: Store the model->world matrix in the object's transform and calculate it a each update
//instead of calculating it at every draw
void SceneObject::draw(const Matrix4& mvpMatrix) const
{

    //Load transform matrix
    Matrix4 matrix;
    matrix.makeRigidTransformation(transform.position, transform.rotation);
    // DEPRECATED
    //gl->glMultMatrixf(matrix.ptr());
    matrix.makeScale(transform.scale);
    //gl->glMultMatrixf(matrix.ptr());


    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->draw();
    for (SceneObject* child : children)
        if (child->enabled) child->draw(mvpMatrix);
}

#include "sceneobject.h"
#include "opengl.h"
#include "matrix4.h"
#include "shaderprogram.h"
#include "meshrenderer.h"
#include "scene.h"

SceneObject::SceneObject(Scene* scene, SceneObject* parent)
    : scene(scene), parent(parent)
{
    //Add sceneObject to scene or to parent
    if (parent == NULL)
        containerIterator = scene->sceneObjects.insert(scene->sceneObjects.end(), this);
    else
        containerIterator= parent->children.insert(scene->sceneObjects.end(), this);
}

SceneObject::~SceneObject()
{
    for (auto it = components.begin(); it != components.end();)
        delete (it++)->second;
    for (auto it = children.begin(); it != children.end();)
        delete *(it++);

    //Remove sceneObject from scene or to parent
    if (parent == NULL)
        scene->sceneObjects.erase(containerIterator);
    else
        parent->children.erase(containerIterator);
}

void SceneObject::update(float deltaTime)
{
    if (this->components.find(Component::CAMERA) == this->components.end() &&
        this->components.find(Component::LIGHT) == this->components.end())
        this->transform.rotation.y += 0.5 * deltaTime;

    /*if (this->components.find(Component::MESHRENDERER) != this->components.end()) {
        MeshRenderer* mr = static_cast<MeshRenderer*>(this->components[Component::MESHRENDERER]);
        for (Color& color : mr->texture->baseData) {
            color.r += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 5);
            color.g += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 5);
            color.b += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 5);
        }
        mr->texture->update();
        for (Mesh::Face& face : mr->mesh->faces) {
            mr->mesh->vertices[face.vertexIndex[0]].x += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[0]].y += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[0]].z += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[1]].x += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[1]].y += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[1]].z += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[2]].x += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[2]].y += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
            mr->mesh->vertices[face.vertexIndex[2]].z += (rand() / (float)RAND_MAX - 0.5f) * (deltaTime * 0.5f);
        }
        mr->mesh->update();
    }*/


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

    //Update Components
    for (auto it = components.begin(); it != components.end();)
        (it++)->second->update(deltaTime);
    //Update Children
    for (auto it = children.begin(); it != children.end();)
        (*(it++))->update(deltaTime);
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

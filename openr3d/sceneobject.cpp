#include "sceneobject.h"
#include "opengl.h"
#include "matrix4.h"
#include "shaderprogram.h"
#include "meshrenderer.h"
#include "scene.h"

SceneObject::SceneObject(Scene* scene)
    : scene(scene), parent(NULL)
{
    //Add sceneObject to scene
    containerIterator = scene->sceneObjects.insert(scene->sceneObjects.end(), this);
}

SceneObject::SceneObject(SceneObject* parent)
    : scene(parent->scene), parent(parent)
{
    //Add sceneObject to parent
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
        this->components.find(Component::LIGHT) == this->components.end()) {
        this->transform.localRotation.x += 0.166 * deltaTime;
        this->transform.localRotation.y += 0.333 * deltaTime;
        this->transform.localRotation.z += 0.500 * deltaTime;
    }

    /*
    ** Update Transfrom
    */
    //Update worldMatrix and worldNormalMatrix using localPosition localRotation and localScale
    Matrix4 scaleMatrix, rigidTransfromMatrix; //rigidTransfromMatrix * scaleMatrix => localMatrix
    scaleMatrix.makeScale(this->transform.localScale);
    rigidTransfromMatrix.makeRigidTransformation(this->transform.localPosition, this->transform.localRotation);
    if (this->parent != NULL)
        this->transform.worldMatrix = this->parent->transform.worldMatrix * rigidTransfromMatrix * scaleMatrix;
    else
        this->transform.worldMatrix = rigidTransfromMatrix * scaleMatrix;
    //TODO: Requires normals to be renormalised after applying this matrix(done in shader). Inneficient!
    //TDOO: The calculation of the matrix inverse is very inneficient since done on a 4x4 and we only need the 3x3 components:
    // - Translation of position does not affect normals.
    // - Rotation is applied to normals just like it is to position.
    // - Uniform scaling of position does not affect the direction of normals.
    // - Non-uniform scaling of position does affect the direction of normals!
    this->transform.worldNormalMatrix = this->transform.worldMatrix.inversed().transposed();
    //Update worldPosition
    /*if (this->parent != NULL)
        this->transform.worldPosition = (this->parent->transform.worldMatrix * this->transform.localPosition.toVector4(1.0f)).toVector3();
    else
        this->transform.worldPosition = this->transform.localPosition;*/
    this->transform.worldPosition = this->transform.worldMatrix.extractTranslation();
    //Extract Scale
    this->transform.worldScale = this->transform.worldMatrix.extractScale();
    //Update worldRotation
    //TODO: The extraction method only works if all rotation were EULER_XYZ! Maybe use Quaternions for the rotation instead of Euler angles
    this->transform.worldRotation = this->transform.worldMatrix.extractEulerAngles();


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
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->modelMatrixIndex, 1, GL_FALSE, this->transform.worldMatrix.ptr());
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->normalMatrixIndex, 1, GL_FALSE, this->transform.worldNormalMatrix.ptr());

    Matrix4 modelViewProjectionMatrix = viewProjectionMatrix * this->transform.worldMatrix;
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->modelViewProjectionMatrixIndex, 1, GL_FALSE, modelViewProjectionMatrix.ptr());

    for (auto& typeComponentPair : components)
        if (typeComponentPair.second->enabled) typeComponentPair.second->draw();
    for (SceneObject* child : children)
        if (child->enabled) child->draw(viewProjectionMatrix);
}

#include "scene.h"
#include "opengl.h"
#include "camera.h"
#include "light.h"
#include "meshrenderer.h"
#include "shaderprogram.h"
#include "boxcollider2d.h"
#include "circlecollider2d.h"
#include "polygoncollider2d.h"
#include "edgecollider2d.h"

Scene::Scene(int width, int height)
    : screen(width, height)
{
    //TODO: Use more than the standard shader
    this->standardShader.load("../openr3d/engine/shaders/standard_vertex_shader.vsh", ShaderProgram::VERTEX);
    this->standardShader.load("../openr3d/engine/shaders/standard_fragment_shader.fsh", ShaderProgram::FRAGMENT);
    this->standardShader.link();

}

Scene::~Scene() {
    for (auto it = sceneObjects.begin(); it != sceneObjects.end();)
        delete *(it++);
    for (Asset* asset : assets)
        delete asset;
}

void Scene::draw() const
{
    standardShader.bind();
    //Configure shader
    gl->glUniform1i(ShaderProgram::activeShaderProgram->useLightIndex, 1);

    if (ShaderProgram::activeShaderProgram == NULL)
        return;

    //Feed light direction and color to shader program
    //TODO Store light direction in light component
    Vector3 lightDirection(0, 0, 1);
    Matrix4 rotation;
    rotation.makeEulerRotation(this->activeLight->transform.getWorldRotation());
    lightDirection = (rotation * lightDirection.toVector4(0.0f)).toVector3().normalize();
    gl->glUniform3fv(ShaderProgram::activeShaderProgram->lightDirectionIndex, 1, lightDirection.ptr());
    gl->glUniform3fv(ShaderProgram::activeShaderProgram->lightColorIndex, 1, static_cast<Light*>(activeLight->components[Component::LIGHT])->color.ptr());

    //Feed camera position to shader program
    gl->glUniform3fv(ShaderProgram::activeShaderProgram->viewPositionIndex, 1, this->activeCamera->transform.getWorldPosition().ptr());

    //Draw the scene from the camera' view
    for (auto it = cameras.begin(); it != cameras.end();) {
        (*(it++))->drawScene();
    }

    ShaderProgram::unbind();
}

//TODO: Fixed Update must be called before physics
//fixed update and physics are called a fixed number of times per second
//Update is called after physics and before every frame
//Multithread?

//TODO: Review pre update / update / post update / fixedUpdate / transform update order
//TODO: Add fixedUpdate
//If I scale a object and recolor during an update, will both effects be visible at the next draw?
//Answers: For now no! The color change will be visible but not the scale since the transform update
//is done before update.
void Scene::update(float deltaTime)
{
    //Pre Update
    for (auto it = sceneObjects.begin(); it != sceneObjects.end();)
        (*(it++))->preUpdate(deltaTime);

    //Physics Update
    physics2d.update(deltaTime);

    //Update
    for (auto it = sceneObjects.begin(); it != sceneObjects.end();)
        (*(it++))->update(deltaTime);

    //Post Update
    for (auto it = sceneObjects.begin(); it != sceneObjects.end();)
        (*(it++))->postUpdate(deltaTime);
}

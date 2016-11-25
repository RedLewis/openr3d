#include "scene.h"
#include "opengl.h"
#include "camera.h"
#include "light.h"
#include "meshrenderer.h"
#include "shaderprogram.h"

Scene::Scene(int width, int height)
    : screen(width, height)
{
    //TODO: Functionalize this process (the creation linking of the sceneObject and the component)

    Texture* texture = new Texture("../assets/texture.ppm");
    Mesh* mesh = new Mesh("../assets/bunny.obj");
    this->assets.push_back(texture);
    this->assets.push_back(mesh);

    //Setup Camera
    SceneObject* cameraObject = new SceneObject(this);
    cameraObject->transform.localPosition = {0, 0, 5.0f};
    cameraObject->transform.localRotation = {0, 0, 0};
    Camera* cameraComponent = new Camera(cameraObject);
    cameraComponent->viewport.set(0.0f, 0.0f, 0.5f, 1.0f);

    //Setup Light
    SceneObject* lightObject = new SceneObject(this);
    lightObject->transform.localRotation = {0, 0, 0};
    Light* lightComponent = new Light(lightObject, Light::Type::DIRECTIONAL);
    lightComponent->color = {1.0, 1.0, 1.0};

    //Setup Mesh
    SceneObject* meshObjectParent = new SceneObject(this);
    MeshRenderer* meshRendererComponent = new MeshRenderer(meshObjectParent);
    meshRendererComponent->mesh = mesh;
    meshRendererComponent->texture = texture;

    cameraObject = new SceneObject(meshObjectParent);
    cameraObject->transform.localPosition.z = 3;
    cameraComponent = new Camera(cameraObject);
    cameraComponent->viewport.set(0.5f, 0.0f, 0.5f, 1.0f);

    //TODO: Clean way of setting up shader
    this->activeCamera = cameraObject;
    this->activeLight = lightObject;
    standardShader.load("../openr3d/shaders/standard_vertex_shader.vsh", ShaderProgram::VERTEX);
    standardShader.load("../openr3d/shaders/standard_fragment_shader.fsh", ShaderProgram::FRAGMENT);
    standardShader.link();
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
    if (ShaderProgram::activeShaderProgram == NULL)
        return;

    //Add Light
    //TODO Store light direction in light component
    Vector3 lightDirection(0, 0, 1);
    Matrix4 rotation;
    rotation.makeEulerRotation(this->activeLight->transform.localRotation);
    lightDirection = (rotation * lightDirection.toVector4(0.0f)).toVector3().normalize();
    gl->glUniform3fv(ShaderProgram::activeShaderProgram->lightDirectionIndex, 1, lightDirection.ptr());
    gl->glUniform4fv(ShaderProgram::activeShaderProgram->lightColorIndex, 1, static_cast<Light*>(activeLight->components[Component::LIGHT])->color.ptr());

    //Draw the scene from the camera' view
    for (Camera* camera : cameras) {
        if (camera->sceneObject->enabled)
            camera->drawScene();
    }

    ShaderProgram::unbind();
}


void Scene::update(float deltaTime)
{
    for (auto it = sceneObjects.begin(); it != sceneObjects.end();)
        (*(it++))->update(deltaTime);
}

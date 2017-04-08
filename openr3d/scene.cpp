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
    //TODO: Functionalize this process (the creation linking of the sceneObject and the component)

    //Setup Camera
    SceneObject* centerofrotation = new SceneObject(this);
    centerofrotation->name = "cameraCenter";
    SceneObject* cameraObject = new SceneObject(centerofrotation);
    cameraObject->name = "camera";
    cameraObject->transform.setLocalPosition({0, 0, 4.0f});
    cameraObject->transform.setLocalRotation({0, 0, 0});
    Camera* cameraComponent = new Camera(cameraObject);
    cameraComponent->setOrthographic(true);
    cameraComponent->setFOV(2);

    //Setup Light
    SceneObject* lightObject = new SceneObject(this);
    lightObject->transform.setLocalRotation({-0.4, -0.4, 0});
    Light* lightComponent = new Light(lightObject, Light::Type::DIRECTIONAL);
    lightComponent->color = {1.0, 1.0, 1.0};

    Mesh* mesh1 = new Mesh("../assets/sphere.obj");
    Mesh* mesh2 = new Mesh("../assets/cube.obj");
    Texture* texture1 = new Texture("../assets/texture.ppm");
    Texture* texture2 = new Texture("../assets/earth.ppm");
    this->assets.push_back(mesh1);
    this->assets.push_back(mesh2);
    this->assets.push_back(texture1);
    this->assets.push_back(texture2);

    {

        SceneObject* parent = new SceneObject(this);
        parent->name = "scale";
        parent->transform.setLocalPosition({0.0f, 0.5f, 0.0f});
        parent->transform.setLocalScale({0.25f, 0.25f, 0.25f});
        parent->transform.setLocalRotation({0.0f, 0.0f, 0.25f});
        MeshRenderer* parentMeshRenderer = new MeshRenderer(parent);
        parentMeshRenderer->mesh = mesh2;
        parentMeshRenderer->texture = texture2;
        new BoxCollider2D(parent, Collider::DYNAMIC);

        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({0.425f, 0.425f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            cube->transform.setLocalRotation({0.0f, 0.0f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);

            /*SceneObject* cubeWorld = new SceneObject(this);
            cubeWorld->transform.setWorldPosition(cube->transform.getWorldPosition());
            cubeWorld->transform.setWorldScale(cube->transform.getWorldScale());
            MeshRenderer* cubeWorldMeshRenderer = new MeshRenderer(cubeWorld);
            cubeWorldMeshRenderer->mesh = mesh2;
            cubeWorldMeshRenderer->texture = texture1;*/
        }
        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({0.400f, -0.400f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);

        }
        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({-0.400f, 0.425f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);
        }
        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({-0.425f, -0.400f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);
        }
    }
    {
        SceneObject* terrain = new SceneObject(this);
        terrain->name = "terrain";
        terrain->transform.setLocalScale({3.0f, 1.0f, 1.0f});
        terrain->transform.setLocalPosition({0.0f, -1.25f, 0.0f});
        MeshRenderer* terrainMeshRenderer = new MeshRenderer(terrain);
        terrainMeshRenderer->mesh = mesh2;
        terrainMeshRenderer->texture = texture1;
        new BoxCollider2D(terrain, Collider::STATIC);
    }
    {
        SceneObject* sphere = new SceneObject(this);
        sphere->name = "sphere";
        sphere->transform.setLocalScale({1.0f, 1.0f, 1.0f});
        sphere->transform.setLocalPosition({1.0f, -0.25f, 0.0f});
        MeshRenderer* sphereMeshRenderer = new MeshRenderer(sphere);
        sphereMeshRenderer->mesh = mesh1;
        sphereMeshRenderer->texture = texture2;
        new CircleCollider2D(sphere, Collider::STATIC);
    }

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
    //Configure shader
    gl->glUniform1i(ShaderProgram::activeShaderProgram->useLightIndex, 1);

    if (ShaderProgram::activeShaderProgram == NULL)
        return;

    //Add Light
    //TODO Store light direction in light component
    Vector3 lightDirection(0, 0, 1);
    Matrix4 rotation;
    rotation.makeEulerRotation(this->activeLight->transform.getLocalRotation());
    lightDirection = (rotation * lightDirection.toVector4(0.0f)).toVector3().normalize();
    gl->glUniform3fv(ShaderProgram::activeShaderProgram->lightDirectionIndex, 1, lightDirection.ptr());
    gl->glUniform3fv(ShaderProgram::activeShaderProgram->lightColorIndex, 1, static_cast<Light*>(activeLight->components[Component::LIGHT])->color.ptr());

    //Draw the scene from the camera' view
    for (auto it = cameras.begin(); it != cameras.end();)
        (*(it++))->drawScene();

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

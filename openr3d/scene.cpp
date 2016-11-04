#include "scene.h"
#include "opengl.h"
#include "camera.h"
#include "light.h"
#include "meshrenderer.h"

Scene::Scene()
{
    //TODO: Functionalize this process (the creation linking of the sceneObject and the component)

    //Setup Camera
    SceneObject* cameraObject = new SceneObject;
    //cameraObject->transform.position = {0, 1, 2};
    //cameraObject->transform.rotation = {-0.5f, 0, 0};
    cameraObject->transform.position = {0, 0, 5.0f};
    cameraObject->transform.rotation = {0, 0, 0};
    Camera* cameraComponent = new Camera;
    cameraComponent->sceneObject = cameraObject;
    cameraObject->components[Component::CAMERA] = cameraComponent;
    this->sceneObjects.push_back(cameraObject);

    //Setup Light
    SceneObject* lightObject = new SceneObject;
    lightObject->transform.rotation = {0, 1.57f/2.0f, 0};
    Light* lightComponent = new Light(Light::Type::DIRECTIONAL);
    lightComponent->sceneObject = lightObject;
    lightObject->components[Component::LIGHT] = lightComponent;
    this->sceneObjects.push_back(lightObject);

    //Setup Mesh
    SceneObject* meshObjectParent = new SceneObject;
    meshObjectParent->transform.position = {0, -1.5f, 0};
    meshObjectParent->transform.rotation = {0, 1.0f, 0};
    meshObjectParent->transform.scale = {1, 1, 1};
    MeshRenderer* meshRendererComponent = new MeshRenderer;
    meshRendererComponent->sceneObject = meshObjectParent;
    meshRendererComponent->mesh = new Mesh("../assets/Tree/Tree.obj");
    meshObjectParent->components[Component::MESHRENDERER] = meshRendererComponent;
    this->sceneObjects.push_back(meshObjectParent);

    SceneObject* meshObjectChild = new SceneObject;
    meshObjectChild->transform.position = {2, 1, 0};
    meshObjectChild->transform.rotation = {0, 0, 0};
    meshObjectChild->transform.scale = {1, 1, 1};
    meshRendererComponent = new MeshRenderer;
    meshRendererComponent->sceneObject = meshObjectChild;
    meshRendererComponent->mesh = new Mesh("../assets/bunny.obj");
    meshObjectChild->components[Component::MESHRENDERER] = meshRendererComponent;
    //this->sceneObjects.push_back(meshObjectChild);
    meshObjectParent->children.push_back(meshObjectChild);

}

void Scene::draw() const
{
    //Switch to ModelView matrix matrix and clear
    glMatrixMode(GL_MODELVIEW);
    //TODO: Maybe load camera matrix here instead of in the update cycle?

    for (SceneObject* sceneObject : sceneObjects)
        if (sceneObject->enabled) {
            glPushMatrix();
            sceneObject->draw();
            glPopMatrix();
        }
}


void Scene::update()
{
    for (SceneObject* sceneObject : sceneObjects)
        if (sceneObject->enabled) sceneObject->update();
}

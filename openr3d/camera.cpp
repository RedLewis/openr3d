#include "camera.h"
#include "opengl.h"
#include "sceneobject.h"
#include "scene.h"

Camera::Camera(SceneObject* sceneObject)
    : Component(Component::Type::CAMERA, sceneObject), viewport(0.0f, 0.0f, 1.0f, 1.0f)
{
    //Add camera to scene
    this->sceneObject->scene->cameras.insert(this);
    orthographic = false;
    fov = 60.0f;
    nearClip = 0.1f;
    farClip = 1000.0f;
}

Camera::~Camera()
{
    auto it = this->sceneObject->scene->cameras.find(this);
    if (it != this->sceneObject->scene->cameras.end())
        this->sceneObject->scene->cameras.erase(it);
}

//TODO: DOES NOT RESPECT TRANSFORM HIEARCHY! (Tranform of father sceneObjects ignored)
void Camera::updateControls()
{
    //Update aspect ratio
    aspectRatio = (viewport.w * this->sceneObject->scene->screen.width) / (viewport.h * this->sceneObject->scene->screen.height);

    // Update Perspective Projection Matrix
    if (orthographic)
        p.makeOrthographicProjection(-aspectRatio * (fov / 2), aspectRatio * (fov / 2), -fov / 2, fov / 2, nearClip, farClip);
    else
        p.makePerspectiveProjection(fov, aspectRatio, nearClip, farClip);

    // Update View Matrix
    c.makeRigidTransformation(sceneObject->transform.worldPosition, sceneObject->transform.worldRotation);
    ci = c.rigidInversed();

    // Update View Projection Matrix
    pci = p * ci;
}

void Camera::drawScene() {
    this->updateControls();
    gl->glViewport(this->viewport.x * this->sceneObject->scene->screen.width,
                   this->viewport.y * this->sceneObject->scene->screen.height,
                   this->viewport.w * this->sceneObject->scene->screen.width,
                   this->viewport.h * this->sceneObject->scene->screen.height);
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->viewMatrixIndex, 1, GL_FALSE, this->ci.ptr());
    for (SceneObject* sceneObject : this->sceneObject->scene->sceneObjects)
        if (sceneObject->enabled) {
            sceneObject->draw(this->pci);
        }
}

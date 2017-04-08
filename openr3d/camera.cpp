#include "camera.h"
#include "opengl.h"
#include "sceneobject.h"
#include "scene.h"

Camera::Camera(SceneObject* sceneObject)
    : Component(Component::Type::CAMERA, sceneObject), viewport(0.0f, 0.0f, 1.0f, 1.0f)
{
    //Add camera to scene
    containerIterator = this->sceneObject->scene->cameras.insert(this->sceneObject->scene->cameras.end(), this);

    orthographic = false;
    fov = 60.0f;
    nearClip = 0.1f;
    farClip = 1000.0f;
}

Camera::~Camera()
{
    //Remove camera from scene
    this->sceneObject->scene->cameras.erase(containerIterator);
}

void Camera::update(float deltaTime)
{
    (void)deltaTime;
}

//TODO: DOES NOT RESPECT TRANSFORM HIEARCHY! (Tranform of father sceneObjects ignored)
void Camera::updateControls()
{
    //Update aspect ratio
    aspectRatio = (viewport.w * this->sceneObject->scene->screen.width) / (viewport.h * this->sceneObject->scene->screen.height);

    // Update Perspective Projection Matrix
    if (orthographic)
        p.makeOrthographicProjection(-aspectRatio * (fov / 2.f), aspectRatio * (fov / 2.f), -fov / 2.f, fov / 2.f, nearClip, farClip);
    else
        p.makePerspectiveProjection(fov, aspectRatio, nearClip, farClip);

    // Update View Matrix
    c.makeRigidTransformation(sceneObject->transform.getWorldPosition(), sceneObject->transform.getWorldRotation());
    ci = c.rigidInversed();

    // Update View Projection Matrix
    pci = p * ci;
}

void Camera::drawScene() {
    updateControls();
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

void Camera::setFOV(float fov) {
    this->fov = fov;
}

float Camera::getFOV() const
{
    return this->fov;
}

void Camera::setOrthographic(bool orthographic) {
    this->orthographic = orthographic;
}

bool Camera::isOrthographic() const {
    return orthographic;
}

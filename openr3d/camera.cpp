#include "camera.h"
#include "opengl.h"
#include "sceneobject.h"

Camera::Camera()
    : Component(Component::Type::CAMERA)
{
    orthographic = false;
    fov = 60.0f;
    aspectRatio = 1.6f;
    nearClip = 0.1f;
    farClip = 1000.0f;
}

//TODO: DOES NOT RESPECT TRANSFORM HIEARCHY! (Tranform of father sceneObjects ignored)
void Camera::update()
{
    /*
    ** Recalculate Matrices
    */
    // Update Perspective Projection Matrix
    if (orthographic)
        p.makeOrthographicProjection(-aspectRatio * (fov / 2), aspectRatio * (fov / 2), -fov / 2, fov / 2, nearClip, farClip);
    else
        p.makePerspectiveProjection(fov, aspectRatio, nearClip, farClip);

    // Update View Matrix
    c.makeRigidTransformation(sceneObject->transform.position, sceneObject->transform.rotation);
    ci = c.rigidInversed();

    // Update View Projection Matrix
    pci = p * ci;

    /*
    ** DEPRECATED: Reload Matrices for next draw calls
    */
    /*
    // Load Perspective Projection Matrix
    gl->glMatrixMode(GL_PROJECTION);
    gl->glLoadMatrixf(p.ptr());
    // Load View Matrix (inverse camera matrix)
    gl->glMatrixMode(GL_MODELVIEW);
    gl->glLoadMatrixf(ci.ptr());
    */
}

void Camera::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
}

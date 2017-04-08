#ifndef CAMERA_H
#define CAMERA_H

#include "component.h"
#include "aligned.h"
#include "matrix4.h"
#include "rectangle.h"
#include <list>

class Camera : public Component
{
private:

    bool orthographic = false;
    // ViewPort controls
    Rectangle viewport;
    // Projection controls
    Matrix4 p;          // Perspective Projection Matrix
    float fov;          // Field of View Angle
    float aspectRatio;	// Projection Plane Aspect Ratio
    float nearClip;     // Near clipping plane distance
    float farClip;      // Far clipping plane distance
    // View controls
    Matrix4 c;          //Camera Matrix
    Matrix4 ci;         //Inverse of Camera Matrix
    // ViewProjection Matrix
    Matrix4 pci;

public:

    std::list<Camera*>::iterator containerIterator;

    Camera(SceneObject* sceneObject);
    ~Camera();

    void update(float deltaTime);

    void updateControls();
    void drawScene();

    //TODO: Set an toBeUpdated variable, otherwise each function calls updateControls
    //which could be called more than once per frame

    void setFOV(float fov);
    float getFOV() const;

    void setOrthographic(bool orthographic);
    bool isOrthographic() const;

};

#endif // CAMERA_H

#ifndef CAMERA_H
#define CAMERA_H

#include "component.h"
#include "aligned.h"
#include "matrix4.h"

class Camera : public Component
{

public:
    bool orthographic = false;
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
    Camera();

    void update();
    void setAspectRatio(float aspectRatio);

};

#endif // CAMERA_H

#ifndef TRANFORM_H
#define TRANFORM_H

#include "aligned.h"
#include "vector3.h"
#include "matrix4.h"

class Transform : public Aligned<Alignment::SSE>
{

public:

    Matrix4 worldMatrix;
    Matrix4 worldNormalMatrix;

    Vector3 localPosition;
    Vector3 localRotation;
    Vector3 localScale;

    Vector3 worldPosition;
    Vector3 worldRotation;
    Vector3 worldScale;

    Transform();
};

#endif // TRANFORM_H

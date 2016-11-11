#ifndef TRANFORM_H
#define TRANFORM_H

#include "aligned.h"
#include "vector3.h"
#include "matrix4.h"

class Transform : public Aligned<Alignment::SSE>
{

public:

    Matrix4 modelMatrix;
    Matrix4 normalMatrix;
    Matrix4 localMatrix;

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Transform();
};

#endif // TRANFORM_H

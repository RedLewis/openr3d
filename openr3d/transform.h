#ifndef TRANFORM_H
#define TRANFORM_H

#include "aligned.h"
#include "vector3.h"

class Transform : public Aligned<16> {

public:

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Transform();
};

#endif // TRANFORM_H

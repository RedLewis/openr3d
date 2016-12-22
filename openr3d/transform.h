#ifndef TRANFORM_H
#define TRANFORM_H

#include "aligned.h"
#include "vector3.h"
#include "matrix4.h"

class SceneObject;

//TODO: Find a way to detect when transform has changed
//This will allow the components like the camera or the collider to update only if position or scale changed.
//Careful, tranform also has changed if the parent was modified
//TODO: Store children and parent in Transform instead of gameObject
//So that when changing the hierachy via setters, the transform updates accordingly
class Transform : public Aligned<Alignment::SSE>
{

private:

    SceneObject* const sceneObject;

    Vector3 localPosition;
    Vector3 localRotation;
    Vector3 localScale;

    Vector3 worldPosition;
    Vector3 worldRotation;
    Vector3 worldScale;

    //Read only
    Matrix4 localToWorldMatrix;
    Matrix4 localToWorldNormalMatrix;
    Matrix4 worldToLocalMatrix;

    bool _changed;
    bool _changedPosition;
    bool _changedRotation;
    bool _changedScale;

    void update();
    //Update controls updates from local vales (changes world and matrices)
    void updateControls();
    //TODO: Need to implement an equivalent update but from world values (changes local and matrices)

    //update child width dynamic collider (or rigidbody) local position to conserve world position (and doesn't affect flags)
    //of corse also does a standard update for other children
    //called after a physical setter
    //Do not need to update rigidbody children matrices, only local position, rotation and scale.
    void physicalUpdate();

public:

    Transform(SceneObject* sceneObject);

    void setLocalPosition(const Vector3& position);
    void setLocalRotation(const Vector3& rotation);
    void setLocalScale(const Vector3& scale);

    void setWorldPosition(const Vector3& position);
    void setWorldRotation(const Vector3& rotation);
    void setWorldScale(const Vector3& scale);

    //setPhysical behaves similarely to setWorld except that it does not change dynamic rigidbody children
    //(bodies that can be modified by the physics engine)
    void setPhysicalPosition(const Vector3& position);
    void setPhysicalRotation(const Vector3& rotation);
    void setPhysicalScale(const Vector3& scale);

    const Vector3& getLocalPosition() const;
    const Vector3& getLocalRotation() const;
    const Vector3& getLocalScale() const;

    const Vector3& getWorldPosition() const;
    const Vector3& getWorldRotation() const;
    const Vector3& getWorldScale() const;

    const Matrix4& getLocalToWorldMatrix() const;
    const Matrix4& getLocalToWorldNormalMatrix() const;
    const Matrix4& getWorldToLocalMatrix() const;

    bool changed() const;
    bool changedPosition() const;
    bool changedRotation() const;
    bool changedScale() const;
    void clearFlags();

};

#endif // TRANFORM_H

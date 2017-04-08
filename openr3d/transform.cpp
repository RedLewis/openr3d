#include "transform.h"
#include "sceneobject.h"
#include <iostream>

Transform::Transform(SceneObject* sceneObject)
    : sceneObject(sceneObject), localPosition(0.0f, 0.0f, 0.0f), localRotation(0.0f, 0.0f, 0.0f), localScale(1.0f, 1.0f, 1.0f)
{
    clearFlags();
    updateControls();
}

void Transform::update()
{
    //Update changes booleans from parent
    if (this->sceneObject->parent != NULL) {
        Transform& parentTransform = this->sceneObject->parent->transform;
        if (parentTransform.changedFlag) {
            setChangedFlag();
            if (parentTransform.changedPositionFlag) {
                setChangedPositionFlag();
            }
            if (parentTransform.changedRotationFlag) {
                setChangedPositionFlag();
                setChangedRotationFlag();
            }
            if (parentTransform.changedScaleFlag) {
                setChangedPositionFlag();
                setChangedRotationFlag(); //TODO: Check that non-uniform scaling can affect rotation
                setChangedScaleFlag();
            }
        }
    }

    this->updateControls();

    //Update components that require to be notified of transform changes

    //Update children's transform
    for (auto it = sceneObject->children.begin(); it != sceneObject->children.end();)
        (*(it++))->transform.update();
}

void Transform::updateControls()
{
    //Update matrices using localPosition localRotation and localScale
    Matrix4 scaleMatrix, rigidTransfromMatrix; //rigidTransfromMatrix * scaleMatrix => localMatrix
    scaleMatrix.makeScale(localScale);
    rigidTransfromMatrix.makeRigidTransformation(localPosition, localRotation);
    if (sceneObject->parent != NULL)
        localToWorldMatrix = sceneObject->parent->transform.localToWorldMatrix * rigidTransfromMatrix * scaleMatrix;
    else
        localToWorldMatrix = rigidTransfromMatrix * scaleMatrix;
    //TODO: Requires normals to be renormalised after applying this matrix(done in shader). Inneficient!
    //TDOO: The calculation of the matrix inverse is very inneficient since done on a 4x4 and we only need the 3x3 components:
    // - Translation of position does not affect normals.
    // - Rotation is applied to normals just like it is to position.
    // - Uniform scaling of position does not affect the direction of normals.
    // - Non-uniform scaling of position does affect the direction of normals!
    worldToLocalMatrix = localToWorldMatrix.inversed();
    localToWorldNormalMatrix = worldToLocalMatrix.transposed();

    //Either extract world position from localToWorldMatrix or use parent worldToLocalMatrix to calculate it from local position
    //Update worldPosition
    worldPosition = localToWorldMatrix.extractTranslation();
    //Extract Scale
    worldScale = localToWorldMatrix.extractScale();
    //Update worldRotation
    //TODO: The extraction method only works if all rotation were EULER_XYZ! Maybe use Quaternions for the rotation instead of Euler angles
    worldRotation = localToWorldMatrix.extractEulerAngles();
}

void Transform::physicalUpdate()
{
    //Update changes booleans from parent
    if (this->sceneObject->parent != NULL) {
        Transform& parentTransform = this->sceneObject->parent->transform;
        if (parentTransform.changedFlag) {
            setChangedFlag();
            if (parentTransform.changedPositionFlag) {
                setChangedPositionFlag();
            }
            if (parentTransform.changedRotationFlag) {
                setChangedPositionFlag();
                setChangedRotationFlag();
            }
            if (parentTransform.changedScaleFlag) {
                setChangedPositionFlag();
                setChangedRotationFlag(); //TODO: Check that non-uniform scaling can affect rotation
                setChangedScaleFlag();
            }
        }
    }

    this->updateControls();

    //Update children's transform BUT cancel transform for dynamic rigidbodies
    for (auto it = sceneObject->children.begin(); it != sceneObject->children.end();)
    {
        SceneObject& childObejct = *(*(it++));
        //apply the tranform cancel to all children both to test (remove second part of if statement when done)
        if (childObejct.isRigidbody) {
            //Cancel transformation: world position are maintained but local position are changed (according to the changes of the parent)
            Transform& childTransform = childObejct.transform;
            //Recalculate local position
            {
                childTransform.localPosition = (this->worldToLocalMatrix * childTransform.worldPosition.toVector4(1.0f)).toVector3();
            }
            //Recalculate local rotation
            {
                Matrix4 parentToChildRotationMatrix;
                {
                    //TODO: parentWorldRotationMatrix is a redundant calculation in a loop!
                    Matrix4 parentWorldRotationMatrix;
                    parentWorldRotationMatrix.makeEulerRotation(this->worldRotation);
                    Matrix4 childWorldRotationMatrix;
                    childWorldRotationMatrix.makeEulerRotation(childTransform.worldRotation);
                    parentToChildRotationMatrix = parentWorldRotationMatrix.transposed() * childWorldRotationMatrix;
                }
                childTransform.localRotation = parentToChildRotationMatrix.extractEulerAngles();
            }
            //Recalculate local scale
            {
                childTransform.localScale = childTransform.worldScale / this->worldScale;
            }
            //TEST THE NEW POSITION BY FORCING MATRICES AND WORLD RECALCULATION (useless in prob)
            childTransform.updateControls();
        }
        else {
            //Update
            childObejct.transform.update();
        }
    }
}

void Transform::setLocalPosition(const Vector3& position)
{
    setChangedFlag();
    setChangedPositionFlag();

    localPosition = position;
    update();
}

void Transform::setLocalRotation(const Vector3& rotation)
{
    setChangedFlag();
    setChangedRotationFlag();

    localRotation = rotation;
    update();
}

void Transform::setLocalScale(const Vector3& scale)
{
    setChangedFlag();
    setChangedScaleFlag();

    localScale = scale;
    update();
}

void Transform::setWorldPosition(const Vector3& position)
{
    setChangedFlag();
    setChangedPositionFlag();

    if (sceneObject->parent != NULL)
        localPosition = (sceneObject->parent->transform.worldToLocalMatrix * position.toVector4(1.0f)).toVector3();
    else
        localPosition = position;
    update();
}

//TODO: Use quaternions
void Transform::setWorldRotation(const Vector3& rotation)
{
    setChangedFlag();
    setChangedRotationFlag();

    //currentToTargetRotationMatrix is a matrix that takes currentWorldRotationMatrix to targetWorldRotationMatrix
    Matrix4 currentToTargetRotationMatrix;
    {
        Matrix4 currentWorldRotationMatrix;
        currentWorldRotationMatrix.makeEulerRotation(worldRotation);
        Matrix4 targetWorldRotationMatrix;
        targetWorldRotationMatrix.makeEulerRotation(rotation);
        currentToTargetRotationMatrix = currentWorldRotationMatrix.transposed() * targetWorldRotationMatrix;
    }
    Matrix4 currentLocalRotationMatrix;
    currentLocalRotationMatrix.makeEulerRotation(localRotation);
    localRotation = (currentLocalRotationMatrix * currentToTargetRotationMatrix).extractEulerAngles();
    update();
}

void Transform::setWorldScale(const Vector3& scale)
{
    setChangedFlag();
    setChangedScaleFlag();

    localScale *= scale / worldScale;
    update();
}


void Transform::setPhysicalPosition(const Vector3& position)
{
    setChangedFlag();
    setChangedPositionFlag();

    if (sceneObject->parent != NULL)
        localPosition = (sceneObject->parent->transform.worldToLocalMatrix * position.toVector4(1.0f)).toVector3();
    else
        localPosition = position;
    physicalUpdate();
}

//TODO: Use quaternions
void Transform::setPhysicalRotation(const Vector3& rotation)
{
    setChangedFlag();
    setChangedRotationFlag();

    //currentToTargetRotationMatrix is a matrix that takes currentWorldRotationMatrix to targetWorldRotationMatrix
    Matrix4 currentToTargetRotationMatrix;
    {
        Matrix4 currentWorldRotationMatrix;
        currentWorldRotationMatrix.makeEulerRotation(worldRotation);
        Matrix4 targetWorldRotationMatrix;
        targetWorldRotationMatrix.makeEulerRotation(rotation);
        currentToTargetRotationMatrix = currentWorldRotationMatrix.transposed() * targetWorldRotationMatrix;
    }
    Matrix4 currentLocalRotationMatrix;
    currentLocalRotationMatrix.makeEulerRotation(localRotation);
    localRotation = (currentLocalRotationMatrix * currentToTargetRotationMatrix).extractEulerAngles();
    physicalUpdate();
}

void Transform::setPhysicalScale(const Vector3& scale)
{
    setChangedFlag();
    setChangedScaleFlag();

    localScale *= scale / worldScale;
    physicalUpdate();
}


const Vector3& Transform::getLocalPosition() const
{
    return localPosition;
}

const Vector3& Transform::getLocalRotation() const
{
    return localRotation;
}

const Vector3& Transform::getLocalScale() const
{
    return localScale;
}

const Vector3& Transform::getWorldPosition() const
{
    return worldPosition;
}

const Vector3& Transform::getWorldRotation() const
{
    return worldRotation;
}

const Vector3& Transform::getWorldScale() const
{
    return worldScale;
}

const Matrix4& Transform::getLocalToWorldMatrix() const
{
    return localToWorldMatrix;
}

const Matrix4& Transform::getLocalToWorldNormalMatrix() const
{
    return localToWorldNormalMatrix;
}

const Matrix4& Transform::getWorldToLocalMatrix() const
{
    return worldToLocalMatrix;
}

bool Transform::getChangedFlag() const
{
    return changedFlag;
}

bool Transform::getChangedPositionFlag() const
{
    return changedPositionFlag;
}

bool Transform::getChangedRotationFlag() const
{
    return changedRotationFlag;
}

bool Transform::getChangedScaleFlag() const
{
    return changedScaleFlag;
}

void Transform::clearFlags()
{
    changedFlag = false;
    changedPositionFlag = false;
    changedRotationFlag = false;
    changedScaleFlag = false;
}

void Transform::setChangedFlag()
{
    changedFlag = true;
}

void Transform::setChangedPositionFlag()
{
    changedPositionFlag = true;
}

void Transform::setChangedRotationFlag()
{
    changedRotationFlag = true;
}

void Transform::setChangedScaleFlag()
{
    changedScaleFlag = true;
}

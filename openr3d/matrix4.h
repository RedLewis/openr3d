#ifndef MATRIX4_H
#define MATRIX4_H

#include "aligned.h"
#include "vector4.h"
#include "vector3.h"

class Matrix4 : Aligned<16>
{

protected:

    //These are in column-major
    //The first index is for the column, and the second is for the row
    //Ex: m[2][1] is the 3rd column (index 2) and second row in that column (index 1)
    //This is the matrix format used by OpenGL internally.
    float m[4][4];

    enum EulerOrder {
        EULER_XYZ,
        EULER_XZY,
        EULER_YXZ,
        EULER_YZX,
        EULER_ZXY,
        EULER_ZYX
    };

public:

    // Constructors
    Matrix4();
    Matrix4(const Matrix4& other);
    Matrix4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

    // Access
    float get(int column, int row) const;
    float* ptr();
    const float* ptr() const;

    // Assignments
    Matrix4& identity();
    Matrix4& set(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    Matrix4& set(int column, int row, float value);
    Matrix4& operator=(const Matrix4& other);

    // Access Operators
    float* operator [] (int i);
    const float* operator [] (int i) const;

    // Arithmetic Operators with Matrix4
    Matrix4 dot(const Matrix4&) const;
    Matrix4 operator * (const Matrix4&) const;
    Matrix4 add(const Matrix4& a) const;
    Matrix4 operator + (const Matrix4&) const;
    Matrix4 sub(const Matrix4& a) const;
    Matrix4 operator - (const Matrix4&) const;

    // Arithmetic Operators with Vector4
    Vector4 dot(const Vector4&) const;
    Vector4 operator * (const Vector4&) const;

    // Arithmetic Operators with Vector3
    Vector3 dot(const Vector3&) const;
    Vector3 operator * (const Vector3&) const;

    // Matrix Operations
    Matrix4& transpose();
    Matrix4 transposed() const;
    Matrix4& inverse();
    Matrix4 inversed() const;
    Matrix4& rigidInverse();
    Matrix4 rigidInversed() const;

    /*
    ** Transformation Matrices
    */

    Matrix4& makeTranslate(float,float,float);
    Matrix4& makeTranslate(const Vector3&);

    Matrix4& makeRotateX(float);
    Matrix4& makeRotateY(float);
    Matrix4& makeRotateZ(float);
    Matrix4& makeArbitraryRotatation(const Vector3&, float);
    Matrix4& makeEulerRotation(float alpha, float beta, float gamma, EulerOrder eulerOrder = EULER_XYZ);
    Matrix4& makeEulerRotation(const Vector3& r, EulerOrder eulerOrder = EULER_XYZ);

    Matrix4& makeScale(float);
    Matrix4& makeScale(float,float,float);
    Matrix4& makeScale(const Vector3& s);

    Matrix4& makeRigidTransformation(float tx, float ty, float tz, float rx, float ry, float rz);
    Matrix4& makeRigidTransformation(const Vector3& translation, const Vector3& rotation);

    Matrix4& makeOrthographicProjection(float right, float left, float top, float bottom, float nearplane, float farplane);
    Matrix4& makePerspectiveProjection(float fov, float aspect, float nearplane, float farplane);

    Matrix4& makeViewport(float xmin, float xmax, float ymin, float ymax);
    Matrix4& makeGBez(const Vector4& p0, const Vector4& p1, const Vector4& p2, const Vector4& p3);
    Matrix4& makeBBez();

    //Other Operations
    void print(const std::string&) const;

};

#endif // MATRIX4_H

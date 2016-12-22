#ifndef VECTOR3_H
#define VECTOR3_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "aligned.h"

//SIMD: Use SSE on x86 and NEON on ARM
//Add modulo operation

class Vector4;

class Vector3 : public Aligned<Alignment::SSE>
{
public:

    union {
        float data[3];
        struct {
            float x;
            float y;
            float z;
        };
    };

    // Constructors
    Vector3();
    Vector3(const Vector3& v);
    Vector3(float a);
    Vector3(float a, float b, float c);

    // Assignments
    void set(float a);
    void set(float a, float b, float c);
    void set(const Vector3& v);
    Vector3& operator=(const Vector3& v);
    Vector3& operator=(float a);

    // Access Operators
    float& operator[](int i);
    const float& operator[](int i) const;

    // Arithmetic Assignment Operator with Vector3
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(const Vector3& v);
    Vector3& operator/=(const Vector3& v);

    // Arithmetic Assignment Operator with float
    Vector3& operator+=(float a);
    Vector3& operator-=(float a);
    Vector3& operator*=(float a);
    Vector3& operator/=(float a);

    // Arithmetic Operator with Vector3
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(const Vector3& v) const;
    Vector3 operator/(const Vector3& v) const;

    // Arithmetic Operator with float
    Vector3 operator+(float a) const;
    Vector3 operator-(float a) const;
    Vector3 operator*(float a) const;
    Vector3 operator/(float a) const;

    // Other Operations
    Vector3 operator-() const;
    Vector3& negate();
    Vector3& zero();

    // Vector Comparaison operators
    bool operator==(const Vector3& v) const;
    bool operator!=(const Vector3& v) const;
    bool operator<=(const Vector3& v) const;
    bool operator>=(const Vector3& v) const;
    bool operator<(const Vector3& v) const;
    bool operator>(const Vector3& v) const;

    // Normalisation
    Vector3& normalize();
    Vector3 normalized() const;
    // Normalizes vector and return its magnitude prior to normalisation
    float unitize();

    // Magnitude
    float magnitude2() const;
    float magnitude() const;
    // Return 1/Magnitude
    float rmagnitude() const;

    // Dot Product
    float dot(const Vector3& v) const;

    // Corss Product
    Vector3 cross(const Vector3& v) const;

    // Rotate Vector arount r axis by theta radians
    Vector3 rotated(float theta, const Vector3& r) const;
    Vector3& rotate(float theta, const Vector3& r);

    // Max or Min values from the two vectors
    Vector3 maxvec(const Vector3& v);
    Vector3 minvec(const Vector3& v);

    // Average value of Vector
    float average();

    // Pointer
    float* ptr();
    const float* ptr() const;

    // To Vector4
    Vector4 toVector4() const;
    Vector4 toVector4(float wVal) const;
    Vector4& asVector4();
    Vector4& asVector4(float wVal);
    const Vector4& asVector4() const;

};

// Float Arithmetic Operators with Vector3
Vector3 operator+(float s, const Vector3& v);
Vector3 operator*(float s, const Vector3& v);

// Dot Product of two Vector3
float dot(const Vector3& a, const Vector3& b);

// Cross Product of two Vector3
Vector3 cross(const Vector3& a, const Vector3& b);

// Vector3 of maxmimum or minimum values from two Vector3
Vector3 maxvec(const Vector3& a, const Vector3& b);
Vector3 minvec(const Vector3& a, const Vector3& b);

// Output Vector3
std::ostream& operator<<(std::ostream& out, const Vector3& v);

// Average value of Vector
float average(const Vector3& v);

#endif // VECTOR3_H

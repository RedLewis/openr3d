#ifndef VECTOR4_H
#define VECTOR4_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "aligned.h"

class Vector3;

class Vector4 : public Aligned<Alignment::SSE>
{
public:

    union {
        __m128 xmm;
        float data[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        };
    };

    // Constructors
    Vector4();
    Vector4(const Vector4& v);
    Vector4(const Vector3& v, float wVal = 0.0f);
    Vector4(const __m128& m);
    Vector4(float a);
    Vector4(float a, float b, float c, float d);

    // Assignments
    void set(float a);
    void set(float a, float b, float c, float d);
    void set(const Vector4& v);
    Vector4& operator=(const Vector4& v);
    Vector4& operator=(float a);

    // Access Operators
    float& operator[](int i);
    const float& operator[](int i) const;

    // Arithmetic Assignment Operator with Vector4
    Vector4& operator+=(const Vector4& v);
    Vector4& operator-=(const Vector4& v);
    Vector4& operator*=(const Vector4& v);
    Vector4& operator/=(const Vector4& v);

    // Arithmetic Assignment Operator with float
    Vector4& operator+=(float a);
    Vector4& operator-=(float a);
    Vector4& operator*=(float a);
    Vector4& operator/=(float a);

    // Arithmetic Operator with Vector4
    Vector4 operator+(const Vector4& v) const;
    Vector4 operator-(const Vector4& v) const;
    Vector4 operator*(const Vector4& v) const;
    Vector4 operator/(const Vector4& v) const;

    // Arithmetic Operator with float
    Vector4 operator+(float a) const;
    Vector4 operator-(float a) const;
    Vector4 operator*(float a) const;
    Vector4 operator/(float a) const;

    // Other Operations
    Vector4 operator-() const;
    Vector4& negate();
    Vector4& zero();

    // Vector Comparaison operators
    bool operator==(const Vector4& v) const;
    bool operator!=(const Vector4& v) const;
    bool operator<=(const Vector4& v) const;
    bool operator>=(const Vector4& v) const;
    bool operator<(const Vector4& v) const;
    bool operator>(const Vector4& v) const;

    // Normalisation
    Vector4& normalize();
    Vector4 normalized() const;
    // Normalizes vector and return its magnitude prior to normalisation
    float unitize();

    // Homogenization
    Vector4& homogenize();
    Vector4 homogenized() const;

    // Magnitude
    float magnitude2() const;
    float magnitude() const;
    // Return 1/Magnitude
    float rmagnitude() const;

    // Dot Product
    float dot(const Vector4& v) const;

    // Max or Min values from the two vectors
    Vector4 maxvec(const Vector4& v);
    Vector4 minvec(const Vector4& v);

    // Average value of Vector
    float average();

    // Pointer
    float* ptr();
    const float* ptr() const;

    // To Vector3
    Vector3 toVector3() const;
    Vector3& asVector3();
    const Vector3& asVector3() const;

};

// Float Arithmetic Operators with Vector4
Vector4 operator+(float s, const Vector4& v);
Vector4 operator*(float s, const Vector4& v);

// Dot Product of two Vector4
float dot(const Vector4& a, const Vector4& b);

// Vector4 of maxmimum or minimum values from two Vector4
Vector4 maxvec(const Vector4& a, const Vector4& b);
Vector4 minvec(const Vector4& a, const Vector4& b);

// Output Vector4
std::ostream& operator<<(std::ostream& out, const Vector4& v);

// Average value of Vector
float average(const Vector4& v);

#endif // VECTOR4_H

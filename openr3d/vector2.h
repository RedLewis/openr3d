#ifndef VECTOR2_H
#define VECTOR2_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

class Vector3;

class Vector2
{
public:

    union {
        float data[2];
        struct {
            float x;
            float y;
        };
    };

    // Constructors
    Vector2();
    Vector2(const Vector2& v);
    Vector2(const Vector3& v);
    Vector2(float a);
    Vector2(float a, float b);

    // Assignments
    void set(float a);
    void set(float a, float b);
    void set(const Vector2& v);
    Vector2& operator=(const Vector2& v);
    Vector2& operator=(float a);

    // Access Operators
    float& operator[](int i);
    const float& operator[](int i) const;

    // Arithmetic Assignment Operator with Vector2
    Vector2& operator+=(const Vector2& v);
    Vector2& operator-=(const Vector2& v);
    Vector2& operator*=(const Vector2& v);
    Vector2& operator/=(const Vector2& v);

    // Arithmetic Assignment Operator with float
    Vector2& operator+=(float a);
    Vector2& operator-=(float a);
    Vector2& operator*=(float a);
    Vector2& operator/=(float a);

    // Arithmetic Operator with Vector2
    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(const Vector2& v) const;
    Vector2 operator/(const Vector2& v) const;

    // Arithmetic Operator with float
    Vector2 operator+(float a) const;
    Vector2 operator-(float a) const;
    Vector2 operator*(float a) const;
    Vector2 operator/(float a) const;

    // Other Operations
    Vector2 operator-() const;
    Vector2& negate();
    Vector2& zero();

    // Vector Comparaison operators
    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;
    bool operator<=(const Vector2& v) const;
    bool operator>=(const Vector2& v) const;
    bool operator<(const Vector2& v) const;
    bool operator>(const Vector2& v) const;

    // Normalisation
    Vector2& normalize();
    Vector2 normalized() const;
    // Normalizes vector and return its magnitude prior to normalisation
    float unitize();

    // Magnitude
    float magnitude2() const;
    float magnitude() const;
    // Return 1/Magnitude
    float rmagnitude() const;

    // Dot Product
    float dot(const Vector2& v) const;

    // Pointer
    float* ptr();
    const float* ptr() const;

    // To Vector3
    Vector3 toVector3() const;
    Vector3 toVector3(float zVal) const;
    Vector3& asVector3();
    Vector3& asVector3(float zVal);
    const Vector3& asVector3() const;

};

// Float Arithmetic Operators with Vector2
Vector2 operator+(float s, const Vector2& v);
Vector2 operator*(float s, const Vector2& v);

// Dot Product of two Vector2
float dot(const Vector2& a, const Vector2& b);

// Vector2 of maxmimum or minimum values from two Vector2
Vector2 maxvec(const Vector2& a, const Vector2& b);
Vector2 minvec(const Vector2& a, const Vector2& b);

// Output Vector2
std::ostream& operator<<(std::ostream& out, const Vector2& v);

// Average value of Vector
float average(const Vector2& v);



/*
** Adapt Vector2 to Boost.Geometry
*/

#include <boost/geometry.hpp>

namespace boost
{
    namespace geometry
    {
        namespace traits
        {
            // Adapt Vector2 to Boost.Geometry

            template<> struct tag<Vector2>
            { typedef point_tag type; };

            template<> struct coordinate_type<Vector2>
            { typedef float type; };

            template<> struct coordinate_system<Vector2>
            { typedef cs::cartesian type; };

            template<> struct dimension<Vector2> : boost::mpl::int_<2> {};

            template<>
            struct access<Vector2, 0>
            {
                static float get(const Vector2& v)
                {
                    return v.x;
                }

                static void set(Vector2& v, float value)
                {
                    v.x = value;
                }
            };

            template<>
            struct access<Vector2, 1>
            {
                static float get(const Vector2& v)
                {
                    return v.y;
                }

                static void set(Vector2& v, float value)
                {
                    v.y = value;
                }
            };
        }
    }
} // namespace boost::geometry::traits

#endif // VECTOR2_H

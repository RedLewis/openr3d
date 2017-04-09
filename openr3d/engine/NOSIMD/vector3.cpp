#include "vector3.h"
#include "vector2.h"
#include "vector4.h"

// Constructors
Vector3::Vector3() : data{0} {}
Vector3::Vector3(const Vector3& v) : data{v.x, v.y, v.z} {}
Vector3::Vector3(const Vector4& v) : data{v.x, v.y, v.z} {}
Vector3::Vector3(const Vector2& v, float zVal) : data{v.x, v.y, zVal} {}
Vector3::Vector3(float a) : data{a, a, a} {}
Vector3::Vector3(float a, float b, float c) : data{a, b, c} {}

// Assignments
void Vector3::set(float a) { x = y = z = a; }
void Vector3::set(float a, float b, float c) { x = a; y = b; z = c; }
void Vector3::set(const Vector3& v) { x = v.x; y = v.y; z = v.z; }
Vector3& Vector3::operator=(const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
Vector3& Vector3::operator=(float a) { x = y = z = a; return *this; }

// Access Operators
float& Vector3::operator[](int i) { return data[i]; }
const float& Vector3::operator[](int i) const { return data[i]; }

// Arithmetic Assignment Operator with Vector3
Vector3& Vector3::operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
Vector3& Vector3::operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
Vector3& Vector3::operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
Vector3& Vector3::operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

// Arithmetic Assignment Operator with float
Vector3& Vector3::operator+=(float a) { x += a; y += a; z += a; return *this; }
Vector3& Vector3::operator-=(float a) { x -= a; y -= a; z -= a; return *this; }
Vector3& Vector3::operator*=(float a) { x *= a; y *= a; z *= a; return *this; }
Vector3& Vector3::operator/=(float a) { x /= a; y /= a; z /= a; return *this; }

// Arithmetic Operator with Vector3
Vector3 Vector3::operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
Vector3 Vector3::operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
Vector3 Vector3::operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
Vector3 Vector3::operator/(const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

// Arithmetic Operator with float
Vector3 Vector3::operator+(float a) const { return Vector3(x + a, y + a, z + a); }
Vector3 Vector3::operator-(float a) const { return Vector3(x - a, y - a, z - a); }
Vector3 Vector3::operator*(float a) const { return Vector3(x * a, y * a, z * a); }
Vector3 Vector3::operator/(float a) const { return Vector3(x / a, y / a, z / a); }

// Other Operations
Vector3 Vector3::operator-() const { return Vector3(x * -1.f, y * -1.f, z * -1.f); }
Vector3& Vector3::negate() { x *= -1.f; y *= -1.f; z *= -1.f; return *this; }
Vector3& Vector3::zero() { x = y = z = 0; return *this; }

// Vector Comparaison operators
bool Vector3::operator==(const Vector3& v) const { return (this->x == v.x && this->y == v.y && this->z == v.z); }
bool Vector3::operator!=(const Vector3& v) const { return (this->x != v.x || this->y != v.y || this->z != v.z); }
bool Vector3::operator<=(const Vector3& v) const { return (this->x <= v.x && this->y <= v.y && this->z <= v.z); }
bool Vector3::operator>=(const Vector3& v) const { return (this->x >= v.x && this->y >= v.y && this->z >= v.z); }
bool Vector3::operator<(const Vector3& v) const  { return (this->x <  v.x && this->y <  v.y && this->z <  v.z); }
bool Vector3::operator>(const Vector3& v) const  { return (this->x >  v.x && this->y >  v.y && this->z >  v.z); }

// Normalisation
Vector3& Vector3::normalize() { return *this /= magnitude(); }
Vector3 Vector3::normalized() const { float m = magnitude(); return Vector3(x / m, y / m, z / m); }
// Normalizes vector and return its magnitude prior to normalisation
float Vector3::unitize() { float m = magnitude(); *this /= m; return m; }

// Magnitude
float Vector3::magnitude2() const { return x * x + y * y + z * z; }
float Vector3::magnitude() const { return sqrtf(x * x + y * y + z * z); }
// Return 1/Magnitude
float Vector3::rmagnitude() const { return 1.f/sqrtf(x * x + y * y + z * z); }

// Dot Product
float Vector3::dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }

// Corss Product
Vector3 Vector3::cross(const Vector3& v) const { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

// Rotate Vector arount r axis by theta radians
Vector3 Vector3::rotated(float theta, const Vector3& r) const {
    Vector3 v0 = r * this->dot(r);
    Vector3 v1 = *this - v0;
    Vector3 v2 = r.cross(v1);
    return v0 +  v1 * cosf(theta) + v2 * sinf(theta);
}
Vector3& Vector3::rotate(float theta, const Vector3& r) { return *this = rotated(theta, r); }

// Max or Min values from the two vectors
Vector3 Vector3::maxvec(const Vector3& v) { return Vector3((x > v.x) ? x : v.x, (y > v.y) ? y : v.y, (z > v.z) ? z : v.z); }
Vector3 Vector3::minvec(const Vector3& v) { return Vector3((x < v.x) ? x : v.x, (y < v.y) ? y : v.y, (z < v.z) ? z : v.z); }

// Average value of Vector
float Vector3::average() { return (x + y + z) / 3.0f; }

// Pointer
float* Vector3::ptr() { return this->data; }
const float* Vector3::ptr() const { return this->data; }

// To Vector4
Vector4 Vector3::toVector4() const { return Vector4(x, y, z, 0); }
Vector4 Vector3::toVector4(float wVal) const { return Vector4(x, y, z, wVal); }



// Float Arithmetic Operators with Vector3
Vector3 operator+(float s, const Vector3& v) { return Vector3(s + v.x, s + v.y, s + v.z); }
Vector3 operator*(float s, const Vector3& v) { return Vector3(s * v.x, s * v.y, s * v.z); }

// Dot Product of two Vector3
float dot(const Vector3& a, const Vector3& b) { return a.dot(b); }

// Cross Product of two Vector3
Vector3 cross(const Vector3& a, const Vector3& b) { return a.cross(b); }

// Vector3 of maxmimum or minimum values from two Vector3
Vector3 maxvec(const Vector3& a, const Vector3& b) { return Vector3((a.x > b.x) ? a.x : b.x, (a.y > b.y) ? a.y : b.y, (a.z > b.z) ? a.z : b.z); }
Vector3 minvec(const Vector3& a, const Vector3& b) { return Vector3((a.x < b.x) ? a.x : b.x, (a.y < b.y) ? a.y : b.y, (a.z < b.z) ? a.z : b.z); }

// Output Vector3
std::ostream& operator<<(std::ostream& out, const Vector3& v) { return out << v.x << " " << v.y << " " << v.z; }

// Average value of Vector
float average(const Vector3& v) { return (v.x + v.y + v.z) / 3.0f; }

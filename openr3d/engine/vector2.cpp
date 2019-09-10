#include "vector2.h"
#include "vector3.h"

// Constructors
Vector2::Vector2() : data{0} {}
Vector2::Vector2(const Vector2& v) : data{v.x, v.y} {}
Vector2::Vector2(const Vector3& v) : data{v.x, v.y} {}
Vector2::Vector2(float a) : data{a, a} {}
Vector2::Vector2(float a, float b) : data{a, b} {}

// Assignments
void Vector2::set(float a) { x = y = a; }
void Vector2::set(float a, float b) { x = a; y = b;; }
void Vector2::set(const Vector2& v) { x = v.x; y = v.y; }
Vector2& Vector2::operator=(const Vector2& v) { x = v.x; y = v.y; return *this; }
Vector2& Vector2::operator=(float a) { x = y = a; return *this; }

// Access Operators
float& Vector2::operator[](int i) { return data[i]; }
const float& Vector2::operator[](int i) const { return data[i]; }

// Arithmetic Assignment Operator with Vector2
Vector2& Vector2::operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
Vector2& Vector2::operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
Vector2& Vector2::operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }
Vector2& Vector2::operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }

// Arithmetic Assignment Operator with float
Vector2& Vector2::operator+=(float a) { x += a; y += a; return *this; }
Vector2& Vector2::operator-=(float a) { x -= a; y -= a; return *this; }
Vector2& Vector2::operator*=(float a) { x *= a; y *= a; return *this; }
Vector2& Vector2::operator/=(float a) { x /= a; y /= a; return *this; }

// Arithmetic Operator with Vector2
Vector2 Vector2::operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
Vector2 Vector2::operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
Vector2 Vector2::operator*(const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
Vector2 Vector2::operator/(const Vector2& v) const { return Vector2(x / v.x, y / v.y); }

// Arithmetic Operator with float
Vector2 Vector2::operator+(float a) const { return Vector2(x + a, y + a); }
Vector2 Vector2::operator-(float a) const { return Vector2(x - a, y - a); }
Vector2 Vector2::operator*(float a) const { return Vector2(x * a, y * a); }
Vector2 Vector2::operator/(float a) const { return Vector2(x / a, y / a); }

// Other Operations
Vector2 Vector2::operator-() const { return Vector2(x * -1.f, y * -1.f); }
Vector2& Vector2::negate() { x *= -1.f; y *= -1.f; return *this; }
Vector2& Vector2::zero() { x = y = 0; return *this; }

// Vector Comparaison operators
bool Vector2::operator==(const Vector2& v) const { return (this->x == v.x && this->y == v.y); }
bool Vector2::operator!=(const Vector2& v) const { return (this->x != v.x || this->y != v.y); }
bool Vector2::operator<=(const Vector2& v) const { return (this->x <= v.x && this->y <= v.y); }
bool Vector2::operator>=(const Vector2& v) const { return (this->x >= v.x && this->y >= v.y); }
bool Vector2::operator<(const Vector2& v) const  { return (this->x <  v.x && this->y <  v.y); }
bool Vector2::operator>(const Vector2& v) const  { return (this->x >  v.x && this->y >  v.y); }

// Normalisation
Vector2& Vector2::normalize() { return *this /= magnitude(); }
Vector2 Vector2::normalized() const { float m = magnitude(); return Vector2(x / m, y / m); }
// Normalizes vector and return its magnitude prior to normalisation
float Vector2::unitize() { float m = magnitude(); *this /= m; return m; }

// Magnitude
float Vector2::magnitude2() const { return x * x + y * y; }
float Vector2::magnitude() const { return sqrtf(x * x + y * y); }
// Return 1/Magnitude
float Vector2::rmagnitude() const { return 1.f/sqrtf(x * x + y * y); }

// Dot Product
float Vector2::dot(const Vector2& v) const { return x * v.x + y * v.y; }

// Pointer
float* Vector2::ptr() { return this->data; }
const float* Vector2::ptr() const { return this->data; }

// To Vector4
Vector3 Vector2::toVector3() const { return Vector3(x, y, 0); }
Vector3 Vector2::toVector3(float zVal) const { return Vector3(x, y, zVal); }



// Float Arithmetic Operators with Vector2
Vector2 operator+(float s, const Vector2& v) { return Vector2(s + v.x, s + v.y); }
Vector2 operator*(float s, const Vector2& v) { return Vector2(s * v.x, s * v.y); }

// Dot Product of two Vector2
float dot(const Vector2& a, const Vector2& b) { return a.dot(b); }

// Vector2 of maxmimum or minimum values from two Vector2
Vector2 maxvec(const Vector2& a, const Vector2& b) { return Vector2((a.x > b.x) ? a.x : b.x, (a.y > b.y) ? a.y : b.y); }
Vector2 minvec(const Vector2& a, const Vector2& b) { return Vector2((a.x < b.x) ? a.x : b.x, (a.y < b.y) ? a.y : b.y); }

// Output Vector2
std::ostream& operator<<(std::ostream& out, const Vector2& v) { return out << "(" << v.x << "," << v.y << ")"; }

// Average value of Vector
float average(const Vector2& v) { return (v.x + v.y) / 2.0f; }

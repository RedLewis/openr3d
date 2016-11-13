#include "vector4.h"
#include "vector3.h"

// Constructors
Vector4::Vector4() : data{0} {}
Vector4::Vector4(const Vector4& v) : data{v.x, v.y, v.z, v.w} {}
Vector4::Vector4(float a) : data{a} {}
Vector4::Vector4(float a, float b, float c, float d) : data{a, b, c, d} {}

// Assignments
void Vector4::set(float a) { x = y = z = w = a; }
void Vector4::set(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
void Vector4::set(const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }
Vector4& Vector4::operator=(const Vector4& v) {  x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
Vector4& Vector4::operator=(float a) { x = y = z = w = a; return *this; }

// Access Operators
float& Vector4::operator[](int i) { return data[i]; }
const float& Vector4::operator[](int i) const { return data[i]; }

// Arithmetic Assignment Operator with Vector4
Vector4& Vector4::operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
Vector4& Vector4::operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
Vector4& Vector4::operator*=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
Vector4& Vector4::operator/=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

// Arithmetic Assignment Operator with float
Vector4& Vector4::operator+=(float a) { x += a; y += a; z += a; w += a; return *this; }
Vector4& Vector4::operator-=(float a) { x -= a; y -= a; z -= a; w -= a; return *this; }
Vector4& Vector4::operator*=(float a) { x *= a; y *= a; z *= a; w *= a; return *this; }
Vector4& Vector4::operator/=(float a) { x /= a; y /= a; z /= a; w /= a; return *this; }

// Arithmetic Operator with Vector4
Vector4 Vector4::operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
Vector4 Vector4::operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
Vector4 Vector4::operator*(const Vector4& v) const { return Vector4(x * v.x, y * v.y, z * v.z, w * v.w); }
Vector4 Vector4::operator/(const Vector4& v) const { return Vector4(x / v.x, y / v.y, z / v.z, w / v.w); }

// Arithmetic Operator with float
Vector4 Vector4::operator+(float a) const { return Vector4(x + a, y + a, z + a, w + a); }
Vector4 Vector4::operator-(float a) const { return Vector4(x - a, y - a, z - a, w - a); }
Vector4 Vector4::operator*(float a) const { return Vector4(x * a, y * a, z * a, w * a); }
Vector4 Vector4::operator/(float a) const { return Vector4(x / a, y / a, z / a, w / a); }

// Other Operations
Vector4 Vector4::operator-() const { return Vector4(x * -1.f, y * -1.f, z * -1.f, w * -1.f); }
Vector4& Vector4::negate() { x *= -1.f; y *= -1.f; z *= -1.f; z *= -1.f; return *this; }
Vector4& Vector4::zero() { x = y = z = w = 0; return *this; }

// Vector Comparaison operators
bool Vector4::operator==(const Vector4& v) const { return (this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w); }
bool Vector4::operator!=(const Vector4& v) const { return (this->x != v.x || this->y != v.y || this->z != v.z || this->w != v.w); }
bool Vector4::operator<=(const Vector4& v) const { return (this->x <= v.x && this->y <= v.y && this->z <= v.z && this->w <= v.w); }
bool Vector4::operator>=(const Vector4& v) const { return (this->x >= v.x && this->y >= v.y && this->z >= v.z && this->w >= v.w); }
bool Vector4::operator<(const Vector4& v) const  { return (this->x <  v.x && this->y <  v.y && this->z <  v.z && this->w <  v.w); }
bool Vector4::operator>(const Vector4& v) const  { return (this->x >  v.x && this->y >  v.y && this->z >  v.z && this->w >  v.w); }

// Normalisation
Vector4& Vector4::normalize() { return *this /= magnitude(); }
Vector4 Vector4::normalized() const { float m = magnitude(); return Vector4(x / m, y / m, z / m, w / m); }
// Normalizes vector and return its magnitude prior to normalisation
float Vector4::unitize() { float m = magnitude(); *this /= m; return m; }

// Homogenization
Vector4& Vector4::homogenize() { x /= w; y /= w; z /= w; w = 1.f; return *this; }
Vector4 Vector4::homogenized() const { return Vector4(x / w, y / w, z / w, 1.f); }

// Magnitude
float Vector4::magnitude2() const { return x * x + y * y + z * z + w * w; }
float Vector4::magnitude() const { return sqrtf(x * x + y * y + z * z + w * w); }
// Return 1/Magnitude
float Vector4::rmagnitude() const { return 1.f/sqrtf(x * x + y * y + z * z + w * w); }

// Dot Product
float Vector4::dot(const Vector4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

// Max or Min values from the two vectors
Vector4 Vector4::maxvec(const Vector4& v) { return Vector4((x > v.x) ? x : v.x, (y > v.y) ? y : v.y, (z > v.z) ? z : v.z, (w > v.w) ? w : v.w); }
Vector4 Vector4::minvec(const Vector4& v) { return Vector4((x < v.x) ? x : v.x, (y < v.y) ? y : v.y, (z < v.z) ? z : v.z, (w < v.w) ? w : v.w); }

// Average value of Vector
float Vector4::average() { return (x + y + z + w) / 4.0f; }

// Pointer
float* Vector4::ptr() { return &(this->x); }
const float* Vector4::ptr() const { return &(this->x); }

// To Vector3
Vector3 Vector4::toVector3() const { return *reinterpret_cast<const Vector3*>(this); }
Vector3& Vector4::asVector3() { return *reinterpret_cast<Vector3*>(this); }
const Vector3& Vector4::asVector3() const { return *reinterpret_cast<const Vector3*>(this); }



// Float Arithmetic Operators with Vector4
Vector4 operator+(float s, const Vector4& v) { return Vector4(s + v.x, s + v.y, s + v.z, s + v.w); }
Vector4 operator*(float s, const Vector4& v) { return Vector4(s * v.x, s * v.y, s * v.z, s * v.w); }

// Dot Product of two Vector4
float dot(const Vector4& a, const Vector4& b) { return a.dot(b); }

// Vector4 of maxmimum or minimum values from two Vector4
Vector4 maxvec(const Vector4& a, const Vector4& b) { return Vector4((a.x > b.x) ? a.x : b.x, (a.y > b.y) ? a.y : b.y, (a.z > b.z) ? a.z : b.z, (a.w > b.w) ? a.w : b.w); }
Vector4 minvec(const Vector4& a, const Vector4& b) { return Vector4((a.x < b.x) ? a.x : b.x, (a.y < b.y) ? a.y : b.y, (a.z < b.z) ? a.z : b.z, (a.w < b.w) ? a.w : b.w); }

// Output Vector4
std::ostream& operator<<(std::ostream& out, const Vector4& v) { return out << v.x << " " << v.y << " " << v.z << " " << v.w; }

// Average value of Vector
float average(const Vector4& v) { return (v.x + v.y + v.z + v.w) / 4.0f; }

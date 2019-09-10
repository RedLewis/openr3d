#include "vector3.h"
#include "vector2.h"
#include "vector4.h"

// Constructors
Vector3::Vector3() : xmm(_mm_setzero_ps()) {}
Vector3::Vector3(const Vector3& v) : xmm(v.xmm) {}
Vector3::Vector3(const Vector4& v) : xmm(v.xmm) {}
Vector3::Vector3(const Vector2& v, float zVal) : xmm(_mm_set_ps(0.f, zVal, v.y, v.x)) {}
Vector3::Vector3(const __m128& m) : xmm(m) {}
Vector3::Vector3(float a) : xmm(_mm_set_ps1(a)) {}
Vector3::Vector3(float a, float b, float c) : xmm(_mm_set_ps(0.f, c, b, a)) {}

// Assignments
void Vector3::set(float a) { xmm = _mm_set_ps1(a); }
void Vector3::set(float a, float b, float c) { xmm = _mm_set_ps(0.f, c, b, a); }
void Vector3::set(const Vector3& v) { xmm = v.xmm; }
Vector3& Vector3::operator=(const Vector3& v) { xmm = v.xmm; return *this; }
Vector3& Vector3::operator=(float a) { xmm = _mm_set_ps1(a); return *this; }

// Access Operators
float& Vector3::operator[](int i) { return data[i]; }
const float& Vector3::operator[](int i) const { return data[i]; }

// Arithmetic Assignment Operator with Vector3
Vector3& Vector3::operator+=(const Vector3& v) { xmm = _mm_add_ps(xmm, v.xmm); return *this; }
Vector3& Vector3::operator-=(const Vector3& v) { xmm = _mm_sub_ps(xmm, v.xmm); return *this; }
Vector3& Vector3::operator*=(const Vector3& v) { xmm = _mm_mul_ps(xmm, v.xmm); return *this; }
Vector3& Vector3::operator/=(const Vector3& v) { xmm = _mm_div_ps(xmm, v.xmm); return *this; }

// Arithmetic Assignment Operator with float
Vector3& Vector3::operator+=(float a) { xmm = _mm_add_ps(xmm, _mm_set_ps1(a)); return *this; }
Vector3& Vector3::operator-=(float a) { xmm = _mm_sub_ps(xmm, _mm_set_ps1(a)); return *this; }
Vector3& Vector3::operator*=(float a) { xmm = _mm_mul_ps(xmm, _mm_set_ps1(a)); return *this; }
Vector3& Vector3::operator/=(float a) { xmm = _mm_div_ps(xmm, _mm_set_ps1(a)); return *this; }

// Arithmetic Operator with Vector3
Vector3 Vector3::operator+(const Vector3& v) const { return _mm_add_ps(xmm, v.xmm); }
Vector3 Vector3::operator-(const Vector3& v) const { return _mm_sub_ps(xmm, v.xmm); }
Vector3 Vector3::operator*(const Vector3& v) const { return _mm_mul_ps(xmm, v.xmm); }
Vector3 Vector3::operator/(const Vector3& v) const { return _mm_div_ps(xmm, v.xmm); }

// Arithmetic Operator with float
Vector3 Vector3::operator+(float a) const { return _mm_add_ps(xmm, _mm_set_ps1(a)); }
Vector3 Vector3::operator-(float a) const { return _mm_sub_ps(xmm, _mm_set_ps1(a)); }
Vector3 Vector3::operator*(float a) const { return _mm_mul_ps(xmm, _mm_set_ps1(a)); }
Vector3 Vector3::operator/(float a) const { return _mm_div_ps(xmm, _mm_set_ps1(a)); }

// Other Operations
Vector3 Vector3::operator-() const { return _mm_sub_ps(_mm_set_ps1(0.f), xmm); }
Vector3& Vector3::negate() { xmm = _mm_sub_ps(_mm_set_ps1(0.f), xmm); return *this; }
Vector3& Vector3::zero() {xmm = _mm_setzero_ps(); return *this; }

// Vector Comparaison operators
bool Vector3::operator==(const Vector3& v) const { return (this->x == v.x && this->y == v.y && this->z == v.z); }
bool Vector3::operator!=(const Vector3& v) const { return (this->x != v.x || this->y != v.y || this->z != v.z); }
bool Vector3::operator<=(const Vector3& v) const { return (this->x <= v.x && this->y <= v.y && this->z <= v.z); }
bool Vector3::operator>=(const Vector3& v) const { return (this->x >= v.x && this->y >= v.y && this->z >= v.z); }
bool Vector3::operator<(const Vector3& v) const  { return (this->x <  v.x && this->y <  v.y && this->z <  v.z); }
bool Vector3::operator>(const Vector3& v) const  { return (this->x >  v.x && this->y >  v.y && this->z >  v.z); }

// Normalisation
Vector3& Vector3::normalize() { xmm = _mm_mul_ps(xmm, _mm_rsqrt_ps(_mm_dp_ps(xmm, xmm, 0b01110111))); return *this; }
Vector3 Vector3::normalized() const { return _mm_mul_ps(xmm, _mm_rsqrt_ps(_mm_dp_ps(xmm, xmm, 0b01110111))); }
// Normalizes vector and return its magnitude prior to normalisation
float Vector3::unitize() { float l = magnitude(); *this /= l; return l; }

// Magnitude
float Vector3::magnitude2() const { return _mm_cvtss_f32(_mm_dp_ps(xmm, xmm, 0b01110001)); }
float Vector3::magnitude() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(xmm, xmm, 0b01110001))); }
// Return 1/Magnitude
float Vector3::rmagnitude() const { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(xmm, xmm, 0b01110001))); }

// Dot Product
float Vector3::dot(const Vector3& v) const { return _mm_cvtss_f32(_mm_dp_ps(xmm, v.xmm, 0b01110001)); }

// Corss Product
Vector3 Vector3::cross(const Vector3& v) const {
    return _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v.xmm, v.xmm, _MM_SHUFFLE(3, 1, 0, 2))),
        _mm_mul_ps(_mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v.xmm, v.xmm, _MM_SHUFFLE(3, 0, 2, 1)))
        );
}

// Rotate Vector arount r axis by theta radians
Vector3 Vector3::rotated(float theta, const Vector3& r) const {
    Vector3 v0 = r * this->dot(r);
    Vector3 v1 = *this - v0;
    Vector3 v2 = r.cross(v1);
    return v0 +  v1 * cosf(theta) + v2 * sinf(theta);
}
Vector3& Vector3::rotate(float theta, const Vector3& r) { return *this = rotated(theta, r); }

// Max or Min values from the two vectors
Vector3 Vector3::maxvec(const Vector3& v) { return _mm_max_ps(xmm, v.xmm); }
Vector3 Vector3::minvec(const Vector3& v) { return _mm_min_ps(xmm, v.xmm); }

// Average value of Vector
float Vector3::average() { return (x + y + z) / 3.0f; }

// Pointer
float* Vector3::ptr() { return &(this->x); }
const float* Vector3::ptr() const { return &(this->x); }

// To Vector4
Vector4 Vector3::toVector4() const { return *reinterpret_cast<const Vector4*>(this); }
Vector4 Vector3::toVector4(float wVal) const { Vector4 v4(this->xmm); v4.w = wVal; return v4; }
Vector4& Vector3::asVector4() { return *reinterpret_cast<Vector4*>(this); }
Vector4& Vector3::asVector4(float wVal) { reinterpret_cast<Vector4*>(this)->w = wVal; return *reinterpret_cast<Vector4*>(this); }
const Vector4& Vector3::asVector4() const { return *reinterpret_cast<const Vector4*>(this); }



// Float Arithmetic Operators with Vector3
Vector3 operator+(float s, const Vector3& v) { return _mm_add_ps(v.xmm, _mm_set_ps1(s)); }
Vector3 operator*(float s, const Vector3& v) { return _mm_mul_ps(v.xmm, _mm_set_ps1(s)); }

// Dot Product of two Vector3
float dot(const Vector3& a, const Vector3& b) { return a.dot(b); }

// Cross Product of two Vector3
Vector3 cross(const Vector3& a, const Vector3& b) { return a.cross(b); }

// Vector3 of maxmimum or minimum values from two Vector3
Vector3 maxvec(const Vector3& a, const Vector3& b) { return _mm_max_ps(a.xmm, b.xmm); }
Vector3 minvec(const Vector3& a, const Vector3& b) { return _mm_min_ps(a.xmm, b.xmm); }

// Output Vector3
std::ostream& operator<<(std::ostream& out, const Vector3& v) { return out << "(" << v.x << "," << v.y << "," << v.z << ")"; }

// Average value of Vector
float average(const Vector3& v) { return (v.x + v.y + v.z) / 3.0f; }

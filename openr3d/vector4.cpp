#include "vector4.h"

// Constructors
Vector4::Vector4() : xmm(_mm_setzero_ps()) {}
Vector4::Vector4(const Vector4& o) : xmm(o.xmm) {}
Vector4::Vector4(const __m128& m) : xmm(m) {}
Vector4::Vector4(float s) : xmm(_mm_set_ps1(s)) {}
Vector4::Vector4(float xVal, float yVal, float zVal, float wVal) : xmm(_mm_set_ps(wVal, zVal, yVal, xVal)) {}

// Assignments
void Vector4::set(float a) { xmm = _mm_set_ps1(a); }
void Vector4::set(float a, float b, float c, float d) { xmm = _mm_set_ps(d, c, b, a); }
void Vector4::set(const Vector4& v) { xmm = v.xmm; }
Vector4& Vector4::operator=(const Vector4& a) { xmm = a.xmm; return *this; }
Vector4& Vector4::operator=(float a) { xmm = _mm_set_ps1(a); return *this; }

// Access Operators
float& Vector4::operator[](int i) { return data[i]; }
const float& Vector4::operator[](int i) const { return data[i]; }

// Arithmetic Assignment Operator with Vector4
Vector4& Vector4::operator+=(const Vector4& v) { xmm = _mm_add_ps(xmm, v.xmm); return *this; }
Vector4& Vector4::operator-=(const Vector4& v) { xmm = _mm_sub_ps(xmm, v.xmm); return *this; }
Vector4& Vector4::operator*=(const Vector4& v) { xmm = _mm_mul_ps(xmm, v.xmm); return *this; }
Vector4& Vector4::operator/=(const Vector4& v) { xmm = _mm_div_ps(xmm, v.xmm); return *this; }

// Arithmetic Assignment Operator with float
Vector4& Vector4::operator+=(float a) { xmm = _mm_add_ps(xmm, _mm_set_ps1(a)); return *this; }
Vector4& Vector4::operator-=(float a) { xmm = _mm_sub_ps(xmm, _mm_set_ps1(a)); return *this; }
Vector4& Vector4::operator*=(float a) { xmm = _mm_mul_ps(xmm, _mm_set_ps1(a)); return *this; }
Vector4& Vector4::operator/=(float a) { xmm = _mm_div_ps(xmm, _mm_set_ps1(a)); return *this; }

// Arithmetic Operator with Vector4
Vector4 Vector4::operator+(const Vector4& v) const { return _mm_add_ps(xmm, v.xmm); }
Vector4 Vector4::operator-(const Vector4& v) const { return _mm_sub_ps(xmm, v.xmm); }
Vector4 Vector4::operator*(const Vector4& v) const { return _mm_mul_ps(xmm, v.xmm); }
Vector4 Vector4::operator/(const Vector4& v) const { return _mm_div_ps(xmm, v.xmm); }

// Arithmetic Operator with float
Vector4 Vector4::operator+(float a) const { return _mm_add_ps(xmm, _mm_set_ps1(a)); }
Vector4 Vector4::operator-(float a) const { return _mm_sub_ps(xmm, _mm_set_ps1(a)); }
Vector4 Vector4::operator*(float a) const { return _mm_mul_ps(xmm, _mm_set_ps1(a)); }
Vector4 Vector4::operator/(float a) const { return _mm_div_ps(xmm, _mm_set_ps1(a)); }

// Other Operations
Vector4 Vector4::operator-() const { return _mm_sub_ps(_mm_set_ps1(0.f), xmm); }
Vector4& Vector4::negate() { xmm = _mm_sub_ps(_mm_set_ps1(0.f), xmm); return *this; }
Vector4& Vector4::zero() {xmm = _mm_setzero_ps(); return *this; }

// Vector Comparaison operators
bool Vector4::operator==(const Vector4& v) const { return (this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w); }
bool Vector4::operator!=(const Vector4& v) const { return (this->x != v.x || this->y != v.y || this->z != v.z || this->w != v.w); }
bool Vector4::operator<=(const Vector4& v) const { return (this->x <= v.x && this->y <= v.y && this->z <= v.z && this->w <= v.w); }
bool Vector4::operator>=(const Vector4& v) const { return (this->x >= v.x && this->y >= v.y && this->z >= v.z && this->w >= v.w); }
bool Vector4::operator<(const Vector4& v) const  { return (this->x <  v.x && this->y <  v.y && this->z <  v.z && this->w <  v.w); }
bool Vector4::operator>(const Vector4& v) const  { return (this->x >  v.x && this->y >  v.y && this->z >  v.z && this->w >  v.w); }

// Normalisation
Vector4& Vector4::normalize() { xmm = _mm_mul_ps(xmm, _mm_rsqrt_ps(_mm_dp_ps(xmm, xmm, 0b11111111))); return *this; }
Vector4 Vector4::normalized() const { return _mm_mul_ps(xmm, _mm_rsqrt_ps(_mm_dp_ps(xmm, xmm, 0b11111111))); }
// Normalizes vector and return its magnitude prior to normalisation
float Vector4::unitize() { float l = magnitude(); *this /= l; return l; }

// Homogenization
Vector4& Vector4::homogenize() { xmm = _mm_div_ps(xmm, _mm_set_ps1(this->w)); return *this; }
Vector4 Vector4::homogenized() const { return _mm_div_ps(xmm, _mm_set_ps1(this->w)); }

// Magnitude
float Vector4::magnitude2() const { return _mm_cvtss_f32(_mm_dp_ps(xmm, xmm, 0b11110001)); }
float Vector4::magnitude() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(xmm, xmm, 0b11110001))); }
// Return 1/Magnitude
float Vector4::rmagnitude() const { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(xmm, xmm, 0b11110001))); }

// Dot Product
float Vector4::dot(const Vector4& v) const { return _mm_cvtss_f32(_mm_dp_ps(xmm, v.xmm, 0b11110001)); }

// Max or Min values from the two vectors
Vector4 Vector4::maxvec(const Vector4& v) { return _mm_max_ps(xmm, v.xmm); }
Vector4 Vector4::minvec(const Vector4& v) { return _mm_min_ps(xmm, v.xmm); }

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
Vector4 operator+(float s, const Vector4& v) { return _mm_add_ps(v.xmm, _mm_set_ps1(s)); }
Vector4 operator*(float s, const Vector4& v) { return _mm_mul_ps(v.xmm, _mm_set_ps1(s)); }

// Dot Product of two Vector4
float dot(const Vector4& a, const Vector4& b) { return a.dot(b); }

// Vector4 of maxmimum or minimum values from two Vector4
Vector4 maxvec(const Vector4& a, const Vector4& b) { return _mm_max_ps(a.xmm, b.xmm); }
Vector4 minvec(const Vector4& a, const Vector4& b) { return _mm_min_ps(a.xmm, b.xmm); }

// Output Vector4
std::ostream& operator<<(std::ostream& out, const Vector4& v) { return out << v.x << " " << v.y << " " << v.z << " " << v.w; }

// Average value of Vector
float average(const Vector4& v) { return (v.x + v.y + v.z + v.w) / 4.0f; }

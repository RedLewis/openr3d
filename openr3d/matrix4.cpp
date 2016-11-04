#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

#include "matrix4.h"

/*
** Constructors
*/

Matrix4::Matrix4()
{
    identity();
}

Matrix4::Matrix4(const Matrix4& other)
{
    std::memcpy(m, other.m, sizeof(m));
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33 )
    : m{m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33}
{
}

/*
** Access
*/

float Matrix4::get(int column, int row) const
{
    return m[column][row];
}

float* Matrix4::ptr()
{
    return &m[0][0];
}

const float* Matrix4::ptr() const
{
    return &m[0][0];
}

/*
** Assignments
*/

Matrix4& Matrix4::identity()
{
    static const float ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    std::memcpy(m, ident, sizeof(m));
    return *this;
}

Matrix4& Matrix4::set(float m00, float m01, float m02, float m03,
                  float m10, float m11, float m12, float m13,
                  float m20, float m21, float m22, float m23,
                  float m30, float m31, float m32, float m33)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
    return *this;
}

Matrix4& Matrix4::set(int column, int row, float value) {
    m[column][row] = value;
    return *this;
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
    std::memcpy(m, other.m, sizeof(m));
    return *this;
}

/*
** Access Operators
*/

float* Matrix4::operator [] (int i)
{
    return m[i];
}

const float* Matrix4::operator [] (int i) const
{
    return m[i];
}

/*
** Arithmetic Operators with Matrix4
*/

Matrix4 Matrix4::dot(const Matrix4& a) const
{
    Matrix4 b;

    //Implement a more performant Matrix * Matrix multiplication

    //The current implementation below is not very efficient:
    //It allocates an additional 8 vectors on the stack and calls their constructors
    //It also calls off to additional functions (which create even more vectors!)
    //Which results in a lot of time being wasted on memory operations
    //This is bad, really bad, ultra bad D:


    /*Vector4 row1(m[0][0], m[1][0], m[2][0], m[3][0]);
    Vector4 row2(m[0][1], m[1][1], m[2][1], m[3][1]);
    Vector4 row3(m[0][2], m[1][2], m[2][2], m[3][2]);
    Vector4 row4(m[0][3], m[1][3], m[2][3], m[3][3]);

    Vector4 col1(a.m[0][0], a.m[0][1], a.m[0][2], a.m[0][3]);
    Vector4 col2(a.m[1][0], a.m[1][1], a.m[1][2], a.m[1][3]);
    Vector4 col3(a.m[2][0], a.m[2][1], a.m[2][2], a.m[2][3]);
    Vector4 col4(a.m[3][0], a.m[3][1], a.m[3][2], a.m[3][3]);

    b.set(row1.dot(col1), row2.dot(col1), row3.dot(col1), row4.dot(col1),
    row1.dot(col2), row2.dot(col2), row3.dot(col2), row4.dot(col2),
    row1.dot(col3), row2.dot(col3), row3.dot(col3), row4.dot(col3),
    row1.dot(col4), row2.dot(col4), row3.dot(col4), row4.dot(col4) );*/



    //Hint: Loops!
    //Hint for the ambitious: SIMD!

    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {
            b[x][y] = m[0][y] * a[x][0] + m[1][y] * a[x][1] + m[2][y] * a[x][2] + m[3][y] * a[x][3];
        }
    }

    return b;
}

Matrix4 Matrix4::operator * (const Matrix4& a) const
{
    return dot(a);
}

Matrix4 Matrix4::add(const Matrix4& a) const
{
    Matrix4 b = {
        m[0][0] + a[0][0], m[0][1] + a[0][1], m[0][2] + a[0][2], m[0][3] + a[0][3],
        m[1][0] + a[1][0], m[1][1] + a[1][1], m[1][2] + a[1][2], m[1][3] + a[1][3],
        m[2][0] + a[2][0], m[2][1] + a[2][1], m[2][2] + a[2][2], m[2][3] + a[2][3],
        m[3][0] + a[3][0], m[3][1] + a[3][1], m[3][2] + a[3][2], m[3][3] + a[3][3]
    };

    return b;
}

Matrix4 Matrix4::operator + (const Matrix4& a) const
{
    return add(a);
}

Matrix4 Matrix4::sub(const Matrix4& a) const
{
    Matrix4 b = {
        m[0][0] - a[0][0], m[0][1] - a[0][1], m[0][2] - a[0][2], m[0][3] - a[0][3],
        m[1][0] - a[1][0], m[1][1] - a[1][1], m[1][2] - a[1][2], m[1][3] - a[1][3],
        m[2][0] - a[2][0], m[2][1] - a[2][1], m[2][2] - a[2][2], m[2][3] - a[2][3],
        m[3][0] - a[3][0], m[3][1] - a[3][1], m[3][2] - a[3][2], m[3][3] - a[3][3]
    };

    return b;
}

Matrix4 Matrix4::operator - (const Matrix4& a) const
{
    return sub(a);
}

/*
** Arithmetic Operators with Vector4
*/

Vector4 Matrix4::dot(const Vector4& a) const
{
    Vector4 b = {
        a[0] * m[0][0] + a[1] * m[1][0] + a[2] * m[2][0] + a[3] * m[3][0],
        a[0] * m[0][1] + a[1] * m[1][1] + a[2] * m[2][1] + a[3] * m[3][1],
        a[0] * m[0][2] + a[1] * m[1][2] + a[2] * m[2][2] + a[3] * m[3][2],
        a[0] * m[0][3] + a[1] * m[1][3] + a[2] * m[2][3] + a[3] * m[3][3]
    };

    return b;
}

Vector4 Matrix4::operator * (const Vector4& a) const
{
    return dot(a);
}

/*
** Arithmetic Operators with Vector3
*/

Vector3 Matrix4::dot(const Vector3& a) const
{
    Vector3 b = {
        a[0] * m[0][0] + a[1] * m[1][0] + a[2] * m[2][0] + a[3] * m[3][0],
        a[0] * m[0][1] + a[1] * m[1][1] + a[2] * m[2][1] + a[3] * m[3][1],
        a[0] * m[0][2] + a[1] * m[1][2] + a[2] * m[2][2] + a[3] * m[3][2]
    };

    //Implement Matrix * Vector3 multiplication
    //Assume the 4th component is 0

    return b;
}

Vector3 Matrix4::operator * (const Vector3& a) const
{
    return dot(a);
}

/*
** Matrix Operations
*/

Matrix4& Matrix4::transpose()
{
    Matrix4 b(*this);
    for(int x = 0; x < 4; ++x)
    {
        for(int y = 0; y < 4; ++y)
        {
            m[y][x] = b.m[x][y];
        }
    }
    return *this;
}

Matrix4 Matrix4::transposed() const
{
    Matrix4 b;
    for(int x = 0; x < 4; ++x)
    {
        for(int y = 0; y < 4; ++y)
        {
            b.m[y][x] = m[x][y];
        }
    }
    return b;
}

Matrix4& Matrix4::inverse()
{
    //Hint: Try basing this on code by cool people on the internet
    //In this class it is okay to use code from the internet
    //So long as you fully understand the code and can clearly explain it if asked to
    //http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform

    /*Matrix4 invOut;
    Matrix4 inv;
    double det;

    inv[0][0] = m[1][1] * m[2][2] * m[3][3] -
        m[1][1] * m[3][2] * m[2][3] -
        m[1][2] * m[2][1] * m[3][3] +
        m[1][2] * m[3][1] * m[2][3] +
        m[1][3] * m[2][1] * m[3][2] -
        m[1][3] * m[3][1] * m[2][2];

    inv[0][1] = -m[0][1] * m[2][2] * m[3][3] +
        m[0][1] * m[3][2] * m[2][3] +
        m[0][2] * m[2][1] * m[3][3] -
        m[0][2] * m[3][1] * m[2][3] -
        m[0][3] * m[2][1] * m[3][2] +
        m[0][3] * m[3][1] * m[2][2];

    inv[0][2] = m[0][1] * m[1][2] * m[3][3] -
        m[0][1] * m[3][2] * m[1][3] -
        m[0][2] * m[1][1] * m[3][3] +
        m[0][2] * m[3][1] * m[1][3] +
        m[0][3] * m[1][1] * m[3][2] -
        m[0][3] * m[3][1] * m[1][2];

    inv[0][3] = -m[0][1] * m[1][2] * m[2][3] +
        m[0][1] * m[2][2] * m[1][3] +
        m[0][2] * m[1][1] * m[2][3] -
        m[0][2] * m[2][1] * m[1][3] -
        m[0][3] * m[1][1] * m[2][2] +
        m[0][3] * m[2][1] * m[1][2];

    inv[1][0] = -m[1][0] * m[2][2] * m[3][3] +
        m[1][0] * m[3][2] * m[2][3] +
        m[1][2] * m[2][0] * m[3][3] -
        m[1][2] * m[3][0] * m[2][3] -
        m[1][3] * m[2][0] * m[3][2] +
        m[1][3] * m[3][0] * m[2][2];

    inv[1][1] = m[0][0] * m[2][2] * m[3][3] -
        m[0][0] * m[3][2] * m[2][3] -
        m[0][2] * m[2][0] * m[3][3] +
        m[0][2] * m[3][0] * m[2][3] +
        m[0][3] * m[2][0] * m[3][2] -
        m[0][3] * m[3][0] * m[2][2];

    inv[1][2] = -m[0][0] * m[1][2] * m[3][3] +
        m[0][0] * m[3][2] * m[1][3] +
        m[0][2] * m[1][0] * m[3][3] -
        m[0][2] * m[3][0] * m[1][3] -
        m[0][3] * m[1][0] * m[3][2] +
        m[0][3] * m[3][0] * m[1][2];

    inv[1][3] = m[0][0] * m[1][2] * m[2][3] -
        m[0][0] * m[2][2] * m[1][3] -
        m[0][2] * m[1][0] * m[2][3] +
        m[0][2] * m[2][0] * m[1][3] +
        m[0][3] * m[1][0] * m[2][2] -
        m[0][3] * m[2][0] * m[1][2];

    inv[2][0] = m[1][0] * m[2][1] * m[3][3] -
        m[1][0] * m[3][1] * m[2][3] -
        m[1][1] * m[2][0] * m[3][3] +
        m[1][1] * m[3][0] * m[2][3] +
        m[1][3] * m[2][0] * m[3][1] -
        m[1][3] * m[3][0] * m[2][1];

    inv[2][1] = -m[0][0] * m[2][1] * m[3][3] +
        m[0][0] * m[3][1] * m[2][3] +
        m[0][1] * m[2][0] * m[3][3] -
        m[0][1] * m[3][0] * m[2][3] -
        m[0][3] * m[2][0] * m[3][1] +
        m[0][3] * m[3][0] * m[2][1];

    inv[2][2] = m[0][0] * m[1][1] * m[3][3] -
        m[0][0] * m[3][1] * m[1][3] -
        m[0][1] * m[1][0] * m[3][3] +
        m[0][1] * m[3][0] * m[1][3] +
        m[0][3] * m[1][0] * m[3][1] -
        m[0][3] * m[3][0] * m[1][1];

    inv[2][3] = -m[0][0] * m[1][1] * m[2][3] +
        m[0][0] * m[2][1] * m[1][3] +
        m[0][1] * m[1][0] * m[2][3] -
        m[0][1] * m[2][0] * m[1][3] -
        m[0][3] * m[1][0] * m[2][1] +
        m[0][3] * m[2][0] * m[1][1];

    inv[3][0] = -m[1][0] * m[2][1] * m[3][2] +
        m[1][0] * m[3][1] * m[2][2] +
        m[1][1] * m[2][0] * m[3][2] -
        m[1][1] * m[3][0] * m[2][2] -
        m[1][2] * m[2][0] * m[3][1] +
        m[1][2] * m[3][0] * m[2][1];

    inv[3][1] = m[0][0] * m[2][1] * m[3][2] -
        m[0][0] * m[3][1] * m[2][2] -
        m[0][1] * m[2][0] * m[3][2] +
        m[0][1] * m[3][0] * m[2][2] +
        m[0][2] * m[2][0] * m[3][1] -
        m[0][2] * m[3][0] * m[2][1];

    inv[3][2] = -m[0][0] * m[1][1] * m[3][2] +
        m[0][0] * m[3][1] * m[1][2] +
        m[0][1] * m[1][0] * m[3][2] -
        m[0][1] * m[3][0] * m[1][2] -
        m[0][2] * m[1][0] * m[3][1] +
        m[0][2] * m[3][0] * m[1][1];

    inv[3][3] = m[0][0] * m[1][1] * m[2][2] -
        m[0][0] * m[2][1] * m[1][2] -
        m[0][1] * m[1][0] * m[2][2] +
        m[0][1] * m[2][0] * m[1][2] +
        m[0][2] * m[1][0] * m[2][1] -
        m[0][2] * m[2][0] * m[1][1];

    det = m[0][0] * inv[0][0] + m[1][0] * inv[0][1] + m[2][0] * inv[0][2] + m[3][0] * inv[0][3];
    det = 1.0 / det;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            invOut[i][j] = inv[i][j] * det;

    return invOut;*/




    //Inversion by Cramer's rule. Code taken from an Intel publication

    float Result[4][4];
    float tmp[12]; /* temp array for pairs */
    float src[16]; /* array of transpose source matrix */
    float det; /* determinant */
               /* transpose matrix */
    for (int i = 0; i < 4; i++) {
        src[i + 0] = (*this).get(i, 0);
        src[i + 4] = (*this).get(i, 1);
        src[i + 8] = (*this).get(i, 2);
        src[i + 12] = (*this).get(i, 3);
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    Result[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];

    //float tempt1 = tmp[0] * src[5];
    //float tempt2 = tmp[1] * src[5];
    //float tempt3 = tmp[3] * src[6];
    //float tempt4 = tmp[2] * src[6];
    //float tempt5 = tmp[4] * src[7];
    //float tempt6 = tmp[5] * src[7];

    //float v1 = tempt1 + tempt3 + tempt5;
    //float v2= tempt2 + tempt4+ tempt6;

    Result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
    Result[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
    Result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
    Result[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    Result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    Result[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    Result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    Result[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
    Result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
    Result[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
    Result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
    Result[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    Result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    Result[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    Result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2] * src[7];
    tmp[1] = src[3] * src[6];
    tmp[2] = src[1] * src[7];
    tmp[3] = src[3] * src[5];
    tmp[4] = src[1] * src[6];
    tmp[5] = src[2] * src[5];
    tmp[6] = src[0] * src[7];
    tmp[7] = src[3] * src[4];
    tmp[8] = src[0] * src[6];
    tmp[9] = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];
    /* calculate second 8 elements (cofactors) */
    Result[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    Result[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    Result[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    Result[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    Result[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    Result[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    Result[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    Result[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    Result[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    Result[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    Result[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    Result[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    Result[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
    Result[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
    Result[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
    Result[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
    /* calculate determinant */
    det = src[0] * Result[0][0] + src[1] * Result[0][1] + src[2] * Result[0][2] + src[3] * Result[0][3];
    /* calculate matrix inverse */
    det = 1.0f / det;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->m[i][j] = Result[i][j] * det;
        }
    }
    return *this;
}

Matrix4 Matrix4::inversed() const
{
    //Hint: Try basing this on code by cool people on the internet
    //In this class it is okay to use code from the internet
    //So long as you fully understand the code and can clearly explain it if asked to
    //http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform

    /*Matrix4 invOut;
    Matrix4 inv;
    double det;

    inv[0][0] = m[1][1] * m[2][2] * m[3][3] -
        m[1][1] * m[3][2] * m[2][3] -
        m[1][2] * m[2][1] * m[3][3] +
        m[1][2] * m[3][1] * m[2][3] +
        m[1][3] * m[2][1] * m[3][2] -
        m[1][3] * m[3][1] * m[2][2];

    inv[0][1] = -m[0][1] * m[2][2] * m[3][3] +
        m[0][1] * m[3][2] * m[2][3] +
        m[0][2] * m[2][1] * m[3][3] -
        m[0][2] * m[3][1] * m[2][3] -
        m[0][3] * m[2][1] * m[3][2] +
        m[0][3] * m[3][1] * m[2][2];

    inv[0][2] = m[0][1] * m[1][2] * m[3][3] -
        m[0][1] * m[3][2] * m[1][3] -
        m[0][2] * m[1][1] * m[3][3] +
        m[0][2] * m[3][1] * m[1][3] +
        m[0][3] * m[1][1] * m[3][2] -
        m[0][3] * m[3][1] * m[1][2];

    inv[0][3] = -m[0][1] * m[1][2] * m[2][3] +
        m[0][1] * m[2][2] * m[1][3] +
        m[0][2] * m[1][1] * m[2][3] -
        m[0][2] * m[2][1] * m[1][3] -
        m[0][3] * m[1][1] * m[2][2] +
        m[0][3] * m[2][1] * m[1][2];

    inv[1][0] = -m[1][0] * m[2][2] * m[3][3] +
        m[1][0] * m[3][2] * m[2][3] +
        m[1][2] * m[2][0] * m[3][3] -
        m[1][2] * m[3][0] * m[2][3] -
        m[1][3] * m[2][0] * m[3][2] +
        m[1][3] * m[3][0] * m[2][2];

    inv[1][1] = m[0][0] * m[2][2] * m[3][3] -
        m[0][0] * m[3][2] * m[2][3] -
        m[0][2] * m[2][0] * m[3][3] +
        m[0][2] * m[3][0] * m[2][3] +
        m[0][3] * m[2][0] * m[3][2] -
        m[0][3] * m[3][0] * m[2][2];

    inv[1][2] = -m[0][0] * m[1][2] * m[3][3] +
        m[0][0] * m[3][2] * m[1][3] +
        m[0][2] * m[1][0] * m[3][3] -
        m[0][2] * m[3][0] * m[1][3] -
        m[0][3] * m[1][0] * m[3][2] +
        m[0][3] * m[3][0] * m[1][2];

    inv[1][3] = m[0][0] * m[1][2] * m[2][3] -
        m[0][0] * m[2][2] * m[1][3] -
        m[0][2] * m[1][0] * m[2][3] +
        m[0][2] * m[2][0] * m[1][3] +
        m[0][3] * m[1][0] * m[2][2] -
        m[0][3] * m[2][0] * m[1][2];

    inv[2][0] = m[1][0] * m[2][1] * m[3][3] -
        m[1][0] * m[3][1] * m[2][3] -
        m[1][1] * m[2][0] * m[3][3] +
        m[1][1] * m[3][0] * m[2][3] +
        m[1][3] * m[2][0] * m[3][1] -
        m[1][3] * m[3][0] * m[2][1];

    inv[2][1] = -m[0][0] * m[2][1] * m[3][3] +
        m[0][0] * m[3][1] * m[2][3] +
        m[0][1] * m[2][0] * m[3][3] -
        m[0][1] * m[3][0] * m[2][3] -
        m[0][3] * m[2][0] * m[3][1] +
        m[0][3] * m[3][0] * m[2][1];

    inv[2][2] = m[0][0] * m[1][1] * m[3][3] -
        m[0][0] * m[3][1] * m[1][3] -
        m[0][1] * m[1][0] * m[3][3] +
        m[0][1] * m[3][0] * m[1][3] +
        m[0][3] * m[1][0] * m[3][1] -
        m[0][3] * m[3][0] * m[1][1];

    inv[2][3] = -m[0][0] * m[1][1] * m[2][3] +
        m[0][0] * m[2][1] * m[1][3] +
        m[0][1] * m[1][0] * m[2][3] -
        m[0][1] * m[2][0] * m[1][3] -
        m[0][3] * m[1][0] * m[2][1] +
        m[0][3] * m[2][0] * m[1][1];

    inv[3][0] = -m[1][0] * m[2][1] * m[3][2] +
        m[1][0] * m[3][1] * m[2][2] +
        m[1][1] * m[2][0] * m[3][2] -
        m[1][1] * m[3][0] * m[2][2] -
        m[1][2] * m[2][0] * m[3][1] +
        m[1][2] * m[3][0] * m[2][1];

    inv[3][1] = m[0][0] * m[2][1] * m[3][2] -
        m[0][0] * m[3][1] * m[2][2] -
        m[0][1] * m[2][0] * m[3][2] +
        m[0][1] * m[3][0] * m[2][2] +
        m[0][2] * m[2][0] * m[3][1] -
        m[0][2] * m[3][0] * m[2][1];

    inv[3][2] = -m[0][0] * m[1][1] * m[3][2] +
        m[0][0] * m[3][1] * m[1][2] +
        m[0][1] * m[1][0] * m[3][2] -
        m[0][1] * m[3][0] * m[1][2] -
        m[0][2] * m[1][0] * m[3][1] +
        m[0][2] * m[3][0] * m[1][1];

    inv[3][3] = m[0][0] * m[1][1] * m[2][2] -
        m[0][0] * m[2][1] * m[1][2] -
        m[0][1] * m[1][0] * m[2][2] +
        m[0][1] * m[2][0] * m[1][2] +
        m[0][2] * m[1][0] * m[2][1] -
        m[0][2] * m[2][0] * m[1][1];

    det = m[0][0] * inv[0][0] + m[1][0] * inv[0][1] + m[2][0] * inv[0][2] + m[3][0] * inv[0][3];
    det = 1.0 / det;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            invOut[i][j] = inv[i][j] * det;

    return invOut;*/




    //Inversion by Cramer's rule. Code taken from an Intel publication

    float Result[4][4];
    float tmp[12]; /* temp array for pairs */
    float src[16]; /* array of transpose source matrix */
    float det; /* determinant */
               /* transpose matrix */
    for (int i = 0; i < 4; i++) {
        src[i + 0] = (*this).get(i, 0);
        src[i + 4] = (*this).get(i, 1);
        src[i + 8] = (*this).get(i, 2);
        src[i + 12] = (*this).get(i, 3);
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    Result[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];

    //float tempt1 = tmp[0] * src[5];
    //float tempt2 = tmp[1] * src[5];
    //float tempt3 = tmp[3] * src[6];
    //float tempt4 = tmp[2] * src[6];
    //float tempt5 = tmp[4] * src[7];
    //float tempt6 = tmp[5] * src[7];

    //float v1 = tempt1 + tempt3 + tempt5;
    //float v2= tempt2 + tempt4+ tempt6;

    Result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
    Result[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
    Result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
    Result[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    Result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    Result[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    Result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    Result[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
    Result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
    Result[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
    Result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
    Result[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    Result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    Result[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    Result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2] * src[7];
    tmp[1] = src[3] * src[6];
    tmp[2] = src[1] * src[7];
    tmp[3] = src[3] * src[5];
    tmp[4] = src[1] * src[6];
    tmp[5] = src[2] * src[5];
    tmp[6] = src[0] * src[7];
    tmp[7] = src[3] * src[4];
    tmp[8] = src[0] * src[6];
    tmp[9] = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];
    /* calculate second 8 elements (cofactors) */
    Result[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    Result[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    Result[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    Result[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    Result[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    Result[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    Result[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    Result[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    Result[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    Result[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    Result[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    Result[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    Result[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
    Result[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
    Result[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
    Result[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
    /* calculate determinant */
    det = src[0] * Result[0][0] + src[1] * Result[0][1] + src[2] * Result[0][2] + src[3] * Result[0][3];
    /* calculate matrix inverse */
    det = 1.0f / det;
    Matrix4 FloatResult;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            FloatResult.set(i, j, float(Result[i][j] * det));
        }
    }
    return FloatResult;
}

Matrix4& Matrix4::rigidInverse()
{
    Matrix4 r = {
        m[0][0], m[1][0], m[2][0], 0,
        m[0][1], m[1][1], m[2][1], 0,
        m[0][2], m[1][2], m[2][2], 0,
        0, 0, 0, 1
    };

    Matrix4 t = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -m[3][0], -m[3][1], -m[3][2], 1
    };

    *this = r*t;
    return *this;
}

Matrix4 Matrix4::rigidInversed() const
{
    //Project 2
    //Calculate the inverse of this matrix with the assumption that it is a rigid transformation
    //This will be useful when implementing cameras!

    Matrix4 r = {
        m[0][0], m[1][0], m[2][0], 0,
        m[0][1], m[1][1], m[2][1], 0,
        m[0][2], m[1][2], m[2][2], 0,
        0, 0, 0, 1
    };

    Matrix4 t = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -m[3][0], -m[3][1], -m[3][2], 1
    };


    return r*t;
}

/*
** Transformation Matrices
*/

Matrix4& Matrix4::makeTranslate(float x, float y, float z)
{
    identity();

    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;

    return *this;
}

Matrix4& Matrix4::makeTranslate(const Vector3& t)
{
    return makeTranslate(t[0], t[1], t[2]);
}

Matrix4& Matrix4::makeRotateX(float angle)
{
    identity();
    m[1][1] = cosf(angle);
    m[2][1] = -sinf(angle);
    m[1][2] = sinf(angle);
    m[2][2] = cosf(angle);

    //Configure this matrix to be a rotation about the X-Axis by 'angle' radians

    return *this;
}

Matrix4& Matrix4::makeRotateY(float angle)
{
    identity();

    m[0][0] = cosf(angle);
    m[2][0] = sinf(angle);
    m[0][2] = -sinf(angle);
    m[2][2] = cosf(angle);

    return *this;
}

Matrix4& Matrix4::makeRotateZ(float angle)
{
    identity();

    m[0][0] = cosf(angle);
    m[1][0] = -sinf(angle);
    m[0][1] = sinf(angle);
    m[1][1] = cosf(angle);

    //Configure this matrix to be a rotation about the Z-Axis by 'angle' radians

    return *this;
}

Matrix4& Matrix4::makeArbitraryRotatation(const Vector3& a, float angle)
{
    Vector3 b = a.normalized();
    float sina = sinf(angle);
    float cosa = cosf(angle);

    identity();

    m[0][0] = cosa + (b[0] * b[0]) * (1 - cosa);
    m[1][0] = b[0] * b[1] * (1 - cosa) - b[2] * sina;
    m[2][0] = b[0] * b[2] * (1 - cosa) + b[1] * sina;

    m[0][1] = b[1] * b[0] * (1 - cosa) + b[2] * sina;
    m[1][1] = cosa + (b[1] * b[1]) * (1 - cosa);
    m[2][1] = b[1] * b[2] * (1 - cosa) - b[0] * sina;

    m[0][2] = b[2] * b[0] * (1 - cosa) - b[1] * sina;
    m[1][2] = b[2] * b[1] * (1 - cosa) + b[0] * sina;
    m[2][2] = cosa + (b[2] * b[2]) * (1 - cosa);

    //Configure this matrix to be a rotation about the 'a' axis by 'angle' radians

    return *this;
}

Matrix4& Matrix4::makeEulerRotation(float alpha, float beta, float gamma, EulerOrder eulerOrder)
{
    float sx=sinf(alpha),cx=cosf(alpha);
    float sy=sinf(beta),cy=cosf(beta);
    float sz=sinf(gamma),cz=cosf(gamma);

    switch (eulerOrder) {
        case EULER_XYZ:
            m[0][0] = cy*cz;    m[1][0] = sx*sy*cz - cx*sz; m[2][0] = cx*sy*cz + sx*sz; m[3][0] = 0;
            m[0][1] = cy*sz;    m[1][1] = sx*sy*sz + cx*cz; m[2][1] = cx*sy*sz - sx*cz; m[3][1] = 0;
            m[0][2] = -sy;      m[1][2] = sx*cy;            m[2][2] = cx*cy;            m[3][2] = 0;
            break;
        default:
            break;
    }

    m[0][3] = 0;    m[1][3] = 0;    m[2][3] = 0;    m[3][3] = 1;
    return *this;
}

Matrix4& Matrix4::makeEulerRotation(const Vector3& r, EulerOrder eulerOrder)
{
    return makeEulerRotation(r[0], r[1], r[2], eulerOrder);
}

Matrix4& Matrix4::makeScale(float s)
{
    return makeScale(s, s, s);
}

Matrix4& Matrix4::makeScale(float sx, float sy, float sz)
{
    identity();

    m[0][0] = sx;
    m[1][1] = sy;
    m[2][2] = sz;

    return *this;
}

Matrix4& Matrix4::makeScale(const Vector3& s)
{
    return makeScale(s[0], s[1], s[2]);
}

Matrix4& Matrix4::makeRigidTransformation(float tx, float ty, float tz, float rx, float ry, float rz)
{
    float sx=sinf(rx),cx=cosf(rx);
    float sy=sinf(ry),cy=cosf(ry);
    float sz=sinf(rz),cz=cosf(rz);

    m[0][0] = cy*cz;    m[1][0] = sx*sy*cz - cx*sz; m[2][0] = cx*sy*cz + sx*sz; m[3][0] = tx;
    m[0][1] = cy*sz;    m[1][1] = sx*sy*sz + cx*cz; m[2][1] = cx*sy*sz - sx*cz; m[3][1] = ty;
    m[0][2] = -sy;      m[1][2] = sx*cy;            m[2][2] = cx*cy;            m[3][2] = tz;
    m[0][3] = 0;        m[1][3] = 0;                m[2][3] = 0;                m[3][3] = 1;

    return *this;
}

Matrix4& Matrix4::makeRigidTransformation(const Vector3& translation, const Vector3& rotation)
{
    return makeRigidTransformation(translation.x, translation.y, translation.z, rotation.x, rotation.y, rotation.z);
}

Matrix4& Matrix4::makeOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
    identity();

    m[0][0] = 2.0f / (right - left);
    m[1][1] = 2.0f / (top - bottom);
    m[2][2] = -2.0f / (far - near);
    m[3][3] = 1.0f;

    m[3][0] = -((right + left)/(right - left));
    m[3][1] = -((top + bottom)/(top - bottom));
    m[3][2] = -((far + near)/(far - near));

    return *this;
}

Matrix4& Matrix4::makePerspectiveProjection(float fov, float aspect, float near, float far)
{
    identity();

    fov = (fov * (float)M_PI) / 180.0f;
    m[0][0] = 1.0f / (aspect * tanf(fov / 2.0f));
    m[1][1] = 1.0f / (tanf(fov / 2.0f));
    m[2][2] = (near + far) / (near - far);
    m[3][3] = 0.0f;
    m[3][2] = (2.0f * near * far) / (near - far);
    m[2][3] = -1.0f;

    return *this;
}

Matrix4& Matrix4::makeViewport(float xmin, float xmax, float ymin, float ymax)
{
    identity();

    m[0][0] = (xmax - xmin) / 2.0f;
    m[1][1] = (ymax - ymin) / 2.0f;
    m[2][2] = 1.0f / 2.0f;
    m[3][3] = 1;
    m[3][0] = (xmin + xmax) / 2.0f;
    m[3][1] = (ymin + ymax) / 2.0f;
    m[3][2] = 1.0f / 2.0f;
    return *this;
}

Matrix4& Matrix4::makeGBez(const Vector4& p0, const Vector4& p1, const Vector4& p2, const Vector4& p3)
{

    identity();

    m[0][0] = p0[0];
    m[0][1] = p0[1];
    m[0][2] = p0[2];
    m[0][3] = p0[3];
    m[1][0] = p1[0];
    m[1][1] = p1[1];
    m[1][2] = p1[2];
    m[1][3] = p1[3];
    m[2][0] = p2[0];
    m[2][1] = p2[1];
    m[2][2] = p2[2];
    m[2][3] = p2[3];
    m[3][0] = p3[0];
    m[3][1] = p3[1];
    m[3][2] = p3[2];
    m[3][3] = p3[3];


    return *this;
}

Matrix4& Matrix4::makeBBez()
{

    identity();

    m[0][0] = -1.0;
    m[0][1] = 3.0;
    m[0][2] = -3.0;
    m[0][3] = 1.0;
    m[1][0] = 3.0;
    m[1][1] = -6.0;
    m[1][2] = 3.0;
    m[1][3] = 0.0;
    m[2][0] = -3.0;
    m[2][1] = 3.0;
    m[2][2] = 0.0;
    m[2][3] = 0.0;
    m[3][0] = 1.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 0.0;

    return *this;
}

/*
** Other Operations
*/

void Matrix4::print(const std::string& comment) const
{
    //Width constants and variables
    static const int pointWidth = 1;
    static const int precisionWidth = 4;
    int integerWidth = 1;

    //Determine the necessary width to the left of the decimal point
    float* elementPtr = (float*)m;
    float maxValue = fabsf(*(elementPtr++));
    while(elementPtr++ < ((float*)m+16)) if(fabsf(*elementPtr) > maxValue) maxValue = fabsf(*elementPtr);
    while(maxValue >= 10.0) { ++integerWidth; maxValue /= 10.0; }

    //Sum up the widths to determine the cell width needed
    int cellWidth = integerWidth + pointWidth + precisionWidth;

    //Set the stream parameters for fixed number of digits after the decimal point
    //and a set number of precision digits
    std::cout << std::fixed;
    std::cout << std::setprecision(precisionWidth);

    //Print the comment
    std::cout << comment << std::endl;

    //Loop through the matrix elements, format each, and print them to screen
    float cellValue;
    for(int element = 0; element < 4; element++)
    {
        std::cout << std::setw(1) << (element == 0 ? "[" : " ");
        for(int vector = 0; vector < 4; vector++)
        {
            cellValue =  m[vector][element];
            std::cout << std::setw(cellWidth + (cellValue >= 0.0 ? 1 : 0)) << cellValue;
            std::cout << std::setw(0) << (vector < 3 ? " " : "");
        }
        std::cout << std::setw(1) << (element == 3 ? "]" : " ") << std::endl;
    }
}

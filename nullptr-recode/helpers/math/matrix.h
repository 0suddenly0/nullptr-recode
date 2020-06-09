#pragma once
#include <limits>
#include <stdint.h>

#include "vector.h"
#include "qangle.h"

struct cplane_t
{
    vec3 normal;
    float dist;
    uint8_t type;   // for fast side tests
    uint8_t signbits;  // signx + (signy<<1) + (signz<<1)
    uint8_t pad[2];

};

class matrix3x4
{
public:
    matrix3x4() {}
    matrix3x4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23)
    {
        m_flMatVal[0][0] = m00; m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
        m_flMatVal[1][0] = m10; m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
        m_flMatVal[2][0] = m20; m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
    }
    //-----------------------------------------------------------------------------
    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    //-----------------------------------------------------------------------------
    void init(const vec3& xAxis, const vec3& yAxis, const vec3& zAxis, const vec3& vecOrigin)
    {
        m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
        m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
        m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
    }

    //-----------------------------------------------------------------------------
    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    //-----------------------------------------------------------------------------
    matrix3x4(const vec3& xAxis, const vec3& yAxis, const vec3& zAxis, const vec3& vecOrigin)
    {
        init(xAxis, yAxis, zAxis, vecOrigin);
    }

    void set_origin(vec3 const& p)
    {
        m_flMatVal[0][3] = p.x;
        m_flMatVal[1][3] = p.y;
        m_flMatVal[2][3] = p.z;
    }

    void invalidate(void)
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                m_flMatVal[i][j] = std::numeric_limits<float>::infinity();;
            }
        }
    }

    vec3 get_x_axis()  const { return at(0); }
    vec3 get_y_axis()  const { return at(1); }
    vec3 get_z_axis()  const { return at(2); }
    vec3 get_origin() const { return at(3); }

    vec3 at(int i) const { return vec3{ m_flMatVal[0][i], m_flMatVal[1][i], m_flMatVal[2][i] }; }

    float* operator[](int i) { return m_flMatVal[i]; }
    const float* operator[](int i) const { return m_flMatVal[i]; }
    float* base() { return &m_flMatVal[0][0]; }
    const float* base() const { return &m_flMatVal[0][0]; }

    float m_flMatVal[3][4];
};
class vmatrix
{
private:
    void vector_multiply_position(const vmatrix& src1, const vec3& src2, vec3& dst) const
    {
        dst.x = src1[0][0] * src2.x + src1[0][1] * src2.y + src1[0][2] * src2.z + src1[0][3];
        dst.y = src1[1][0] * src2.x + src1[1][1] * src2.y + src1[1][2] * src2.z + src1[1][3];
        dst.z = src1[2][0] * src2.x + src1[2][1] * src2.y + src1[2][2] * src2.z + src1[2][3];
    }
public:

    vmatrix() {}
    vmatrix(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    ) {
        init(
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33
        );
    }

    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    vmatrix(const vec3& forward, const vec3& left, const vec3& up) {
        init(
            forward.x, left.x, up.x, 0.0f,
            forward.y, left.y, up.y, 0.0f,
            forward.z, left.z, up.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    // Construct from a 3x4 matrix
    vmatrix(const matrix3x4& matrix3x4)
    {
        init(matrix3x4);
    }

    // Set the values in the matrix.
    void init(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    ) {
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
    }


    // Initialize from a 3x4
    void  init(const matrix3x4& matrix3x4) {
        memcpy(m, matrix3x4.base(), sizeof(matrix3x4));

        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        m[3][3] = 1.0f;
    }

    // array access
    float* operator[](int i)
    {
        return m[i];
    }

    const float* operator[](int i) const
    {
        return m[i];
    }

    // Get a pointer to m[0][0]
    float* base()
    {
        return &m[0][0];
    }

    const float* base() const
    {
        return &m[0][0];
    }

    void  set_left(const vec3& vLeft) {
        m[0][1] = vLeft.x;
        m[1][1] = vLeft.y;
        m[2][1] = vLeft.z;
    }
    void  set_up(const vec3& vUp) {
        m[0][2] = vUp.x;
        m[1][2] = vUp.y;
        m[2][2] = vUp.z;
    }
    void  set_forward(const vec3& vForward) {
        m[0][0] = vForward.x;
        m[1][0] = vForward.y;
        m[2][0] = vForward.z;
    }

    void  get_basis_vectors(vec3& vForward, vec3& vLeft, vec3& vUp) const {
        vForward = vec3(m[0][0], m[1][0], m[2][0]);
        vLeft = vec3(m[0][1], m[1][1], m[2][1]);
        vUp = vec3(m[0][2], m[1][2], m[2][2]);
    }
    void  set_basis_vectors(const vec3& vForward, const vec3& vLeft, const vec3& vUp) {
        set_forward(vForward);
        set_left(vLeft);
        set_up(vUp);
    }

    // Get/Set the translation.
    vec3& get_translation(vec3& vTrans) const {
        vTrans.x = m[0][3];
        vTrans.y = m[1][3];
        vTrans.z = m[2][3];
        return vTrans;
    }
    void  set_translation(const vec3& vTrans) {
        m[0][3] = vTrans.x;
        m[1][3] = vTrans.y;
        m[2][3] = vTrans.z;
    }

    void  pre_translate(const vec3& vTrans) {
        vec3 tmp;
        vector_multiply_position(*this, vTrans, tmp);
        m[0][3] = tmp.x;
        m[1][3] = tmp.y;
        m[2][3] = tmp.z;
    }
    void  post_translate(const vec3& vTrans) {
        m[0][3] += vTrans.x;
        m[1][3] += vTrans.y;
        m[2][3] += vTrans.z;
    }

    matrix3x4& as3x4() {
        return *((matrix3x4*)this);
    }
    const matrix3x4& as3x4() const {
        return *((const matrix3x4*)this);
    }
    void  copy_from3x4(const matrix3x4& m3x4) {
        memcpy(m, m3x4.base(), sizeof(matrix3x4));
        m[3][0] = m[3][1] = m[3][2] = 0;
        m[3][3] = 1;
    }
    void  set3x4(matrix3x4& matrix3x4) const {
        memcpy(matrix3x4.base(), m, sizeof(matrix3x4));
    }

    bool  operator==(const vmatrix& src) const;
    bool  operator!=(const vmatrix& src) const { return !(*this == src); }

    // Access the basis vectors.
    vec3  get_left() const {
        return vec3(m[0][1], m[1][1], m[2][1]);
    }
    vec3  get_up() const {
        return vec3(m[0][2], m[1][2], m[2][2]);
    }
    vec3  get_forward() const {
        return vec3(m[0][0], m[1][0], m[2][0]);
    }
    vec3  get_translation() const {
        return vec3(m[0][3], m[1][3], m[2][3]);
    }


    // Matrix->vector operations.

    // Multiply by a 3D vector (same as operator*).
    void  v3mul(const vec3& vIn, vec3& vOut) const {
        float rw;

        rw = 1.0f / (m[3][0] * vIn.x + m[3][1] * vIn.y + m[3][2] * vIn.z + m[3][3]);
        vOut.x = (m[0][0] * vIn.x + m[0][1] * vIn.y + m[0][2] * vIn.z + m[0][3]) * rw;
        vOut.y = (m[1][0] * vIn.x + m[1][1] * vIn.y + m[1][2] * vIn.z + m[1][3]) * rw;
        vOut.z = (m[2][0] * vIn.x + m[2][1] * vIn.y + m[2][2] * vIn.z + m[2][3]) * rw;
    }

    // Multiply by a 4D vector.
    //void  V4Mul( const Vector4D &vIn, Vector4D &vOut ) const;

    // Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
    vec3  apply_rotation(const vec3& vVec) const {
        return vmul3x3(vVec);
    }

    // Multiply by a vector (divides by w, assumes input w is 1).
    vec3  operator*(const vec3& vVec) const {
        vec3 vRet;
        vRet.x = m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z + m[0][3];
        vRet.y = m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z + m[1][3];
        vRet.z = m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z + m[2][3];

        return vRet;
    }
    
    // Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
    vec3  vmul3x3(const vec3& vVec) const {
        vec3 vResult;
        vector_multiply_position(*this, vVec, vResult);
        return vResult;
    }

    // Apply the inverse (transposed) rotation (only works on pure rotation matrix)
    vec3  vmul3x3_transpose(const vec3& vVec) const {
        vec3 tmp = vVec;
        tmp.x -= m[0][3];
        tmp.y -= m[1][3];
        tmp.z -= m[2][3];

        return vec3(
            m[0][0] * tmp.x + m[1][0] * tmp.y + m[2][0] * tmp.z,
            m[0][1] * tmp.x + m[1][1] * tmp.y + m[2][1] * tmp.z,
            m[0][2] * tmp.x + m[1][2] * tmp.y + m[2][2] * tmp.z
        );
    }

    // Multiply by the upper 3 rows.
    vec3  vmul4x3(const vec3& vVec) const {
        return vec3(
            m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z,
            m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z,
            m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z
        );
    }

    // Apply the inverse (transposed) transformation (only works on pure rotation/translation)
    vec3  vmul4x3_transpose(const vec3& vVec) const {
        return vec3(
            m[0][0] * vVec.x + m[1][0] * vVec.y + m[2][0] * vVec.z,
            m[0][1] * vVec.x + m[1][1] * vVec.y + m[2][1] * vVec.z,
            m[0][2] * vVec.x + m[1][2] * vVec.y + m[2][2] * vVec.z
        );
    }



    // Matrix->plane operations.
    //public:
    // Transform the plane. The matrix can only contain translation and rotation.
    //void  TransformPlane( const v_plane &inPlane, v_plane &outPlane ) const;

    // Just calls TransformPlane and returns the result.
    //v_plane  operator*(const v_plane &thePlane) const;

    // Matrix->matrix operations.
public:

    vmatrix& operator=(const vmatrix& mOther);

    // Add two matrices.
    const vmatrix& operator+=(const vmatrix& other) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] += other.m[i][j];
            }
        }

        return *this;
    }

    // Add/Subtract two matrices.
    vmatrix  operator+(const vmatrix& other) const {
        vmatrix ret;
        for (int i = 0; i < 16; i++) {
            ((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
        }
        return ret;
    }
    vmatrix  operator-(const vmatrix& other) const {
        vmatrix ret;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                ret.m[i][j] = m[i][j] - other.m[i][j];
            }
        }

        return ret;
    }

    // Negation.
    vmatrix  operator-() const {
        vmatrix ret;
        for (int i = 0; i < 16; i++) {
            ((float*)ret.m)[i] = -((float*)m)[i];
        }
        return ret;
    }

    // Return inverse matrix. Be careful because the results are undefined 
    // if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
    vmatrix  operator~() const;

    // Matrix operations.
public:
    // Set to identity.
    void  identity();
    bool  is_identity() const;
public:
    // The matrix.
    float  m[4][4];
};

void matrix_get_column(const matrix3x4& src, int nCol, vec3& pColumn)
{
    pColumn.x = src[0][nCol];
    pColumn.y = src[1][nCol];
    pColumn.z = src[2][nCol];
}

void matrix_position(const matrix3x4& matrix, vec3& position)
{
    matrix_get_column(matrix, 3, position);
}

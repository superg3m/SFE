#include "matrix.hpp"
#include "../Common/common.hpp"
#include "quaternion.hpp"

namespace Math {
    Matrix4::Matrix4() {
        v[0] = Vector4(0, 0, 0, 0);
        v[1] = Vector4(0, 0, 0, 0);
        v[2] = Vector4(0, 0, 0, 0);
        v[3] = Vector4(0, 0, 0, 0);
    }

    Matrix4::Matrix4(Vector4 r0, Vector4 r1, Vector4 r2, Vector4 r3) {
        this->v = {r0, r1, r2, r3};
    }

    Matrix4::Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) {
        v[0] = Vector4(m00, m01, m02, m03);
        v[1] = Vector4(m10, m11, m12, m13);
        v[2] = Vector4(m20, m21, m22, m23);
        v[3] = Vector4(m30, m31, m32, m33);
    }

    Matrix4 Matrix4::transpose() {
        Matrix4 ret;

        ret.v[0].x = this->v[0].x;
        ret.v[0].y = this->v[1].x;
        ret.v[0].z = this->v[2].x;
        ret.v[0].w = this->v[3].x;

        ret.v[1].x = this->v[0].y;
        ret.v[1].y = this->v[1].y;
        ret.v[1].z = this->v[2].y;
        ret.v[1].w = this->v[3].y;

        ret.v[2].x = this->v[0].z;
        ret.v[2].y = this->v[1].z;
        ret.v[2].z = this->v[2].z;
        ret.v[2].w = this->v[3].z;

        ret.v[3].x = this->v[0].w;
        ret.v[3].y = this->v[1].w;
        ret.v[3].z = this->v[2].w;
        ret.v[3].w = this->v[3].w;      

        return ret;
    }

    Matrix4 Matrix4::Identity() {
        Matrix4 ret;
        ret.v = {
            Vector4(1, 0, 0, 0),
            Vector4(0, 1, 0, 0),
            Vector4(0, 0, 1, 0),
            Vector4(0, 0, 0, 1)
        };

        return ret;
    }

    Matrix4 Matrix4::FromColumnMajor(const float mat[16]) {
        Matrix4 ret = {
            Vector4{mat[0], mat[4], mat[8], mat[12]},
            Vector4{mat[1], mat[5], mat[9], mat[13]},
            Vector4{mat[2], mat[6], mat[10], mat[14]},
            Vector4{mat[3], mat[7], mat[11], mat[15]},
        };

        return ret;
    }

    Matrix4 Matrix4::Scale(Matrix4 mat, float scale) {
        return Matrix4::Scale(mat, Vector3(scale, scale, scale));
    }

    Matrix4 Matrix4::Scale(Matrix4 mat, Vector3 s) {
        Matrix4 scale_matrix;
        scale_matrix.v = {
            Vector4(s.x,  0.0f, 0.0f, 0.0f),
            Vector4(0.0f, s.y,  0.0f, 0.0f),
            Vector4(0.0f, 0.0f, s.z,  0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f) 
        };

        return scale_matrix * mat;
    }

    Matrix4 Matrix4::Scale(Matrix4 mat, float scale_x, float scale_y, float scale_z) {
        return Matrix4::Scale(mat, Vector3(scale_x, scale_y, scale_z));
    }


    Matrix4 Matrix4::Rotate(Matrix4 mat, float theta, Vector3 axis) {
        float rad = DEGREES_TO_RAD(theta);
        float c = cosf(rad);
        float s = sinf(rad);
        float t = 1.0f - c;

        axis = axis.normalize();
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        Matrix4 rot;
        rot.v = {
            Vector4(t * x * x + c,     t * x * y - z * s, t * x * z + y * s, 0.0f),
            Vector4(t * x * y + z * s, t * y * y + c,     t * y * z - x * s, 0.0f),
            Vector4(t * x * z - y * s, t * y * z + x * s, t * z * z + c,     0.0f),
            Vector4(0.0f,              0.0f,              0.0f,              1.0f)
        };

        return rot * mat;
    }

    Matrix4 Matrix4::Rotate(Matrix4 mat, float theta, float rot_x, float rot_y, float rot_z) {
        return Matrix4::Rotate(mat, theta, Vector3(rot_x, rot_y, rot_z));
    }

    Matrix4 Matrix4::Rotate(Matrix4 mat, Quaternion quat) {
        float theta;
        Vector3 axis;
        quat.to_angle_axis(theta, axis);
        return Matrix4::Rotate(mat, theta, axis);
    }

    Matrix4 Matrix4::Translate(Matrix4 mat, Vector3 t) {
        Matrix4 translate_matrix;
        translate_matrix.v = {
            Vector4(1.0f, 0.0f, 0.0f, t.x),
            Vector4(0.0f, 1.0f, 0.0f, t.y),
            Vector4(0.0f, 0.0f, 1.0f, t.z),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f)
        };

        return translate_matrix * mat;
    }

    Matrix4 Matrix4::Translate(Matrix4 mat, float x, float y, float z) {
        return Matrix4::Translate(mat, Vector3(x, y, z));
    }

    Matrix4 Matrix4::Transform(Vector3 s, float theta, Vector3 axis, Vector3 t) {
        Matrix4 scale_matrix = Matrix4::Scale(Matrix4::Identity(), s);
        Matrix4 rotation_matrix = Matrix4::Rotate(Matrix4::Identity(), theta, axis);
        Matrix4 translation_matrix = Matrix4::Translate(Matrix4::Identity(), t);

        return translation_matrix * rotation_matrix * scale_matrix;
    }

    Matrix4 Matrix4::InverseTransform(Vector3 s, float theta, Vector3 axis, Vector3 t) {
        Matrix4 inverse_scale_matrix = Matrix4::Scale(Matrix4::Identity(), s.scale(1 / s.x, 1 / s.y, 1 / s.z));
        Matrix4 inverse_rotation_matrix = Matrix4::Rotate(Matrix4::Identity(), theta, axis).transpose();
        Matrix4 inverse_translation_matrix = Matrix4::Translate(Matrix4::Identity(), t.scale(-1));

        return inverse_scale_matrix * inverse_rotation_matrix * inverse_translation_matrix;
    }

    void Matrix4::Decompose(Matrix4 mat, Vector3* out_position, Quaternion* out_orientation, Vector3* out_scale) {
        Vector3 translation = Vector3(mat.v[0].w, mat.v[1].w, mat.v[2].w);
        Vector3 scale = Vector3(0);
        {
            Vector3 column1 = Vector3(mat.v[0].x, mat.v[1].x, mat.v[2].x);
            Vector3 column2 = Vector3(mat.v[0].y, mat.v[1].y, mat.v[2].y);
            Vector3 column3 = Vector3(mat.v[0].z, mat.v[1].z, mat.v[2].z);
        
            float scale_x = column1.magnitude();
            float scale_y = column2.magnitude();
            float scale_z = column3.magnitude();
            scale = Vector3(scale_x, scale_y, scale_z);
        }

        Quaternion orientation = Quaternion::Identity();
        {
            Vector3 column1 = Vector3(mat.v[0].x, mat.v[1].x, mat.v[2].x);
            Vector3 column2 = Vector3(mat.v[0].y, mat.v[1].y, mat.v[2].y);
            Vector3 column3 = Vector3(mat.v[0].z, mat.v[1].z, mat.v[2].z);
            
            column1 = column1.scale(1.0f / scale.x);
            column2 = column2.scale(1.0f / scale.y);
            column3 = column3.scale(1.0f / scale.z);
            
            Matrix4 rotation_matrix = Matrix4(
                Vector4{column1.x, column2.x, column3.x, 0},
                Vector4{column1.y, column2.y, column3.y, 0},
                Vector4{column1.z, column2.z, column3.z, 0},
                Vector4{0,         0,         0,         0}
            );
            orientation = Quaternion::FromRotationMatrix(rotation_matrix);

            /*
            // TRS = M
            // R = T⁻¹ * M * S⁻¹
            // R = T⁻¹ * (TRS) * S⁻¹
            // R = T⁻¹ * TR * I
            // R = I * R * I
            // R = R
            Vector3 inverse_scale = Vector3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
            Matrix4 inverse_scale_matrix = Matrix4::scale(Matrix4::Identity(), inverse_scale);
            Matrix4 inverse_translation_matrix = Matrix4::Translate(Matrix4::Identity(), translation.scale(-1));
            Matrix4 rotation_matrix = inverse_translation_matrix * mat * inverse_scale_matrix;
            orientation = Quaternion::fromRotationMatrix(rotation_matrix);
            */
        }

        if (out_position) {
            *out_position = translation;
        }

        if (out_scale) {
            *out_scale = scale;
        }

        if (out_orientation) {
            *out_orientation = orientation;
        }
    }

    Matrix4 Matrix4::Perspective(float fov_degrees, float aspect, float near_plane, float far_plane) {
        float fov_radians = DEGREES_TO_RAD(fov_degrees);

        const float t = tanf(fov_radians / 2) * near_plane;
        const float b = -t;
        const float r = t * aspect;
        const float l = -t * aspect;

        const float p = (2.0f * near_plane);

        const float A = p / (r - l);
        const float B = p / (t - b);
        const float C = -((far_plane + near_plane) / (far_plane - near_plane));
        const float D = -((p * far_plane) / (far_plane - near_plane));

        Matrix4 ret;
        ret.v = {
            Vector4(A,  0,  0,  0),
            Vector4(0,  B,  0,  0),
            Vector4(0,  0,  C,  D),
            Vector4(0,  0, -1,  0)
        };

        return ret;
    }

    // Found at: https://en.wikipedia.org/wiki/Orthographic_projection
    Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane) {
        const float A = 2.0f / (right - left);
        const float B = 2.0f / (top - bottom);
        const float C = -2.0f / (far_plane - near_plane);
        const float D = -(right + left) / (right - left);
        const float E = -(top + bottom) / (top - bottom);
        const float F = -(far_plane + near_plane) / (far_plane - near_plane);

        Matrix4 ret = {
            A,  0,  0,  D,
            0,  B,  0,  E,
            0,  0,  C,  F,
            0,  0,  0,  1
        };

        return ret;
    }

    // Found at: https://www.khronos.org/opengl/wiki/GluLookAt_code
    Matrix4 Matrix4::Lookat(Vector3 position, Vector3 target, Vector3 world_up) {
        Vector3 forward = (position - target).normalize();
        Vector3 right   = Vector3::cross(world_up, forward).normalize();
        Vector3 up      = Vector3::cross(forward, right).normalize();

        Matrix4 rotation;
        rotation.v = {
            Vector4(right.x,   right.y,   right.z,   0),
            Vector4(up.x,      up.y,      up.z,      0),
            Vector4(forward.x, forward.y, forward.z, 0),
            Vector4(0,         0,         0,         1)
        };
        
        Matrix4 translation = Matrix4::Translate(Matrix4::Identity(), -position.x, -position.y, -position.z);

        return rotation * translation;
    }

    float gm_mat3_determinant_helper(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
        return a * (e * i - f * h) -
                b * (d * i - f * g) +
                c * (d * h - e * g);
    }

    Matrix4 Matrix4::inverse(bool &success) {
        success = false;

        float m00 = this->v[0].x, m01 = this->v[0].y, m02 = this->v[0].z, m03 = this->v[0].w;
        float m10 = this->v[1].x, m11 = this->v[1].y, m12 = this->v[1].z, m13 = this->v[1].w;
        float m20 = this->v[2].x, m21 = this->v[2].y, m22 = this->v[2].z, m23 = this->v[2].w;
        float m30 = this->v[3].x, m31 = this->v[3].y, m32 = this->v[3].z, m33 = this->v[3].w;

        float c00 = gm_mat3_determinant_helper(m11, m12, m13, m21, m22, m23, m31, m32, m33);
        float c01 = gm_mat3_determinant_helper(m10, m12, m13, m20, m22, m23, m30, m32, m33);
        float c02 = gm_mat3_determinant_helper(m10, m11, m13, m20, m21, m23, m30, m31, m33);
        float c03 = gm_mat3_determinant_helper(m10, m11, m12, m20, m21, m22, m30, m31, m32);

        float det = m00 * c00 - m01 * c01 + m02 * c02 - m03 * c03;
        if (NEAR_ZERO(det)) {
            return Matrix4::Identity();
        }

        float invDet = 1.0f / det;
        Matrix4 inv;

        // Row 0
        inv.v[0].x = invDet * c00;
        inv.v[0].y = invDet * (-gm_mat3_determinant_helper(m01, m02, m03, m21, m22, m23, m31, m32, m33));
        inv.v[0].z = invDet * gm_mat3_determinant_helper(m01, m02, m03, m11, m12, m13, m31, m32, m33);
        inv.v[0].w = invDet * (-gm_mat3_determinant_helper(m01, m02, m03, m11, m12, m13, m21, m22, m23));

        // Row 1
        inv.v[1].x = invDet * (-c01);
        inv.v[1].y = invDet * gm_mat3_determinant_helper(m00, m02, m03, m20, m22, m23, m30, m32, m33);
        inv.v[1].z = invDet * (-gm_mat3_determinant_helper(m00, m02, m03, m10, m12, m13, m30, m32, m33));
        inv.v[1].w = invDet * gm_mat3_determinant_helper(m00, m02, m03, m10, m12, m13, m20, m22, m23);

        // Row 2
        inv.v[2].x = invDet * c02;
        inv.v[2].y = invDet * (-gm_mat3_determinant_helper(m00, m01, m03, m20, m21, m23, m30, m31, m33));
        inv.v[2].z = invDet * gm_mat3_determinant_helper(m00, m01, m03, m10, m11, m13, m30, m31, m33);
        inv.v[2].w = invDet * (-gm_mat3_determinant_helper(m00, m01, m03, m10, m11, m13, m20, m21, m23));

        // Row 3
        inv.v[3].x = invDet * (-c03);
        inv.v[3].y = invDet * gm_mat3_determinant_helper(m00, m01, m02, m20, m21, m22, m30, m31, m32);
        inv.v[3].z = invDet * (-gm_mat3_determinant_helper(m00, m01, m02, m10, m11, m12, m30, m31, m32));
        inv.v[3].w = invDet * gm_mat3_determinant_helper(m00, m01, m02, m10, m11, m12, m20, m21, m22);

        success = true;

        return inv;
    }

    Matrix4 Matrix4::operator*(const Matrix4 &right) {
        Matrix4 C;

        for (int i = 0; i < 4; i++) {
            C.v[i].x += this->v[i].x * right.v[0].x;
            C.v[i].x += this->v[i].y * right.v[1].x;
            C.v[i].x += this->v[i].z * right.v[2].x;
            C.v[i].x += this->v[i].w * right.v[3].x;

            C.v[i].y += this->v[i].x * right.v[0].y;
            C.v[i].y += this->v[i].y * right.v[1].y;
            C.v[i].y += this->v[i].z * right.v[2].y;
            C.v[i].y += this->v[i].w * right.v[3].y;

            C.v[i].z += this->v[i].x * right.v[0].z;
            C.v[i].z += this->v[i].y * right.v[1].z;
            C.v[i].z += this->v[i].z * right.v[2].z;
            C.v[i].z += this->v[i].w * right.v[3].z;
            
            C.v[i].w += this->v[i].x * right.v[0].w;
            C.v[i].w += this->v[i].y * right.v[1].w;
            C.v[i].w += this->v[i].z * right.v[2].w;
            C.v[i].w += this->v[i].w * right.v[3].w;
        }
            
        return C;
    }

    Vector4 Matrix4::operator*(const Vector4 &right) {
        Vector4 ret;
        ret.x += this->v[0].x * right.x;
        ret.x += this->v[0].y * right.y;
        ret.x += this->v[0].z * right.z;
        ret.x += this->v[0].w * right.w;

        ret.y += this->v[1].x * right.x;
        ret.y += this->v[1].y * right.y;
        ret.y += this->v[1].z * right.z;
        ret.y += this->v[1].w * right.w;

        ret.z += this->v[2].x * right.x;
        ret.z += this->v[2].y * right.y;
        ret.z += this->v[2].z * right.z;
        ret.z += this->v[2].w * right.w;
        
        ret.w += this->v[3].x * right.x;
        ret.w += this->v[3].y * right.y;
        ret.w += this->v[3].z * right.z;
        ret.w += this->v[3].w * right.w;
            
        return ret;
    }

    Matrix4& Matrix4::operator*=(const Matrix4 &right) {
        *this = *this * right;
        return *this;
    }

    bool Matrix4::operator==(const Matrix4 &right) {
        return (this->v[0] == right.v[0]) && (this->v[1] == right.v[1]) && (this->v[2] == right.v[2]) && (this->v[3] == right.v[3]);
    }

    bool Matrix4::operator!=(const Matrix4 &right) {
        return !(*this == right);
    }
} 

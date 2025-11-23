#pragma once

#include <array>

#include <Math/vector.hpp>

namespace Math {
    typedef struct Quaternion Quaternion;

    // Matrices are ROW-MAJOR
    struct Mat4 {
        std::array<Vector4, 4> v;

        Mat4();
        Mat4(Vector4 r0, Vector4 r1, Vector4 r2, Vector4 r3);
        Mat4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        );

        Mat4 transpose();
        Mat4 inverse(bool &success);

        static Mat4 Identity();
        static Mat4 FromColumnMajor(const float mat[16]);
        static Mat4 Scale(Mat4 mat, float scale);
        static Mat4 Scale(Mat4 mat, Vector3 s);
        static Mat4 Scale(Mat4 mat, float scale_x, float scale_y, float scale_z);
        static Mat4 Rotate(Mat4 mat, float theta, Vector3 axis);
        static Mat4 Rotate(Mat4 mat, float theta, float rot_x, float rot_y, float rot_z);
        static Mat4 Rotate(Mat4 mat, Quaternion quat);
        static Mat4 Translate(Mat4 mat, Vector3 t);
        static Mat4 Translate(Mat4 mat, float x, float y, float z);
        static Mat4 Transform(Vector3 s, float theta, Vector3 axis, Vector3 t);
        static Mat4 InverseTransform(Vector3 s, float theta, Vector3 axis, Vector3 t);
        static void Decompose(Mat4 mat, Vector3* out_position, Quaternion* out_orientation, Vector3* out_scale);
        
        static Mat4 Perspective(float fov_degrees, float aspect, float near_plane, float far_plane);
        static Mat4 Orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane);
        static Mat4 Lookat(Vector3 position, Vector3 target, Vector3 world_up);

        Vector4 operator*(const Vector4 &right);
        Mat4 operator*(const Mat4 &right);
        Mat4& operator*=(const Mat4 &right);

        bool operator==(const Mat4 &right);
        bool operator!=(const Mat4 &right);
    };
}
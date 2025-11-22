#include <array>

#include <Math/vector.hpp>

namespace Math {
    typedef struct Quaternion Quaternion;

    // Matrices are ROW-MAJOR
    struct Matrix4 {
        std::array<Vector4, 4> v;

        Matrix4();
        Matrix4(Vector4 r0, Vector4 r1, Vector4 r2, Vector4 r3);
        Matrix4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        );

        Matrix4 transpose();
        Matrix4 inverse(bool &success);

        static Matrix4 Identity();
        static Matrix4 FromColumnMajor(const float mat[16]);
        static Matrix4 Scale(Matrix4 mat, float scale);
        static Matrix4 Scale(Matrix4 mat, Vector3 s);
        static Matrix4 Scale(Matrix4 mat, float scale_x, float scale_y, float scale_z);
        static Matrix4 Rotate(Matrix4 mat, float theta, Vector3 axis);
        static Matrix4 Rotate(Matrix4 mat, float theta, float rot_x, float rot_y, float rot_z);
        static Matrix4 Rotate(Matrix4 mat, Quaternion quat);
        static Matrix4 Translate(Matrix4 mat, Vector3 t);
        static Matrix4 Translate(Matrix4 mat, float x, float y, float z);
        static Matrix4 Transform(Vector3 s, float theta, Vector3 axis, Vector3 t);
        static Matrix4 InverseTransform(Vector3 s, float theta, Vector3 axis, Vector3 t);
        static void Decompose(Matrix4 mat, Vector3* out_position, Quaternion* out_orientation, Vector3* out_scale);
        
        static Matrix4 Perspective(float fov_degrees, float aspect, float near_plane, float far_plane);
        static Matrix4 Orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane);
        static Matrix4 Lookat(Vector3 position, Vector3 target, Vector3 world_up);

        Vector4 operator*(const Vector4 &right);
        Matrix4 operator*(const Matrix4 &right);
        Matrix4& operator*=(const Matrix4 &right);

        bool operator==(const Matrix4 &right);
        bool operator!=(const Matrix4 &right);
    };
}
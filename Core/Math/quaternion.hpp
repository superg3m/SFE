#pragma once

#include <Math/vector.hpp>
#include <Math/matrix.hpp>

namespace Math {
    struct Quaternion {
        float w;
        Vector3 v;

        Quaternion();
        Quaternion(float theta, Vector3 axis);
        Quaternion(float theta, float x, float y, float z);

        Quaternion inverse();
        Quaternion normalize();
        Quaternion scale(float scale);
        Matrix4 getMatrix4();
        void getAngleAxis(float &theta, Vector3 &vec);

        static Quaternion Identity();
        static Quaternion Literal(float w, Vector3 axis);
        static Quaternion Literal(float w, float x, float y, float z);
        static Quaternion FromEuler(Vector3 euler_angles_degrees);
        static Quaternion FromAngleAxis(float w, Vector3 axis);
        static Quaternion FromRotationMatrix(const float m[16]);
        static Quaternion FromRotationMatrix(Matrix4 mat);
        static Quaternion Slerp(Quaternion q, Quaternion r, float t);
        static float Dot(Quaternion a, Quaternion b);

        Quaternion operator+(const Quaternion &right);
        Quaternion& operator+=(const Quaternion &right);
        
        Quaternion operator-(const Quaternion &right);
        Quaternion& operator-=(const Quaternion &right);

        Quaternion operator*(const Quaternion &right);
        Quaternion& operator*=(const Quaternion &right);

        Vector3 operator*(const Vector3 &right);

        bool operator==(const Quaternion &right);
        bool operator!=(const Quaternion &right);
    };
} 



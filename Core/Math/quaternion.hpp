#pragma once

#include <Math/vector.hpp>
#include <Math/matrix.hpp>

namespace Math {
    struct Quaternion {
        float w;
        Vec3 v;

        Quaternion();
        Quaternion(float theta, Vec3 axis);
        Quaternion(float theta, float x, float y, float z);

        Quaternion inverse();
        Quaternion normalize();
        Quaternion scale(float scale);
        Mat4 getMat4();
        void getAngleAxis(float &theta, Vec3 &vec);

        static Quaternion Identity();
        static Quaternion Literal(float w, Vec3 axis);
        static Quaternion Literal(float w, float x, float y, float z);
        static Quaternion FromEuler(Vec3 euler_angles_degrees);
        static Quaternion FromAngleAxis(float w, Vec3 axis);
        static Quaternion FromRotationMatrix(const float m[16]);
        static Quaternion FromRotationMatrix(Mat4 mat);
        static Quaternion SLerp(Quaternion q, Quaternion r, float t);
        static float Dot(Quaternion a, Quaternion b);

        Quaternion operator+(const Quaternion &right);
        Quaternion& operator+=(const Quaternion &right);
        
        Quaternion operator-(const Quaternion &right);
        Quaternion& operator-=(const Quaternion &right);

        Quaternion operator*(const Quaternion &right);
        Quaternion& operator*=(const Quaternion &right);

        Vec3 operator*(const Vec3 &right);

        bool operator==(const Quaternion &right);
        bool operator!=(const Quaternion &right);
    };
} 



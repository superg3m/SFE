#pragma once

namespace Math {
    struct Vector2 {
        float x;
        float y;

        Vector2();
        explicit Vector2(float fill);
        explicit Vector2(float x, float y);

        float magnitude();
        float magnitudeSquared();
        Vector2 normalize();
        Vector2 scale(float scale) const;
        Vector2 scale(Vector2 s) const;
        Vector2 scale(float scale_x, float scale_y) const;

        /**
         * @brief The return value tells you:
         * -1: the vectors are 180 degrees from eachother in other words they vectors are pointing in opposite directions
         *  0: the vectors are perpendicular or orthogonal to eachother
         *  1: the vectors are going the same direction
         * 
         * @param a 
         * @param b 
         * @return float 
         */
        static float dot(Vector2 a, Vector2 b);
        static float distance(Vector2 a, Vector2 b);
        static float distanceSquared(Vector2 a, Vector2 b);
        static Vector2 lerp(Vector2 a, Vector2 b, float t);
        static Vector2 euler(float yaw, float pitch);

        Vector2 operator+(const Vector2 &right);
        Vector2& operator+=(const Vector2 &right);

        Vector2 operator-(const Vector2 &right);
        Vector2& operator-=(const Vector2 &right);

        Vector2 operator*(const Vector2 &right);
        Vector2& operator*=(const Vector2 &right);

        Vector2 operator/(const Vector2 &right);
        Vector2& operator/=(const Vector2 &right);

        bool operator==(const Vector2 &right);
        bool operator!=(const Vector2 &right);
    };

    typedef struct Vector4 Vector4;
    struct Vector3 {
        union {
            struct {
                float x;
                float y;
                float z;
            };

            struct {
                float r;
                float g;
                float b;
            };
        };

        Vector3();
        explicit Vector3(float fill);
        explicit Vector3(float x, float y, float z);
        explicit Vector3(Vector2 v, float z);
        explicit Vector3(Vector4 v);

        float magnitude();
        float magnitudeSquared();
        Vector3 normalize();
        Vector3 scale(float scale) const;
        Vector3 scale(Vector3 s) const;
        Vector3 scale(float scale_x, float scale_y, float scale_z) const;

        /**
         * @brief The return value tells you:
         * -1: the vectors are 180 degrees from eachother in other words they vectors are pointing in opposite directions
         *  0: the vectors are perpendicular or orthogonal to eachother
         *  1: the vectors are going the same direction
         * 
         * @param a 
         * @param b 
         * @return float 
         */
        static float dot(Vector3 a, Vector3 b);
        static float distance(Vector3 a, Vector3 b);
        static float distanceSquared(Vector3 a, Vector3 b);
        static Vector3 lerp(Vector3 a, Vector3 b, float t);
        static Vector3 cross(Vector3 a, Vector3 b);
        static Vector3 euler(float yaw, float pitch);

        Vector3 operator+(const Vector3 &right);
        Vector3& operator+=(const Vector3 &right);

        Vector3 operator-(const Vector3 &right);
        Vector3& operator-=(const Vector3 &right);

        Vector3 operator*(const Vector3 &right);
        Vector3& operator*=(const Vector3 &right);

        Vector3 operator/(const Vector3 &right);
        Vector3& operator/=(const Vector3 &right);

        bool operator==(const Vector3 &right);
        bool operator!=(const Vector3 &right);
    };

    struct Vector4 {
        union {
            struct {
                float x;
                float y;
                float z;
                float w;
            };

            struct {
                float r;
                float g;
                float b;
                float a;
            };
        };

        Vector4();
        explicit Vector4(float fill);
        explicit Vector4(float x, float y, float z, float w);
        explicit Vector4(Vector3 v, float w);

        float magnitude();
        float magnitudeSquared();
        Vector4 normalize();
        Vector4 scale(float scale) const;
        Vector4 scale(Vector4 s) const;
        Vector4 scale(float scale_x, float scale_y, float scale_z, float scale_w) const;

        /**
         * @brief The return value tells you:
         * -1: the vectors are 180 degrees from eachother in other words they vectors are pointing in opposite directions
         *  0: the vectors are perpendicular or orthogonal to eachother
         *  1: the vectors are going the same direction
         * 
         * @param a 
         * @param b 
         * @return float 
         */
        static float dot(Vector4 a, Vector4 b);
        static Vector4 lerp(Vector4 a, Vector4 b, float t);
        static float distance(Vector4 a, Vector4 b);
        static float distanceSquared(Vector4 a, Vector4 b);

        Vector4 operator+(const Vector4 &right);
        Vector4& operator+=(const Vector4 &right);

        Vector4 operator-(const Vector4 &right);
        Vector4& operator-=(const Vector4 &right);

        Vector4 operator*(const Vector4 &right);
        Vector4& operator*=(const Vector4 &right);

        Vector4 operator/(const Vector4 &right);
        Vector4& operator/=(const Vector4 &right);

        bool operator==(const Vector4 &right);
        bool operator!=(const Vector4 &right);
    };
};
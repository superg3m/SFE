#include <Common/common.hpp>
#include <Math/vector.hpp>

namespace Math {
    Vector2::Vector2() {
        this->x = 0.0f;
        this->y = 0.0f;
    }

    Vector2::Vector2(float fill) {
        this->x = fill;
        this->y = fill;
    }

    Vector2::Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    float Vector2::magnitude() {
        return sqrtf((this->x * this->x) + (this->y * this->y));
    }

    float Vector2::magnitudeSquared() {
        return (this->x * this->x) + (this->y * this->y);
    }

    Vector2 Vector2::normalize() {
        Vector2 ret(0, 0);
        const float magnitude = this->magnitude();
        if (magnitude == 0) {
            return Vector2(0,0);
        }

        ret.x = this->x / magnitude;
        ret.y = this->y / magnitude;

        return ret;
    }

    Vector2 Vector2::scale(float scale) const {
        return Vector2(this->x * scale, this->y * scale);
    }

    Vector2 Vector2::scale(Vector2 s) const {
        return Vector2(this->x * s.x, this->y * s.y);
    }

    Vector2 Vector2::scale(float scale_x, float scale_y) const {
        return Vector2(this->x * scale_x, this->y * scale_y);
    }

    float Vector2::dot(Vector2 a, Vector2 b) {
        return (a.x * b.x) + (a.y * b.y);
    }

    float Vector2::distance(Vector2 a, Vector2 b) {
        return sqrtf(SQUARED(b.x - a.x) + SQUARED(b.y - a.y));
    }

    float Vector2::distanceSquared(Vector2 a, Vector2 b) {
        return SQUARED(b.x - a.x) + SQUARED(b.y - a.y);
    }

    Vector2 Vector2::lerp(Vector2 a, Vector2 b, float t) {
        Vector2 ab = (b - a);
        return a + (ab.scale(t));
    }

    Vector2 Vector2::euler(float yaw, float pitch) {
        Vector2 ret = Vector2(0, 0);
        ret.x = cosf(DEGREES_TO_RAD(yaw)) * cosf(DEGREES_TO_RAD(pitch));
        ret.y = sinf(DEGREES_TO_RAD(pitch));

        return ret;
    }

    Vector2 Vector2::operator+(const Vector2 &right) {
        return Vector2(this->x + right.x, this->y + right.y);
    }
    Vector2& Vector2::operator+=(const Vector2 &right) {
        this->x += right.x;
        this->y += right.y;

        return *this;
    }

    Vector2 Vector2::operator-(const Vector2 &right) {
        return Vector2(this->x - right.x, this->y - right.y);
    }
    Vector2& Vector2::operator-=(const Vector2 &right) {
        this->x -= right.x;
        this->y -= right.y;

        return *this;
    }

    Vector2 Vector2::operator*(const Vector2 &right) {
        return Vector2(this->x * right.x, this->y * right.y);
    }

    Vector2& Vector2::operator*=(const Vector2 &right) {
        this->x *= right.x;
        this->y *= right.y;

        return *this;
    }

    Vector2 Vector2::operator/(const Vector2 &right) {
        return Vector2(this->x / right.x, this->y / right.y);
    }
    Vector2& Vector2::operator/=(const Vector2 &right) {
        this->x /= right.x;
        this->y /= right.y;

        return *this;
    }

    bool Vector2::operator==(const Vector2 &right) {
        return NEAR_ZERO(this->x - right.x) && NEAR_ZERO(this->y - right.y);
    }

    bool Vector2::operator!=(const Vector2 &right) {
        return !(*this == right);
    }

    Vector3::Vector3() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    Vector3::Vector3(float fill) {
        this->x = fill;
        this->y = fill;
        this->z = fill;
    }

    Vector3::Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3::Vector3(Vector2 v, float z) {
        this->x = v.x;
        this->y = v.y;
        this->z = z;
    }

    Vector3::Vector3(Vector4 v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    float Vector3::magnitude() {
        return sqrtf(SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z));
    }

    float Vector3::magnitudeSquared() {
        return SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z);
    }

    Vector3 Vector3::normalize() {
        Vector3 ret(0, 0, 0);
        const float magnitude = this->magnitude();
        if (magnitude == 0) {
            return Vector3(0, 0, 0);
        }

        ret.x = this->x / magnitude;
        ret.y = this->y / magnitude;
        ret.z = this->z / magnitude;

        return ret;
    }

    Vector3 Vector3::scale(float scale) const {
        return Vector3(this->x * scale, this->y * scale, this->z * scale);
    }

    Vector3 Vector3::scale(Vector3 s) const {
        return Vector3(this->x * s.x, this->y * s.y, this->z * s.z);
    }

    Vector3 Vector3::scale(float scale_x, float scale_y, float scale_z) const {
        return Vector3(this->x * scale_x, this->y * scale_y, this->z * scale_z);
    }

    float Vector3::distance(Vector3 a, Vector3 b) {
        return sqrtf(SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z));
    }

    float Vector3::distanceSquared(Vector3 a, Vector3 b) {
        return SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z);
    }

    float Vector3::dot(Vector3 a, Vector3 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    Vector3 Vector3::lerp(Vector3 a, Vector3 b, float t) {
        Vector3 ab = (b - a);
        return a + (ab.scale(t));
    }

    Vector3 Vector3::cross(Vector3 A, Vector3 B) {
        Vector3 ret(0, 0, 0);
        ret.x = A.y * B.z - A.z * B.y;
        ret.y = A.z * B.x - A.x * B.z;
        ret.z = A.x * B.y - A.y * B.x;

        return ret;
    }

    Vector3 Vector3::euler(float yaw, float pitch) {
        Vector3 ret = Vector3(0, 0, 0);
        ret.x = cosf(DEGREES_TO_RAD(yaw)) * cosf(DEGREES_TO_RAD(pitch));
        ret.y = sinf(DEGREES_TO_RAD(pitch));
        ret.z = sinf(DEGREES_TO_RAD(yaw)) * cosf(DEGREES_TO_RAD(pitch));

        return ret;
    }

    Vector3 Vector3::operator+(const Vector3 &right) {
        return Vector3(this->x + right.x, this->y + right.y, this->z + right.z);
    }
    Vector3& Vector3::operator+=(const Vector3 &right) {
        this->x += right.x;
        this->y += right.y;
        this->z += right.z;

        return *this;
    }

    Vector3 Vector3::operator-(const Vector3 &right) {
        return Vector3(this->x - right.x, this->y - right.y, this->z - right.z);
    }
    Vector3& Vector3::operator-=(const Vector3 &right) {
        this->x -= right.x;
        this->y -= right.y;
        this->z -= right.z;

        return *this;
    }


    Vector3 Vector3::operator*(const Vector3 &right) {
        return Vector3(this->x * right.x, this->y * right.y, this->z * right.z);
    }
    Vector3& Vector3::operator*=(const Vector3 &right) {
        this->x *= right.x;
        this->y *= right.y;
        this->z *= right.z;

        return *this;
    }


    Vector3 Vector3::operator/(const Vector3 &right) {
        return Vector3(this->x / right.x, this->y / right.y, this->z / right.z);
    }
    Vector3& Vector3::operator/=(const Vector3 &right) {
        this->x /= right.x;
        this->y /= right.y;
        this->z /= right.z;

        return *this;
    }

    bool Vector3::operator==(const Vector3 &right) {
        return NEAR_ZERO(this->x - right.x) && NEAR_ZERO(this->y - right.y) && NEAR_ZERO(this->z - right.z);
    }
    bool Vector3::operator!=(const Vector3 &right) {
        return !(*this == right);
    }

    Vector4::Vector4() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }


    Vector4::Vector4(float fill) {
        this->x = fill;
        this->y = fill;
        this->z = fill;
        this->w = fill;
    }

    Vector4::Vector4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vector4::Vector4(Vector3 v, float w) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = w;
    }

    float Vector4::magnitude() {
        return sqrtf(SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z) + SQUARED(this->w));
    }

    float Vector4::magnitudeSquared() {
        return SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z) + SQUARED(this->w);
    }

    Vector4 Vector4::normalize() {
        Vector4 ret(0, 0, 0, 0);
        const float magnitude = this->magnitude();
        if (magnitude == 0) {
            return Vector4(0, 0, 0, 0);
        }

        ret.x = this->x / magnitude;
        ret.y = this->y / magnitude;
        ret.z = this->z / magnitude;
        ret.w = this->w / magnitude;

        return ret;
    }

    Vector4 Vector4::scale(float scale) const {
        return Vector4(this->x * scale, this->y * scale, this->z * scale, this->w * scale);
    }

    Vector4 Vector4::scale(Vector4 s) const {
        return Vector4(this->x * s.x, this->y * s.y, this->z * s.z, this->w * s.w);
    }

    Vector4 Vector4::scale(float scale_x, float scale_y, float scale_z, float scale_w) const {
        return Vector4(this->x * scale_x, this->y * scale_y, this->z * scale_z, this->w * scale_w);
    }

    float Vector4::distance(Vector4 a, Vector4 b) {
        return sqrtf(SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z) + SQUARED(b.w - a.w));
    }

    float Vector4::distanceSquared(Vector4 a, Vector4 b) {
        return SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z) + SQUARED(b.w - a.w);
    }

    float Vector4::dot(Vector4 a, Vector4 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
    }

    Vector4 Vector4::lerp(Vector4 a, Vector4 b, float t) {
        Vector4 ab = (b - a);
        return a + (ab.scale(t));
    }


    Vector4 Vector4::operator+(const Vector4 &right) {
        return Vector4(this->x + right.x, this->y + right.y, this->z + right.z, this->w + right.w);
    }
    Vector4& Vector4::operator+=(const Vector4 &right) {
        this->x += right.x;
        this->y += right.y;
        this->z += right.z;
        this->w += right.w;

        return *this;
    }

    Vector4 Vector4::operator-(const Vector4 &right) {
        return Vector4(this->x - right.x, this->y - right.y, this->z - right.z, this->w - right.w);
    }
    Vector4& Vector4::operator-=(const Vector4 &right) {
        this->x -= right.x;
        this->y -= right.y;
        this->z -= right.z;
        this->w -= right.w;

        return *this;
    }


    Vector4 Vector4::operator*(const Vector4 &right) {
        return Vector4(this->x * right.x, this->y * right.y, this->z * right.z, this->w * right.w);
    }
    Vector4& Vector4::operator*=(const Vector4 &right) {
        this->x *= right.x;
        this->y *= right.y;
        this->z *= right.z;
        this->w *= right.w;

        return *this;
    }


    Vector4 Vector4::operator/(const Vector4 &right) {
        return Vector4(this->x / right.x, this->y / right.y, this->z / right.z, this->w / right.w);
    }
    Vector4& Vector4::operator/=(const Vector4 &right) {
        this->x /= right.x;
        this->y /= right.y;
        this->z /= right.z;
        this->w /= right.w;

        return *this;
    }

    bool Vector4::operator==(const Vector4 &right) {
        return NEAR_ZERO(this->x - right.x) && NEAR_ZERO(this->y - right.y) && NEAR_ZERO(this->z - right.z) && NEAR_ZERO(this->w - right.w);
    }
    bool Vector4::operator!=(const Vector4 &right) {
        return !(*this == right);
    }
};
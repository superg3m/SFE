#pragma once

namespace Math {
    typedef struct Vector3 Vector3;

    float Lerp(float a, float b, float t);
    float inverseLerp(float a, float b, float value);
    Vector3 barycentric(Vector3 a, Vector3 b, Vector3 c, float u, float v);
    float remap(float x, float s_min, float s_max, float e_min, float e_max);
    float moveoward(float current, float target, float delta);
    float smoothstep(float edge0, float edge1, float x);
    float smootherstep(float edge0, float edge1, float x);
    int mod(int a, int b);
} 

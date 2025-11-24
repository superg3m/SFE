#pragma once

namespace Math {
    typedef struct Vec3 Vec3;

    float Lerp(float a, float b, float t);
    float inverseLerp(float a, float b, float value);
    Vec3 barycentric(Vec3 a, Vec3 b, Vec3 c, float u, float v);
    float remap(float x, float s_min, float s_max, float e_min, float e_max);
    float moveoward(float current, float target, float delta);
    float smoothstep(float edge0, float edge1, float x);
    float smootherstep(float edge0, float edge1, float x);
    int mod(int a, int b);
} 

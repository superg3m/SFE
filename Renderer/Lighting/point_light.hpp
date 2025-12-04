#pragma once

#include <Math/math.hpp>

struct PointLight {
    Math::Vec3 position;

    float constant;
    float linear;
    float quadratic;

    Math::Vec3 ambient;
    Math::Vec3 diffuse;
    Math::Vec3 specular;
};

struct PointLightLocation {
    unsigned int position;

    unsigned int constant;
    unsigned int linear;
    unsigned int quadratic;

    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};
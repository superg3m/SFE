#pragma once

#include <Math/math.hpp>

struct PointLight {
    Math::Vector3 position;

    float constant;
    float linear;
    float quadratic;

    Math::Vector3 ambient;
    Math::Vector3 diffuse;
    Math::Vector3 specular;
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
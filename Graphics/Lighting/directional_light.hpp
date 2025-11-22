#pragma once

#include <Math/math.hpp>

struct DirectionalLight {
    Math::Vector3 direction;

    Math::Vector3 ambient;
    Math::Vector3 diffuse;
    Math::Vector3 specular;
};

struct DirectionalLightLocation {
    unsigned int direction;

    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};


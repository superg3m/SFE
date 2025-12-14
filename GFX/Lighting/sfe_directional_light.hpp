#pragma once

#include "../../Core/Math/sfe_math.hpp"

struct DirectionalLight {
    Math::Vec3 direction;

    Math::Vec3 ambient;
    Math::Vec3 diffuse;
    Math::Vec3 specular;
};

struct DirectionalLightLocation {
    unsigned int direction;

    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};


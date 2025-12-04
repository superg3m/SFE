#pragma once

#include <Math/math.hpp>

struct SpotLight {
    Math::Vec3 position;
    Math::Vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
 
    Math::Vec3 ambient;
    Math::Vec3 diffuse;
    Math::Vec3 specular;        
};

struct SpotLightLocation {
    unsigned int position;
    unsigned int direction;
    unsigned int cutOff;
    unsigned int outerCutOff;

    unsigned int constant;
    unsigned int linear;
    unsigned int quadratic;
 
    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};

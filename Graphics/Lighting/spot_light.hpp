#pragma once

#include <Math/math.hpp>

struct SpotLight {
    Math::Vector3 position;
    Math::Vector3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
 
    Math::Vector3 ambient;
    Math::Vector3 diffuse;
    Math::Vector3 specular;        
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

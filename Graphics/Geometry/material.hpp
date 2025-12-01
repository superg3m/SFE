#pragma once

#include <Math/math.hpp>

using GLTextureID = int;
enum TextureType {
    TEXTURE_TYPE_UNKNOWN = 0,
    TEXTURE_TYPE_DIFFUSE,
    TEXTURE_TYPE_SPECULAR,
    TEXTURE_TYPE_AMBIENT,
    TEXTURE_TYPE_EMISSIVE,
    TEXTURE_TYPE_HEIGHT,
    TEXTURE_TYPE_NORMALS,
    TEXTURE_TYPE_SHININESS,
    TEXTURE_TYPE_OPACITY,
    TEXTURE_TYPE_DISPLACEMENT,
    TEXTURE_TYPE_LIGHT_MAP,
    TEXTURE_TYPE_REFLECTION,

    TEXTURE_COUNT
};

static const char* texture_to_string[TEXTURE_COUNT] = {
    stringify(TEXTURE_TYPE_UNKNOWN),
    stringify(TEXTURE_TYPE_DIFFUSE),
    stringify(TEXTURE_TYPE_SPECULAR),
    stringify(TEXTURE_TYPE_AMBIENT),
    stringify(TEXTURE_TYPE_EMISSIVE),
    stringify(TEXTURE_TYPE_HEIGHT),
    stringify(TEXTURE_TYPE_NORMALS),
    stringify(TEXTURE_TYPE_SHININESS),
    stringify(TEXTURE_TYPE_OPACITY),
    stringify(TEXTURE_TYPE_DISPLACEMENT),
    stringify(TEXTURE_TYPE_LIGHT_MAP),
    stringify(TEXTURE_TYPE_REFLECTION)
};

STATIC_ASSERT(ArrayCount(texture_to_string) == (TEXTURE_COUNT));

struct Texture {
    char* name = nullptr;
    GLTextureID id = -1;
};

struct Material {
    Texture color_map;
    Texture specular_map;
    Texture normal_map;
    Texture height_map;

    float shininess = 32.0f;
    float opacity = 1.0f;

    Math::Vec3 ambient_color = Math::Vec3(1.0f);
    Math::Vec3 diffuse_color = Math::Vec3(1.0f);
    Math::Vec3 specular_color = Math::Vec3(1.0f);
};

struct MaterialLocation {
    unsigned int color_map;
    unsigned int specular_map;
    unsigned int normal_map;
    unsigned int height_map;

    unsigned int color;
    unsigned int shininess;
    unsigned int opacity;
};
#pragma once

#include <Math/math.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>
#include <assimp/version.h>

#include <Texture/texture.hpp>

enum TextureType {
    TEXTURE_TYPE_NONE = aiTextureType_NONE,
    TEXTURE_TYPE_DIFFUSE = aiTextureType_DIFFUSE,
    TEXTURE_TYPE_SPECULAR = aiTextureType_SPECULAR,
    TEXTURE_TYPE_AMBIENT = aiTextureType_AMBIENT,
    TEXTURE_TYPE_EMISSIVE = aiTextureType_EMISSIVE,
    TEXTURE_TYPE_HEIGHT = aiTextureType_HEIGHT,
    TEXTURE_TYPE_NORMALS = aiTextureType_NORMALS,
    TEXTURE_TYPE_SHININESS = aiTextureType_SHININESS,
    TEXTURE_TYPE_OPACITY = aiTextureType_OPACITY,
    TEXTURE_TYPE_DISPLACEMENT = aiTextureType_DISPLACEMENT,
    TEXTURE_TYPE_LIGHTMAP = aiTextureType_LIGHTMAP,
    TEXTURE_TYPE_REFLECTION = aiTextureType_REFLECTION,

    TEXTURE_COUNT
};

static const char* texture_to_string[TEXTURE_COUNT] = {
    stringify(TEXTURE_TYPE_NONE),
    stringify(TEXTURE_TYPE_DIFFUSE),
    stringify(TEXTURE_TYPE_SPECULAR),
    stringify(TEXTURE_TYPE_AMBIENT),
    stringify(TEXTURE_TYPE_EMISSIVE),
    stringify(TEXTURE_TYPE_HEIGHT),
    stringify(TEXTURE_TYPE_NORMALS),
    stringify(TEXTURE_TYPE_SHININESS),
    stringify(TEXTURE_TYPE_OPACITY),
    stringify(TEXTURE_TYPE_DISPLACEMENT),
    stringify(TEXTURE_TYPE_LIGHTMAP),
    stringify(TEXTURE_TYPE_REFLECTION)
};

STATIC_ASSERT(ArrayCount(texture_to_string) == (TEXTURE_COUNT));

struct Material {
    int index; // the material index
    
    Texture textures[TEXTURE_COUNT];

    float shininess = 32.0f;
    float opacity = 1.0f;

    Math::Vec3 ambient_color = Math::Vec3(1.0f);
    Math::Vec3 diffuse_color = Math::Vec3(1.0f);
    Math::Vec3 specular_color = Math::Vec3(1.0f);
};

struct MaterialLocation {
    unsigned int textures[TEXTURE_COUNT];

    unsigned int ambient_color;
    unsigned int diffuse_color;
    unsigned int specular_color;
    unsigned int shininess;
    unsigned int opacity;
};
#include "../Math/math.hpp"
#include "../Common/common.hpp"
#include "shader_base.hpp"

using GLTextureID = int;
enum TextureType {
    TEXTURE_TYPE_DIFFUSE = 0,
    TEXTURE_TYPE_SPECULAR,
    TEXTURE_TYPE_NORMAL,
    TEXTURE_TYPE_HEIGHT,

    TEXTURE_COUNT
};

static const char* texture_to_string[TEXTURE_COUNT] = {
    stringify(TEXTURE_TYPE_DIFFUSE),
    stringify(TEXTURE_TYPE_SPECULAR),
    stringify(TEXTURE_TYPE_NORMAL),
    stringify(TEXTURE_TYPE_HEIGHT),
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

    Math::Vector3 color = Math::Vector3(1);
    float shininess = 32.0f;
    float opacity = 1.0f;
};
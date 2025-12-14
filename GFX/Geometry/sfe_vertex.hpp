#pragma once

#include <glad/glad.h>

#include "../../Core/Math/sfe_math.hpp"
#include "../../Core/Common/sfe_common.hpp"

namespace GFX {
    const float MAGIC_NUMBER = -123450510.0f;
    const int RESERVED_VERTEX_ATTRIBUTE_LOCATIONS = 8;

    struct Vertex {
        Math::Vec3 aPosition    = Math::Vec3(MAGIC_NUMBER);   // location 0
        Math::Vec3 aNormal      = Math::Vec3(MAGIC_NUMBER);   // location 1
        Math::Vec2 aTexCoord    = Math::Vec2(MAGIC_NUMBER);   // location 2
        Math::Vec3 aTangent     = Math::Vec3(MAGIC_NUMBER);   // location 3
        Math::Vec3 aBitangent   = Math::Vec3(MAGIC_NUMBER);   // location 4
        Math::Vec3 aColor       = Math::Vec3(MAGIC_NUMBER);   // location 5
        Math::IVec4 aBoneIDs    = Math::IVec4(MAGIC_NUMBER);  // location 6
        Math::Vec4 aBoneWeights = Math::Vec4(MAGIC_NUMBER);   // location 7

        Vertex() = default;
        Vertex(Math::Vec3 pos);
        Vertex(Math::Vec3 pos, Math::Vec3 norm);
        Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv);
        Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv, Math::Vec3 tangent, Math::Vec3 bitangent);
        Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv, Math::Vec3 tangent, Math::Vec3 bitangent, Math::Vec3 color);
        Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv, Math::Vec3 tangent, Math::Vec3 bitangent, Math::Vec3 color, Math::IVec4 boneIDs, Math::Vec4 bone_weights);
    };

    enum class VertexAttributeFlag : uint32_t {
        INVALID       = 0x0,
        aPosition     = 0x1,
        aNormal       = 0x2,
        aTexCoord     = 0x4,
        aTangent      = 0x8,
        aBitangent    = 0x10,
        aColor        = 0x20,
        aBoneIDs      = 0x40,
        aBoneWeights  = 0x80,

        PNBundle = aPosition | aNormal,
        PNTBundle = aPosition | aNormal | aTexCoord,
        TangentBundle = PNTBundle | aTangent | aBitangent,
        ColorBundle = TangentBundle | aColor,
        All = ColorBundle | aBoneIDs | aBoneWeights,
    };

    inline bool operator&(VertexAttributeFlag a, VertexAttributeFlag b) {
        return static_cast<bool>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
    inline VertexAttributeFlag operator|(VertexAttributeFlag a, VertexAttributeFlag b) {
        return static_cast<VertexAttributeFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    struct AttributeDescriptor {
        VertexAttributeFlag flag;
        int location;
        int component_count;
        GLenum gl_type; // float, int
        bool normalized;
        bool is_integer;     
        size_t data_size;
        int is_instanced;
    };
}
#include <Math/math.hpp>
#include <glad/glad.h>

struct Vertex {
    Math::Vector3 aPosition = Math::Vector3(0.0f);    // location 0
    Math::Vector3 aNormal = Math::Vector3(0.0f);      // location 1
    Math::Vector2 aTexCoord = Math::Vector2(0.0f);    // location 2
    Math::Vector3 aTangent = Math::Vector3(0.0f);     // location 3
    Math::Vector3 aBitangent = Math::Vector3(0.0f);   // location 4
    Math::Vector3 aColor = Math::Vector3(1.0f);       // location 5
    Math::Vector4 aBoneIDs = Math::Vector4(0.0f);     // location 6 (use GM_IVec4 if you have it for integer IDs)
    Math::Vector4 aBoneWeights = Math::Vector4(0.0f); // location 7 (using vec4 for weights for simplicity)

    // Default constructor to initialize, or use default member initializers if C++11
    Vertex() = default;

    Vertex(Math::Vector3 pos) { 
        this->aPosition = pos;
    }

    Vertex(Math::Vector3 pos, Math::Vector3 norm) { 
        this->aPosition = pos;
        this->aNormal = norm;
    }

    Vertex(Math::Vector3 pos, Math::Vector3 norm, Math::Vector2 uv) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
    }

    Vertex(Math::Vector3 pos, Math::Vector3 norm, Math::Vector2 uv, Math::Vector3 tangent, Math::Vector3 bitangent) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
        this->aTangent = tangent;
        this->aBitangent = bitangent;
    }

    Vertex(Math::Vector3 pos, Math::Vector3 norm, Math::Vector2 uv, Math::Vector3 tangent, Math::Vector3 bitangent, Math::Vector3 color) : Vertex(pos, norm, uv, tangent, bitangent) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
        this->aTangent = tangent;
        this->aBitangent = bitangent;
        this->aColor = color;
    }

    Vertex(Math::Vector3 pos, Math::Vector3 norm, Math::Vector2 uv, Math::Vector3 tangent, Math::Vector3 bitangent, Math::Vector3 color, Math::Vector4 boneIDs, Math::Vector4 bone_weights) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
        this->aTangent = tangent;
        this->aBitangent = bitangent;
        this->aColor = color;
        this->aBoneIDs = boneIDs;
        this->aBoneWeights = bone_weights;
    }
};

enum class VertexAttributeFlag : uint32_t {
    aPosition     = 0x1,
    aNormal       = 0x2,
    aTexCoord     = 0x4,
    aTangent      = 0x8,
    aBitangent    = 0x10,
    aColor        = 0x20,
    aBoneIDs      = 0x40,
    aBoneWeights  = 0x80,

    PNTBundle = aPosition | aNormal | aTexCoord,
    TangentBundle = PNTBundle | aTangent | aBitangent,
    ColorBundle = TangentBundle | aColor,
    All = ColorBundle | aBoneIDs | aBoneWeights,
};

inline VertexAttributeFlag operator|(VertexAttributeFlag a, VertexAttributeFlag b) {
    return static_cast<VertexAttributeFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline bool hasVertexAttributeFlag(VertexAttributeFlag value, VertexAttributeFlag flag) {
    return (static_cast<uint32_t>(value) & static_cast<uint32_t>(flag)) == static_cast<uint32_t>(flag);
}

struct AttributeDescriptor {
    VertexAttributeFlag flag;
    int location;
    int componentCount;       // e.g., 3 for vec3, 4 for vec4
    GLenum glType;            // e.g., GL_FLOAT, GL_INT
    bool normalized;
    bool isInteger;           // True if using glVertexAttribIPointer
    size_t byteSize;
};

static const std::array<AttributeDescriptor, 8> ALL_ATTRIBUTE_DESCRIPTORS = {
    AttributeDescriptor{VertexAttributeFlag::aPosition,    0, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    AttributeDescriptor{VertexAttributeFlag::aNormal,      1, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    AttributeDescriptor{VertexAttributeFlag::aTexCoord,    2, 2, GL_FLOAT, GL_FALSE, false, sizeof(float) * 2},
    AttributeDescriptor{VertexAttributeFlag::aTangent,     3, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    AttributeDescriptor{VertexAttributeFlag::aBitangent,   4, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    AttributeDescriptor{VertexAttributeFlag::aColor,       5, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3}, // Or GL_UNSIGNED_BYTE, true, true
    AttributeDescriptor{VertexAttributeFlag::aBoneIDs,     6, 4, GL_INT,   GL_FALSE, true,  sizeof(int)   * 4}, // Use GL_INT and isInteger=true
    AttributeDescriptor{VertexAttributeFlag::aBoneWeights, 7, 4, GL_FLOAT, GL_FALSE, false, sizeof(float) * 4}
};
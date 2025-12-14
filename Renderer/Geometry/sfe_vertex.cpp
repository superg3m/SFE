#include "sfe_vertex.hpp"

namespace Renderer {
    const AttributeDescriptor ALL_ATTRIBUTE_DESCRIPTORS[8] = {
        AttributeDescriptor{VertexAttributeFlag::aPosition,    0, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3, false},
        AttributeDescriptor{VertexAttributeFlag::aNormal,      1, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3, false},
        AttributeDescriptor{VertexAttributeFlag::aTexCoord,    2, 2, GL_FLOAT, GL_FALSE, false, sizeof(float) * 2, false},
        AttributeDescriptor{VertexAttributeFlag::aTangent,     3, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3, false},
        AttributeDescriptor{VertexAttributeFlag::aBitangent,   4, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3, false},
        AttributeDescriptor{VertexAttributeFlag::aColor,       5, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3, false}, // Or GL_UNSIGNED_BYTE, true, true
        AttributeDescriptor{VertexAttributeFlag::aBoneIDs,     6, 4, GL_INT,   GL_FALSE, true,  sizeof(int)   * 4, false}, // Use GL_INT and isInteger=true
        AttributeDescriptor{VertexAttributeFlag::aBoneWeights, 7, 4, GL_FLOAT, GL_FALSE, false, sizeof(float) * 4, false}
    };

    Vertex::Vertex(Math::Vec3 pos) { 
        this->aPosition = pos;
    }

    Vertex::Vertex(Math::Vec3 pos, Math::Vec3 norm) { 
        this->aPosition = pos;
        this->aNormal = norm;
    }

    Vertex::Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
    }

    Vertex::Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv, Math::Vec3 tangent, Math::Vec3 bitangent) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
        this->aTangent = tangent;
        this->aBitangent = bitangent;
    }

    Vertex::Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv, Math::Vec3 tangent, Math::Vec3 bitangent, Math::Vec3 color) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
        this->aTangent = tangent;
        this->aBitangent = bitangent;
        this->aColor = color;
    }

    Vertex::Vertex(Math::Vec3 pos, Math::Vec3 norm, Math::Vec2 uv, Math::Vec3 tangent, Math::Vec3 bitangent, Math::Vec3 color, Math::IVec4 boneIDs, Math::Vec4 bone_weights) {
        this->aPosition = pos;
        this->aNormal = norm; 
        this->aTexCoord = uv;
        this->aTangent = tangent;
        this->aBitangent = bitangent;
        this->aColor = color;
        this->aBoneIDs = boneIDs;
        this->aBoneWeights = bone_weights;
    }
}
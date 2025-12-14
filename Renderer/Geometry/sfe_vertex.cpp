#include "sfe_vertex.hpp"

namespace Renderer {
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
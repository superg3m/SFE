#pragma once

#include <glad/glad.h>
#include "../../Core/Common/sfe_common.hpp"
#include "../GPUBuffer/sfe_vertex_buffer.hpp"
#include "../GPUBuffer/sfe_index_buffer.hpp"
#include "../Geometry/sfe_vertex.hpp"

namespace GFX {
    struct VertexArray {
        GLuint id = 0;

        VertexArray() = default;
        static VertexArray Create();

        void bind() const;
        void bindVBO(int location, bool instanced, const VertexBuffer& buffer);
        void bindEBO(const IndexBuffer& buffer);
    private:
        DS::Hashmap<int, bool> vertex_attribute_locations;
        void bindVertexAttribute(int &location, bool instanced, s64 stride, AttributeDesc desc);
    };
}
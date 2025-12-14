#pragma once

#include <glad/glad.h>
#include "../../Core/Common/sfe_common.hpp"
#include "../GPUBuffer/sfe_gpu_buffer.hpp"
#include "../Geometry/sfe_vertex.hpp"
#include "../GPUBuffer/sfe_gpu_buffer.hpp"

namespace Renderer {
    struct VertexArray {
        GLuint id = 0;

        VertexArray() = default;
        static VertexArray Create();

        void bind() const;
        void bindBuffer(const GPUBuffer& buffer);
    private:
        DS::Hashmap<int, bool> vertex_attribute_locations;
        void bindVertexAttribute(int location, bool instanced, s64 stride, VertexAttributeDescriptor desc);
    };
}
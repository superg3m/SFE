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
        void bindBuffer(int location, bool instanced, const GPUBuffer& buffer);
       
    private:
        DS::Hashmap<int, bool> vertex_attribute_locations;

        // track used locations
        void bindVertexAttribute(int &location, bool instanced, s64 stride, s64 offset, BufferStrideTypeInfo type_info);
    };
}
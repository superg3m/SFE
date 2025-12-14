#pragma once

#include <glad/glad.h>
#include "../../Core/Common/sfe_common.hpp"
#include "../../Core/DataStructure/sfe_ds.hpp"

namespace Renderer {
    enum class BufferType {
        VERTEX,
        INDEX,
        UNIFORM,
    };

    enum class BufferUsage {
        STATIC,
        DYNAMIC,
        STREAM
    };

    // NOTE(Jovanni): The values are the component_count
    enum class BufferStrideTypeInfo {
        BOOL = 1,
        INT = 1,
        FLOAT = 1, 
        VEC2 = 2,
        VEC3 = 3,
        VEC4 = 4, 
        IVEC4 = 4, 
        MAT4 = 16
    };

    // TODO(Jovanni): Delete these buffers prob put them in a vao or something
    struct GPUBuffer {
        GLuint id = 0;
        
        DS::Vector<BufferStrideTypeInfo> stride_type_info;
        int stride;
        BufferType type = type;
        BufferUsage usage = usage;
        
        GPUBuffer() = default;
        static GPUBuffer VBO(BufferType type, BufferUsage usage, int stride, DS::Vector<BufferStrideTypeInfo> stride_type_info, size_t buffer_size, void* buffer_data);
        static GPUBuffer EBO(int index_count, void* indices);
        void bind() const;
        void updateEntireBuffer(size_t buffer_size, void* buffer_data);

    private:
        GLenum gl_type; // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER
        GLenum gl_usage; // GL_STATIC_DRAW, GL_DYNAMIC_DRAW

        void allocate(size_t buffer_size, void* buffer_data);
    };
}
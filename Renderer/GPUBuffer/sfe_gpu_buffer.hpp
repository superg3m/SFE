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
        byte_t buffer_size = 0;
        
        DS::Vector<BufferStrideTypeInfo> stride_type_info;
        byte_t stride;
        BufferType type = type;
        BufferUsage usage = usage;
        
        GPUBuffer() = default;
        static GPUBuffer VBO(BufferType type, BufferUsage usage, byte_t stride, DS::Vector<BufferStrideTypeInfo> stride_type_info, byte_t buffer_size, void* buffer_data);
        static GPUBuffer EBO(byte_t indices_count, unsigned int* indices_data);
        void bind() const;
        void updateEntireBuffer(byte_t buffer_size, const void* buffer_data);

    private:
        GLenum gl_type; // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER
        GLenum gl_usage; // GL_STATIC_DRAW, GL_DYNAMIC_DRAW

        void allocate(const void* buffer_data);
    };
}
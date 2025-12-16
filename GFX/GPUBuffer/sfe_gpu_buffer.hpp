#pragma once

#include <glad/glad.h>
#include "../../Core/Common/sfe_common.hpp"
#include "../../Core/DataStructure/sfe_ds.hpp"
#include "../sfe_gl_check.hpp"

namespace GFX {
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

    struct AttributeDesc {
        s64 offset;
        BufferStrideTypeInfo type;

        AttributeDesc() = delete;
        AttributeDesc(s64 offset, BufferStrideTypeInfo type) {
            this->offset = offset;
            this->type = type;
        }
    };

    // TODO(Jovanni): Delete these buffers prob put them in a vao or something
    struct GPUBuffer {
        GLuint id = 0;
        
        DS::Vector<AttributeDesc> descriptors;
        int stride;
        BufferType type;
        BufferUsage usage;
        GLenum gl_type; // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER
        GLenum gl_usage; // GL_STATIC_DRAW, GL_DYNAMIC_DRAW
        
        GPUBuffer() = default;

        template <typename T>
        static GPUBuffer VBO(BufferUsage usage, DS::Vector<AttributeDesc> descriptors, DS::Vector<T> buffer) {
            GPUBuffer ret;
            ret.type = BufferType::VERTEX;
            ret.usage = usage;
            ret.stride = sizeof(T);
            ret.descriptors = descriptors;
            ret.gl_type = GL_ARRAY_BUFFER;
            ret.gl_usage = (
                (usage == BufferUsage::STATIC) ? GL_STATIC_DRAW : 
                (usage == BufferUsage::DYNAMIC) ? GL_DYNAMIC_DRAW :
                (usage == BufferUsage::STREAM) ? GL_STREAM_DRAW : -1
            );

            ret.allocate(sizeof(T) * buffer.count(), buffer.data());

            return ret;
        }

        static GPUBuffer EBO(DS::Vector<unsigned int> data);

        template <typename T>
        void updateEntireBuffer(DS::Vector<T> buffer) {
            #if 0
                this->bind();
                void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
                size_t buffer_size = sizeof(T) * buffer.count();
                Memory::Copy(ptr, buffer_size, buffer.data(), buffer_size);
                glUnmapBuffer(GL_ARRAY_BUFFER);
            #else
                glCheckError(glBindBuffer(this->gl_type, this->id));
                glCheckError(glBufferSubData(this->gl_type, 0, sizeof(T) * buffer.count(), buffer.data()));
            #endif
        }

        void bind() const;
    private:
        void allocate(size_t buffer_size, void* buffer_data);
    };
}
#include "sfe_gpu_buffer.hpp"
#include "../sfe_gl_check.hpp"

namespace Renderer {
    GPUBuffer GPUBuffer::VBO(BufferType type, BufferUsage usage, int stride, DS::Vector<BufferStrideTypeInfo> stride_type_info) {
        GPUBuffer ret;
        ret.type = type;
        ret.usage = usage;
        ret.stride_type_info = stride_type_info;
        ret.gl_type = (
            (type == BufferType::VERTEX) ? GL_ARRAY_BUFFER : 
            (type == BufferType::UNIFORM) ? GL_UNIFORM_BUFFER : -1
        );

        ret.gl_usage = (
            (usage == BufferUsage::STATIC) ? GL_STATIC_DRAW : 
            (usage == BufferUsage::DYNAMIC) ? GL_DYNAMIC_DRAW :
            (usage == BufferUsage::STREAM) ? GL_STREAM_DRAW : -1
        );

        return ret;
    }

    GPUBuffer GPUBuffer::EBO() {
        GPUBuffer ret;
        ret.type = BufferType::INDEX;
        ret.usage = BufferUsage::STATIC;
        ret.gl_type = GL_ELEMENT_ARRAY_BUFFER;
        ret.gl_usage = GL_STATIC_DRAW;

        return ret;
    }

    // TODO(Jovanni): make this cached
    void GPUBuffer::bind() const {
        glCheckError(glBindBuffer(this->gl_type, this->id));
    }

    void GPUBuffer::updateEntireBuffer(size_t buffer_size, const void* buffer_data) {
        this->bind();
        glCheckError(glBufferSubData(this->id, 0, buffer_size, buffer_data));
    }

    void GPUBuffer::allocate(size_t buffer_size, const void* buffer_data) {
        glCheckError(glGenBuffers(1, &this->id));
        this->bind();

        if (buffer_data) {
            LOG_DEBUG("id: %d, gl_usage: %d\n", this->id, this->gl_usage);
            glCheckError(glBufferData(this->id, buffer_size, buffer_data, this->gl_usage));
        }
    }
}
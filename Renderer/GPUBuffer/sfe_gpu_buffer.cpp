#include "sfe_gpu_buffer.hpp"
#include "../sfe_gl_check.hpp"

namespace Renderer {
    GPUBuffer GPUBuffer::VBO(BufferType type, BufferUsage usage, int stride, DS::Vector<BufferStrideTypeInfo> stride_type_info, size_t buffer_size, void* buffer_data) {
        GPUBuffer ret;
        ret.type = type;
        ret.usage = usage;
        ret.stride = stride;
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

        ret.allocate(buffer_size, buffer_data);

        return ret;
    }

    GPUBuffer GPUBuffer::EBO(int index_count, void* indices) {
        GPUBuffer ret;
        ret.type = BufferType::INDEX;
        ret.usage = BufferUsage::STATIC;
        ret.gl_type = GL_ELEMENT_ARRAY_BUFFER;
        ret.gl_usage = GL_STATIC_DRAW;

        ret.allocate(sizeof(unsigned int) * index_count, indices);

        return ret;
    }

    // TODO(Jovanni): make this cached
    void GPUBuffer::bind() const {
        glCheckError(glBindBuffer(this->gl_type, this->id));
    }

    void GPUBuffer::updateEntireBuffer(size_t buffer_size, void* buffer_data) {
        this->bind();
        glCheckError(glBufferSubData(this->id, 0, buffer_size, buffer_data));
    }

    void GPUBuffer::allocate(size_t buffer_size, void* buffer_data) {
        glCheckError(glGenBuffers(1, &this->id));
        this->bind();

        glCheckError(glBufferData(this->gl_type, buffer_size, buffer_data, this->gl_usage));
    }
}
#include "sfe_gpu_buffer.hpp"

namespace Renderer {
    GPUBuffer::GPUBuffer(BufferType type, BufferUsage usage, byte_t stride, DS::Vector<BufferStrideTypeInfo> stride_type_info, byte_t buffer_size, const void* buffer_data) {
        this->type = type;
        this->usage = usage;

        this->gl_type = (
            (type == BufferType::VERTEX) ? GL_ARRAY_BUFFER: 
            (type == BufferType::INDEX) ? GL_ELEMENT_ARRAY_BUFFER : 
            GL_UNIFORM_BUFFER
        );

        this->gl_usage = (
            (usage == BufferUsage::STATIC) ? GL_STATIC_DRAW : 
            (usage == BufferUsage::DYNAMIC) ? GL_DYNAMIC_DRAW : 
            GL_STREAM_DRAW
        );

        this->stride = stride;
        this->stride_type_info = stride_type_info;
        this->buffer_size = buffer_size;
        this->allocate(buffer_data);
    }

    // TODO(Jovanni): make this cached
    void GPUBuffer::bind() const {
        glBindBuffer(this->gl_type, id);
    }

    void GPUBuffer::updateEntireBuffer(byte_t buffer_size, const void* buffer_data) {
        RUNTIME_ASSERT(this->buffer_size == buffer_size);

        this->bind();
        glBufferSubData(this->id, 0, this->buffer_size, buffer_data);
    }

    void GPUBuffer::allocate(const void* buffer_data) {
        this->bind();

        glGenBuffers(1, &this->id);
        glBufferData(this->id, this->buffer_size, buffer_data, this->gl_usage);
    }
}
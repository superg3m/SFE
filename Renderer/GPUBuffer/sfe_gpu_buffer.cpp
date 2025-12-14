#include "sfe_gpu_buffer.hpp"
#include "../sfe_gl_check.hpp"

namespace Renderer {
    GPUBuffer GPUBuffer::VBO(BufferType type, BufferUsage usage, byte_t stride, DS::Vector<BufferStrideTypeInfo> stride_type_info, byte_t buffer_size, void* buffer_data) {
        GPUBuffer ret;
        ret.type = type;
        ret.usage = usage;
        ret.stride_type_info = stride_type_info;
        ret.buffer_size = buffer_size;
        ret.allocate(buffer_data);

        return ret;
    }

    GPUBuffer GPUBuffer::EBO(byte_t indices_count, unsigned int* indices_data) {
        GPUBuffer ret;
        ret.type = BufferType::INDEX;
        ret.usage = BufferUsage::STATIC;
        ret.buffer_size = sizeof(unsigned int) * indices_count;
        ret.allocate(indices_data);

        return ret;
    }

    // TODO(Jovanni): make this cached
    void GPUBuffer::bind() const {
        glCheckError(glBindBuffer(this->gl_type, id));
    }

    void GPUBuffer::updateEntireBuffer(byte_t buffer_size, const void* buffer_data) {
        RUNTIME_ASSERT(this->buffer_size == buffer_size);

        this->bind();
        glCheckError(glBufferSubData(this->id, 0, this->buffer_size, buffer_data));
    }

    void GPUBuffer::allocate(const void* buffer_data) {
        this->bind();

        glCheckError(glGenBuffers(1, &this->id));
        if (buffer_data) {
            glCheckError(glBufferData(this->id, this->buffer_size, buffer_data, this->gl_usage));
        }
    }
}
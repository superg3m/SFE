#include "sfe_gpu_buffer.hpp"

namespace GFX {
    GPUBuffer GPUBuffer::EBO(DS::Vector<unsigned int> buffer) {
        GPUBuffer ret;
        ret.type = BufferType::INDEX;
        ret.usage = BufferUsage::STATIC;
        ret.gl_type = GL_ELEMENT_ARRAY_BUFFER;
        ret.gl_usage = GL_STATIC_DRAW;

        ret.allocate(sizeof(unsigned int) * buffer.count(), buffer.data());

        return ret;
    }

    void GPUBuffer::allocate(size_t buffer_size, void* buffer_data) {
        glCheckError(glGenBuffers(1, &this->id));
        glCheckError(glBindBuffer(this->gl_type, this->id));

        glCheckError(glBufferData(this->gl_type, buffer_size, buffer_data, this->gl_usage));
    }
}
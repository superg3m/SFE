#include "sfe_gpu_buffer.hpp"
#include "../sfe_gl_check.hpp"

namespace GFX {
    template <typename T>
    GPUBuffer GPUBuffer::VBO(BufferUsage usage, DS::Vector<AttributeDesc> descriptors, DS::Vector<T> buffer) {
        GPUBuffer ret;
        ret.type = type;
        ret.usage = usage;
        ret.stride = stride;
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

    GPUBuffer GPUBuffer::EBO(DS::Vector<unsigned int> buffer) {
        GPUBuffer ret;
        ret.type = BufferType::INDEX;
        ret.usage = BufferUsage::STATIC;
        ret.gl_type = GL_ELEMENT_ARRAY_BUFFER;
        ret.gl_usage = GL_STATIC_DRAW;

        ret.allocate(sizeof(unsigned int) * buffer.count(), buffer.data());

        return ret;
    }

    void GPUBuffer::updateEntireBuffer(size_t buffer_size, void* buffer_data) {
        glCheckError(glBindBuffer(this->gl_type, this->id));
        glCheckError(glBufferSubData(this->id, 0, buffer_size, buffer_data));
    }

    void GPUBuffer::allocate(size_t buffer_size, void* buffer_data) {
        glCheckError(glGenBuffers(1, &this->id));
        glCheckError(glBindBuffer(this->gl_type, this->id));

        glCheckError(glBufferData(this->gl_type, buffer_size, buffer_data, this->gl_usage));
    }
}
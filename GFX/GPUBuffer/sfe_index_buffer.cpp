#include "sfe_index_buffer.hpp"

namespace GFX {
    IndexBuffer IndexBuffer::Create(DS::Vector<unsigned int> buffer) {
        IndexBuffer ret;
        ret.allocate(sizeof(unsigned int) * buffer.count(), buffer.data());

        return ret;
    }


    void IndexBuffer::bind() const {
        glCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id));
    }

    void IndexBuffer::allocate(size_t buffer_size, void* buffer_data) {
        glCheckError(glGenBuffers(1, &this->id));
        
        this->bind();
        glCheckError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, buffer_data, GL_STATIC_DRAW));
    }
}
#include "sfe_vertex_buffer.hpp"

namespace GFX {
    void VertexBuffer::bind() const {
        glCheckError(glBindBuffer(GL_ARRAY_BUFFER, this->id));
    }

    void VertexBuffer::allocate(size_t buffer_size, void* buffer_data) {
        glCheckError(glGenBuffers(1, &this->id));
        
        this->bind();
        glCheckError(glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer_data, this->gl_usage));
    }
}
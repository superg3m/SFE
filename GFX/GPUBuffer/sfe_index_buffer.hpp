#pragma once

#include <glad/glad.h>
#include "../../Core/Common/sfe_common.hpp"
#include "../../Core/DataStructure/sfe_ds.hpp"
#include "../sfe_gl_check.hpp"

namespace GFX {
    // TODO(Jovanni): Delete these buffers prob put them in a vao or something
    struct IndexBuffer {
        GLuint id = 0;
        
        GLenum gl_type; // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER
        
        IndexBuffer() = default;

        static IndexBuffer Create(DS::Vector<unsigned int> data);
        void bind() const;
    private:
        void allocate(size_t buffer_size, void* buffer_data);
    };
}
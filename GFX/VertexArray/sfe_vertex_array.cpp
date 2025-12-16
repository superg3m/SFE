#include "sfe_vertex_array.hpp"
#include "../sfe_gl_check.hpp"
#include "../sfe_renderer.hpp" 

namespace GFX {
    VertexArray VertexArray::Create() { 
        VertexArray ret;
        ret.vertex_attribute_locations = DS::Hashmap<int, bool>(1);
        glCheckError(glGenVertexArrays(1, &ret.id));

        return ret;
    }

    inline void VertexArray::bind() const { 
        GFX::BindVAO((this->id));
    }

    void VertexArray::bindVBO(int location, bool instanced, const GPUBuffer& buffer) {
        RUNTIME_ASSERT(buffer.type == BufferType::VERTEX);
    
        this->bind();
        buffer.bind();
        for (AttributeDesc desc : buffer.descriptors) {
            this->bindVertexAttribute(location, instanced, buffer.stride, desc);
        }

        if (this->vertex_attribute_locations.count() == 0) {
            this->vertex_attribute_locations = DS::Hashmap<int, bool>(1);
        }
    }

    void VertexArray::bindEBO(const GPUBuffer& buffer) {
        RUNTIME_ASSERT(buffer.type == BufferType::INDEX);

        this->bind();
        buffer.bind();
    }

    void VertexArray::bindVertexAttribute(int &location, bool instanced, s64 stride, AttributeDesc desc) {
        RUNTIME_ASSERT_MSG(
            !this->vertex_attribute_locations.has(location),
            "Location already assigned"
        );

        bool is_integer = (desc.type == BufferStrideTypeInfo::INT) || (desc.type == BufferStrideTypeInfo::IVEC4);
        GLenum gl_type  = is_integer ? GL_INT : GL_FLOAT;
        bool is_matrix = desc.type == BufferStrideTypeInfo::MAT4;

        int max_attributes = 0;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
        RUNTIME_ASSERT_MSG(max_attributes > location, "Location outside range");

        if (is_matrix) {
            for (int i = 0; i < 4; i++) {
                RUNTIME_ASSERT_MSG(max_attributes > location + i,
                    "You are trying to use a location that is outside of the max vertex attributes: %d",
                    max_attributes
                );

                glEnableVertexAttribArray(location + i);
                glVertexAttribPointer(location + i, (int)BufferStrideTypeInfo::VEC4, GL_FLOAT, false, stride, (void*)(desc.offset + (sizeof(Math::Vec4) * i)));
                glVertexAttribDivisor(location + i, instanced);
                this->vertex_attribute_locations.put(location + i, true);
            }

            location += 4;
        } else {
            glEnableVertexAttribArray(location);
            if (is_integer) {
                glVertexAttribIPointer(location, (int)desc.type, gl_type, stride, (void*)desc.offset);
            } else {
                glVertexAttribPointer(location, (int)desc.type, gl_type, false, stride, (void*)desc.offset);
            }

            glVertexAttribDivisor(location, instanced);
            this->vertex_attribute_locations.put(location, true);
            location += 1;
        }
    }
}
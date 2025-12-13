#include "sfe_vertex_array.hpp"

namespace Renderer {
    VertexArray VertexArray::Create() { 
        VertexArray ret;
        ret.vertex_attribute_locations = DS::Hashmap<int, bool>(1);

        glGenVertexArrays(1, &ret.id);
        return ret;
    }

    void VertexArray::bind() const { 
        glBindVertexArray(this->id); 
    }

    // TODO(Jovanni): this is scuffeds
    void VertexArray::bindBuffer(int location, bool instanced, const GPUBuffer& buffer) {
        this->bind();
        buffer.bind();

        int start_location = location; 

        byte_t offset = 0;
        for (const auto& type_info : buffer.stride_type_info) {
            this->bindVertexAttribute(location, instanced, buffer.stride, offset, type_info);
            offset += (int)type_info;
        }

        // hacky
        if (this->vertex_attribute_locations.count() == 0) {
            this->vertex_attribute_locations = DS::Hashmap<int, bool>(1);
        }

        for (int i = start_location; i < location; i++) {
            this->vertex_attribute_locations.put(i, true);
        }
    }

    void VertexArray::bindVertexAttribute(int &location, bool instanced, byte_t stride, byte_t offset, BufferStrideTypeInfo type_info) {
        RUNTIME_ASSERT_MSG(
            !this->vertex_attribute_locations.has(location),
            "Location already assigned"
        );

        bool is_integer = (type_info == BufferStrideTypeInfo::INT) || (type_info == BufferStrideTypeInfo::IVEC4);
        GLenum gl_type  = is_integer ? GL_INT : GL_FLOAT;
        bool is_matrix = type_info == BufferStrideTypeInfo::MAT4;

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
                glVertexAttribPointer(location + i, 4, gl_type, false, stride, (void*)(offset + (sizeof(Math::Vec4) * i)));
                // this->vertex_attribute_locations.put(location + i, true);
                
                glVertexAttribDivisor(location + i, instanced);
            }

            location += 4;
        } else {
            glEnableVertexAttribArray(location);
            if (is_integer) {
                glVertexAttribIPointer(location, (int)type_info, gl_type, stride, (void*)offset);
            } else {
                glVertexAttribPointer(location, (int)type_info, gl_type, false, stride, (void*)offset);
            }

            // this->vertex_attribute_locations.put(location, true);
            glVertexAttribDivisor(location, instanced);
            location += 1;
        }
    }
}
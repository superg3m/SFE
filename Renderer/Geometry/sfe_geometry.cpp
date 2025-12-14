#include <float.h>

#include <stb_image.h>

#include "../../Core/DataStructure/sfe_contiguous.hpp"
#include "../../Core/Random/sfe_random.hpp"
#include "../ShaderBase/sfe_shader_base.hpp"
#include "../sfe_renderer_state.hpp"
#include "../sfe_gl_check.hpp"
#include "sfe_geometry.hpp"
#include "sfe_vertex.hpp"

namespace Renderer {
    static Math::Mat4 convertAssimpMatrixToGM(aiMatrix4x4 ai_matrix) {
        Math::Mat4 ret;

        ret.v[0].x = ai_matrix.a1; ret.v[1].x = ai_matrix.b1; ret.v[2].x = ai_matrix.c1; ret.v[3].x = ai_matrix.d1; 
        ret.v[0].y = ai_matrix.a2; ret.v[1].y = ai_matrix.b2; ret.v[2].y = ai_matrix.c2; ret.v[3].y = ai_matrix.d2;
        ret.v[0].z = ai_matrix.a3; ret.v[1].z = ai_matrix.b3; ret.v[2].z = ai_matrix.c3; ret.v[3].z = ai_matrix.d3;
        ret.v[0].w = ai_matrix.a4; ret.v[1].w = ai_matrix.b4; ret.v[2].w = ai_matrix.c4; ret.v[3].w = ai_matrix.d4;

        return ret;
    }

    static Math::AABB CalculateAABB(const DS::Vector<Vertex>& vertices) {
        float x_min = FLT_MAX;
        float y_min = FLT_MAX;
        float z_min = FLT_MAX;

        float x_max = FLT_MIN;
        float y_max = FLT_MIN;
        float z_max = FLT_MIN;
        for (const Vertex v : vertices) {
            float x = v.aPosition.x;
            float y = v.aPosition.y;
            float z = v.aPosition.z;

            if (x_min > x) {
                x_min = x;
            } else if (x_max < x) {
                x_max = x;
            }

            if (y_min > y) {
                y_min = y;
            } else if (y_max < y) {
                y_max = y;
            }

            if (z_min > z) {
                z_min = z;
            } else if (z_max < z) {
                z_max = z;
            }
        }

        Math::Vec3 center  = Math::Vec3(
            (x_max + x_min) / 2.0f,
            (y_max + y_min) / 2.0f,
            (z_max + z_min) / 2.0f
        );

        Math::Vec3 extents = Math::Vec3(
            (x_max - x_min) / 2.0f,
            (y_max - y_min) / 2.0f,
            (z_max - z_min) / 2.0f
        );

        return Math::AABB::FromCenterExtents(center, extents);
    }

    Geometry::Geometry() {
        this->draw_type = GL_TRIANGLES;
        this->vertex_count = 0;
        this->index_count = 0;

        this->base_vertex = 0;
        this->base_index = 0;

        this->next = nullptr;
    }

    Geometry::Geometry(const DS::Vector<Vertex>& vertices, GLenum draw_type) {
        RUNTIME_ASSERT(vertices.count() > 0);

        this->draw_type = draw_type;
        this->vertices = vertices;
        this->vertex_count = this->vertices.count();
        this->index_count = this->indices.count();

        this->setup();
    }

    Geometry::Geometry(const DS::Vector<Vertex>& vertices, const DS::Vector<unsigned int>& indices, GLenum draw_type) {
        RUNTIME_ASSERT(vertices.count() > 0);

        this->draw_type = draw_type;
        this->vertices = vertices;
        this->indices = indices;
        this->vertex_count = this->vertices.count();
        this->index_count = this->indices.count();

        this->setup();
    }

    Geometry Geometry::Quad() {
        DS::Vector<Vertex> quad_vertices = {
            //         Position                              Normal                        UV
            Vertex{Math::Vec3( 0.5f,  0.5f, 0.0f),  Math::Vec3(1.0f, 0.0f, 0.0f),  Math::Vec2(1, 1)}, // top right
            Vertex{Math::Vec3( 0.5f, -0.5f, 0.0f),  Math::Vec3(0.0f, 1.0f, 0.0f),  Math::Vec2(1, 0)}, // bottom right
            Vertex{Math::Vec3(-0.5f, -0.5f, 0.0f),  Math::Vec3(0.0f, 0.0f, 1.0f),  Math::Vec2(0, 0)}, // bottom left
            Vertex{Math::Vec3(-0.5f,  0.5f, 0.0f),  Math::Vec3(1.0f, 1.0f, 0.0f),  Math::Vec2(0, 1)}  // top left 
        }; 

        DS::Vector<unsigned int> quad_indices = {
            0, 1, 3,
            1, 2, 3
        };

        Geometry ret;
        ret.draw_type = GL_TRIANGLES;
        ret.vertices = quad_vertices;
        ret.indices = quad_indices;
        ret.vertex_count = ret.vertices.count();
        ret.index_count = ret.indices.count();

        ret.setup();

        return ret;
    }

    Geometry Geometry::Quad(int width, int height) {
        RUNTIME_ASSERT_MSG((width & 1) == 0, "width must be even\n");
        RUNTIME_ASSERT_MSG((height & 1) == 0, "height must be even\n");

        const int TOTAL_VERTEX_COUNT = width * height;
        DS::Vector<Vertex> vertices = DS::Vector<Vertex>(TOTAL_VERTEX_COUNT);
        vertices.resize(TOTAL_VERTEX_COUNT);

        int idx = 0;
        const int HALF_WIDTH = width / 2;
        const int HALF_HEIGHT = height / 2;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                float x = j - HALF_WIDTH;
                float y = 0.0f;
                float z = HALF_HEIGHT - i;

                const float u = Math::Remap(j, 0, width, 0, 1);
                const float v = Math::Remap(i, 0, height - 1, 0, 1);

                vertices[idx].aPosition = Math::Vec3(x, y, z);
                vertices[idx++].aTexCoord = Math::Vec2(u, v);
            }
        }

        DS::Vector<unsigned> indices = DS::Vector<unsigned>((width - 1) * (height - 1) * 6);
        for (int i = 0; i < height - 1; i++) {
            for (int j = 0; j < width - 1; j++) {
                unsigned tl =  i      * width + j;
                unsigned tr =  i      * width + (j + 1);
                unsigned bl = (i + 1) * width + j;
                unsigned br = (i + 1) * width + (j + 1);

                // Triangle 1 (counter-clockwise)
                indices.push(tl);
                indices.push(bl);
                indices.push(tr);

                // Triangle 2 (counter-clockwise)
                indices.push(bl);
                indices.push(br);
                indices.push(tr);
            }
        }

        Geometry ret;
        ret.draw_type = GL_TRIANGLES;
        ret.vertices = vertices;
        ret.indices = indices;

        ret.vertex_count = vertices.count();
        ret.index_count = indices.count();

        ret.setup();
        return ret;
    }

    Geometry Geometry::AABB() {
        DS::Vector<Vertex> aabb_vertices = {
            Vertex{Math::Vec3(-1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3( 1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3( 1.0f, -1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, -1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3(-1.0f, -1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3(-1.0f, -1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3(-1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},

            Vertex{Math::Vec3(-1.0f, 1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3( 1.0f, 1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, 1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3( 1.0f, 1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, 1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3(-1.0f, 1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3(-1.0f, 1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3(-1.0f, 1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},

            Vertex{Math::Vec3(-1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3(-1.0f, 1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3( 1.0f, 1.0f, -1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, -1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3( 1.0f, 1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3(-1.0f, -1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)}, Vertex{Math::Vec3(-1.0f, 1.0f,  1.0f), Math::Vec3(0, 0, 0), Math::Vec2(0, 0)},
        };

        Geometry ret;
        ret.draw_type = GL_LINES;
        ret.vertices = aabb_vertices;
        ret.vertex_count = ret.vertices.count();
        ret.index_count = ret.indices.count();
        ret.setup();

        return ret;
    }


    Geometry Geometry::Cube() {
        DS::Vector<Vertex> cube_vertices = {
            // Front face
            Vertex{Math::Vec3(-1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, -1), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, -1.0f, -1.0f), Math::Vec3(0, 0, -1), Math::Vec2(1, 0)},
            Vertex{Math::Vec3( 1.0f,  1.0f, -1.0f), Math::Vec3(0, 0, -1), Math::Vec2(1, 1)},
            Vertex{Math::Vec3(-1.0f,  1.0f, -1.0f), Math::Vec3(0, 0, -1), Math::Vec2(0, 1)},

            // Back face
            Vertex{Math::Vec3(-1.0f, -1.0f, 1.0f), Math::Vec3(0, 0, 1), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, -1.0f, 1.0f), Math::Vec3(0, 0, 1), Math::Vec2(1, 0)},
            Vertex{Math::Vec3( 1.0f,  1.0f, 1.0f), Math::Vec3(0, 0, 1), Math::Vec2(1, 1)},
            Vertex{Math::Vec3(-1.0f,  1.0f, 1.0f), Math::Vec3(0, 0, 1), Math::Vec2(0, 1)},

            // Left face
            Vertex{Math::Vec3(-1.0f, -1.0f,  1.0f), Math::Vec3(-1, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3(-1.0f, -1.0f, -1.0f), Math::Vec3(-1, 0, 0), Math::Vec2(1, 0)},
            Vertex{Math::Vec3(-1.0f,  1.0f, -1.0f), Math::Vec3(-1, 0, 0), Math::Vec2(1, 1)},
            Vertex{Math::Vec3(-1.0f,  1.0f,  1.0f), Math::Vec3(-1, 0, 0), Math::Vec2(0, 1)},

            // Right face
            Vertex{Math::Vec3( 1.0f, -1.0f, -1.0f), Math::Vec3(1, 0, 0), Math::Vec2(0, 0)},
            Vertex{Math::Vec3( 1.0f, -1.0f,  1.0f), Math::Vec3(1, 0, 0), Math::Vec2(1, 0)},
            Vertex{Math::Vec3( 1.0f,  1.0f,  1.0f), Math::Vec3(1, 0, 0), Math::Vec2(1, 1)},
            Vertex{Math::Vec3( 1.0f,  1.0f, -1.0f), Math::Vec3(1, 0, 0), Math::Vec2(0, 1)},

            // Bottom face
            Vertex{Math::Vec3(-1.0f, -1.0f, -1.0f), Math::Vec3(0, -1, 0), Math::Vec2(0, 1)},
            Vertex{Math::Vec3( 1.0f, -1.0f, -1.0f), Math::Vec3(0, -1, 0), Math::Vec2(1, 1)},
            Vertex{Math::Vec3( 1.0f, -1.0f,  1.0f), Math::Vec3(0, -1, 0), Math::Vec2(1, 0)},
            Vertex{Math::Vec3(-1.0f, -1.0f,  1.0f), Math::Vec3(0, -1, 0), Math::Vec2(0, 0)},

            // Top face
            Vertex{Math::Vec3(-1.0f,  1.0f, -1.0f), Math::Vec3(0, 1, 0), Math::Vec2(0, 1)},
            Vertex{Math::Vec3( 1.0f,  1.0f, -1.0f), Math::Vec3(0, 1, 0), Math::Vec2(1, 1)},
            Vertex{Math::Vec3( 1.0f,  1.0f,  1.0f), Math::Vec3(0, 1, 0), Math::Vec2(1, 0)},
            Vertex{Math::Vec3(-1.0f,  1.0f,  1.0f), Math::Vec3(0, 1, 0), Math::Vec2(0, 0)},
        };

        DS::Vector<unsigned int> cube_indices = {
            0,  1,  2,  2,  3,  0,  // Front
            4,  5,  6,  6,  7,  4,  // Back
            8,  9,  10, 10, 11, 8,  // Left
            12, 13, 14, 14, 15, 12, // Right
            16, 17, 18, 18, 19, 16, // Bottom
            20, 21, 22, 22, 23, 20  // Top
        };

        Geometry ret;
        ret.draw_type = GL_TRIANGLES;
        ret.vertices = cube_vertices;
        ret.indices = cube_indices;
        ret.vertex_count = ret.vertices.count();
        ret.index_count = ret.indices.count();
        ret.setup();

        return ret;
    }

    Geometry Geometry::Sphere(int segments) {
        DS::Vector<Vertex> sphere_vertices;
        DS::Vector<unsigned int> sphere_indices;

        int rings = segments;
        int sectors = segments;
        float TWO_PI = 2.0f * PI;

        for (int r = 0; r <= rings; ++r) {
            float phi = PI * r / rings;
            float y = cosf(phi);
            float sin_phi = sinf(phi);

            for (int s = 0; s <= sectors; ++s) {
                float theta = TWO_PI * s / sectors;
                float x = sin_phi * cosf(theta);
                float z = sin_phi * sinf(theta);

                float u = static_cast<float>(s) / sectors;
                float v = static_cast<float>(r) / rings;

                sphere_vertices.push(Vertex{
                    Math::Vec3(x, y, z),
                    Math::Vec3(x, y, z), // Normal (unit direction)
                    Math::Vec2(u, v)     // UV
                });
            }
        }

        for (int r = 0; r < rings; ++r) {
            for (int s = 0; s < sectors; ++s) {
                int i0 = r * (sectors + 1) + s;
                int i1 = i0 + 1;
                int i2 = i0 + (sectors + 1);
                int i3 = i2 + 1;

                // Triangle 1
                sphere_indices.push(i0);
                sphere_indices.push(i2);
                sphere_indices.push(i1);

                // Triangle 2
                sphere_indices.push(i1);
                sphere_indices.push(i2);
                sphere_indices.push(i3);
            }
        }

        Geometry ret;
        ret.draw_type = GL_TRIANGLES;
        ret.vertices = sphere_vertices;
        ret.indices = sphere_indices;
        ret.vertex_count = ret.vertices.count();
        ret.index_count = ret.indices.count();
        ret.setup();

        return ret;
    }

    Geometry Geometry::Model(const char* path) {
        Geometry ret;
        ret.draw_type = GL_TRIANGLES;

        Assimp::Importer importer;
        unsigned int assimp_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
        const aiScene* scene = importer.ReadFile(path, assimp_flags);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERROR("ASSIMP ERROR: %s\n", importer.GetErrorString());
        }

        ret.loadMeshFromScene(path, String::Length(path), scene);

        return ret;
    }

    void Geometry::draw(const ShaderBase* shader) {
        shader->use();
        this->VAO.bind();

        /*
        struct Batch {
            DS::Vector<GLsizei> firsts;
            DS::Vector<GLsizei> counts;
            Material material; // If it has the same material index then you can multi-draw
        }
        */

        // Vector<Batch> batches // Calculate the number of batches

        /*
        // Define the draw calls
        GLsizei firsts[] = { 0, 36 }; // Start of Cube 1 (0), Start of Cube 2 (36 vertices)
        GLsizei counts[] = { 36, 36 }; // 36 vertices per cube
        GLsizei numDraws = 2;
        // glMultiDrawArrays(mode, firsts, counts, numDraws);
        */

        for (Geometry* geo = this; geo != nullptr; geo = geo->next) {
            if (geo->vertex_count == 0) {
                continue;
            }

            shader->setMaterial(geo->material);

            if (geo->index_count > 0) {
                Renderer::IncrementDrawCallCount();
                glCheckError(glDrawElementsBaseVertex(
                    this->draw_type, geo->index_count, GL_UNSIGNED_INT, 
                    (void*)(sizeof(unsigned int) * geo->base_index), 
                    geo->base_vertex
                ));
            } else {
                Renderer::IncrementDrawCallCount();
                glCheckError(glDrawArrays(
                    this->draw_type,
                    geo->base_vertex,
                    geo->vertex_count
                ));
            }
        }
    }

    void Geometry::drawInstanced(const ShaderBase* shader, int instance_count) {
        shader->use();
        this->VAO.bind();

        for (Geometry* geo = this; geo != nullptr; geo = geo->next) {
            if (geo->vertex_count == 0) {
                continue;
            }

            shader->setMaterial(geo->material);

            if (geo->index_count > 0) {
                Renderer::IncrementDrawCallCount();
                glCheckError(glDrawElementsInstancedBaseVertex(
                    this->draw_type, geo->index_count,
                    GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * geo->base_index),
                    instance_count, geo->base_vertex
                ));
            } else {
                Renderer::IncrementDrawCallCount();
                glCheckError(glDrawArraysInstanced(
                    this->draw_type,
                    geo->base_vertex,
                    geo->vertex_count,
                    instance_count
                ));
            }
        }
    }

    void Geometry::setup(bool should_destory_data) {
        this->aabb = CalculateAABB(vertices);
        this->total_vertex_count = this->vertices.count();

        VertexAttributeFlag flags = VertexAttributeFlag::INVALID;
        flags = (vertices[0].aPosition != Math::Vec3(MAGIC_NUMBER)) ? flags | VertexAttributeFlag::aPosition : flags;
        flags = (vertices[0].aNormal != Math::Vec3(MAGIC_NUMBER)) ? flags | VertexAttributeFlag::aNormal : flags;
        flags = (vertices[0].aTexCoord != Math::Vec2(MAGIC_NUMBER)) ? flags | VertexAttributeFlag::aTexCoord : flags;
        flags = (vertices[0].aBitangent != Math::Vec3(MAGIC_NUMBER)) ? flags | VertexAttributeFlag::aBitangent : flags;
        flags = (vertices[0].aColor != Math::Vec3(MAGIC_NUMBER)) ? flags | VertexAttributeFlag::aColor : flags;
        flags = (vertices[0].aBoneIDs != Math::IVec4(MAGIC_NUMBER)) ? flags | VertexAttributeFlag::aBoneIDs : flags;
        flags = (vertices[0].aBoneWeights != Math::Vec4(MAGIC_NUMBER)) ? flags | VertexAttributeFlag::aBoneWeights : flags;
        RUNTIME_ASSERT(flags != VertexAttributeFlag::INVALID);

        DS::Vector<Renderer::VertexAttributeDescriptor> stride_type_info = {
            {BufferStrideTypeInfo::VEC3, OFFSET_OF(Vertex, aPosition)},
            {BufferStrideTypeInfo::VEC3, OFFSET_OF(Vertex, aNormal)},
            {BufferStrideTypeInfo::VEC2, OFFSET_OF(Vertex, aTexCoord)},
            {BufferStrideTypeInfo::VEC3, OFFSET_OF(Vertex, aBitangent)},
            {BufferStrideTypeInfo::VEC3, OFFSET_OF(Vertex, aColor)},
            {BufferStrideTypeInfo::IVEC4, OFFSET_OF(Vertex, aBoneIDs)},
            {BufferStrideTypeInfo::VEC3, OFFSET_OF(Vertex, aBoneWeights)},
        };

        //int stride = sizeof(Vertex);
        //int vertex_data_size = this->vertices.count() * sizeof(Vertex);

        this->VAO = VertexArray::Create();
        this->VAO.bind();

        this->VBO = GPUBuffer::VBO(BufferType::VERTEX, BufferUsage::STATIC, sizeof(Vertex), stride_type_info, this->vertices.count() * sizeof(Vertex), this->vertices.data());
        this->VBO.bind();

        this->EBO = GPUBuffer::EBO(this->indices.count(), this->indices.data());
        this->EBO.bind();

        // TODO(Jovanni): This is odd tbh, seems like VAO, VBO, UBO are enough differnt to warrent their own thing
        this->VAO.bindBuffer(0, false, this->VBO);

        for (Geometry* geo = this; geo != nullptr; geo = geo->next) {
            if (geo->vertex_count == 0) {
                continue;
            }

            geo->material.has_normals = flags & VertexAttributeFlag::aNormal;
            geo->material.has_texcoord = flags & VertexAttributeFlag::aTexCoord;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        if (should_destory_data) {
            this->vertices.~Vector();
            this->indices.~Vector();
            this->materials.~Vector();
        }
    }

    void Geometry::loadMeshFromScene(const char *path, u64 path_length, const aiScene* scene) {
        Geometry* current = this;
        unsigned int total_vertex_count = 0;
        unsigned int total_index_count = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            total_vertex_count += scene->mMeshes[i]->mNumVertices;
            total_index_count  += scene->mMeshes[i]->mNumFaces * 3;

            if (i > 0) {
                current->next = (Geometry*)Memory::Malloc(sizeof(Geometry));
                current = current->next;
            }
        }

        this->vertices = DS::Vector<Vertex>(total_vertex_count);
        this->indices = DS::Vector<GLuint>(total_index_count);
        this->materials = DS::Vector<Material>(scene->mNumMaterials);
        this->materials.resize(scene->mNumMaterials);

        s64 index = String::LastIndexOf(path, path_length, STRING_LIT_ARG("/"));
        RUNTIME_ASSERT(index > -1);

        const int STRING_CAPACITY = 512;
        char directory[STRING_CAPACITY] = {0};
        String::Copy(directory, STRING_CAPACITY, path, index);
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            const aiMaterial* ai_material = scene->mMaterials[i];
            this->materials[i].index = i;

            aiColor4D ambient_color(0.0f, 0.0f, 0.0f, 0.0f);
            if (ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color) == AI_SUCCESS) {
                this->materials[i].ambient_color.r = ambient_color.r;
                this->materials[i].ambient_color.g = ambient_color.g;
                this->materials[i].ambient_color.b = ambient_color.b;
            }

            aiColor3D diffuse_color(0.0f, 0.0f, 0.0f);
            if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS) {
                this->materials[i].diffuse_color.r = diffuse_color.r;
                this->materials[i].diffuse_color.g = diffuse_color.g;
                this->materials[i].diffuse_color.b = diffuse_color.b;
            }

            aiColor3D specular_color(0.0f, 0.0f, 0.0f);
            if (ai_material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color) == AI_SUCCESS) {
                this->materials[i].specular_color.r = specular_color.r;
                this->materials[i].specular_color.g = specular_color.g;
                this->materials[i].specular_color.b = specular_color.b;
            }

            float opacity = 1.0f;
            if (ai_material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
                this->materials[i].opacity = opacity;
            } else {
                // LOG_WARN("Mesh Failed opacity matkey?\n");
            }

            for (int type_int = 0; type_int < TEXTURE_COUNT; type_int++) {
                aiTextureType type = static_cast<aiTextureType>(type_int);
                if (ai_material->GetTextureCount(type) <= 0) {
                    continue;
                }

                aiString str;
                if (ai_material->GetTexture(type, 0, &str, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                    const char* texture_path = str.C_Str();
                    u64 texture_path_length = String::Length(texture_path);

                    u64 filename_length = 0;
                    u64 filename_capacity = index + 1 + texture_path_length + 1;
                    RUNTIME_ASSERT(filename_capacity < STRING_CAPACITY);
                    char filename[STRING_CAPACITY] = {0};
                    String::Append(filename, filename_length, STRING_CAPACITY, directory, index);
                    String::Append(filename, filename_length, STRING_CAPACITY, '/');
                    String::Append(filename, filename_length, STRING_CAPACITY, texture_path, texture_path_length);

                    const aiTexture* ai_texture = scene->GetEmbeddedTexture(str.C_Str());
                    if (ai_texture) {
                        int width, height, nrChannel = 0;
                        u8* image_data = stbi_load_from_memory((u8*)ai_texture->pcData, ai_texture->mWidth, &width, &height, &nrChannel, 0);
                        Texture texture = Texture::LoadFromMemory(image_data, width, height, nrChannel);

                        LOG_DEBUG("Material: %d | has embedded Texture of type: %s\n", i, texture_to_string[type]);
                        this->materials[i].textures[type] = texture;
                    } else {
                        LOG_DEBUG("Material: %d | has external texture of type: %s\n", i, texture_to_string[type]);
                        this->materials[i].textures[type] = Texture::LoadFromFile(filename);
                    }
                } else {
                    LOG_ERROR("Failed to get texture path for material: %d | type: %s\n", i, texture_to_string[type]);
                }
            }
        }

        this->processNode(this, scene->mRootNode, scene, convertAssimpMatrixToGM(scene->mRootNode->mTransformation));
        this->setup();
    }

    Geometry* Geometry::processNode(Geometry* root, aiNode* node, const aiScene* scene, Math::Mat4 parent_transform) {
        Geometry* current = root;
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            RUNTIME_ASSERT_MSG(mesh->mPrimitiveTypes != 0, "Primitive type not set by ASSIMP in mesh.\n");
            if ((mesh->mPrimitiveTypes & (mesh->mPrimitiveTypes-1)) != 0) {
                LOG_ERROR(
                    "This mesh has more than one primitive"
                    "type in it. The model should be loaded with the "
                    "aiProcess_SortByPType flag set.\n"
                );

                RUNTIME_ASSERT(false);
            }

            /* We assume that each mesh has its own primitive type. Here
            * we identify that type by number and by the OpenGL name.. */
            unsigned int meshPrimitiveType;
            if (mesh->mPrimitiveTypes & aiPrimitiveType_POINT) {
                meshPrimitiveType = 1;
                current->draw_type = GL_POINTS;
            } else if (mesh->mPrimitiveTypes & aiPrimitiveType_LINE) {
                meshPrimitiveType = 2;
                current->draw_type = GL_LINES;
            } else if (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) {
                meshPrimitiveType = 3;
                current->draw_type = GL_TRIANGLES;
            } else if (mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON) {
                LOG_ERROR(
                    "Mesh %u (%u/%u meshes in node \"%s\"): We only "
                    "support drawing triangle, line, or point meshes. "
                    "This mesh contained polygons, and we are skipping it. "
                    "To resolve this problem, ensure that the file is loaded "
                    "with aiProcess_Triangulate to force ASSIMP to triangulate "
                    "the model.\n",
                    node->mMeshes[i], i+1, node->mNumMeshes, node->mName.data
                );

                RUNTIME_ASSERT(false);
            } else {
                RUNTIME_ASSERT_MSG(false, "Unknown primitive type in mesh.\n");
            }

            processAssimpMesh(current, mesh, scene, parent_transform);
            current = current->next;
        }

        Math::Mat4 new_parent_transform = parent_transform * convertAssimpMatrixToGM(node->mTransformation);
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            current = processNode(current, node->mChildren[i], scene, new_parent_transform);
        }

        return current;
    }

    void Geometry::processAssimpMesh(Geometry* root, aiMesh* mesh, const aiScene* scene, Math::Mat4 parent_transform) {
        root->base_vertex = (unsigned int)this->vertices.count();
        root->base_index = (unsigned int)this->indices.count();
        root->material = this->materials[mesh->mMaterialIndex];
        root->index_count = mesh->mNumFaces * 3;
        root->vertex_count = mesh->mNumVertices;
        
        Vertex v = Vertex();
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            const aiVector3D& ai_position = mesh->mVertices[j];

            Math::Vec4 transformed_position = parent_transform * Math::Vec4(ai_position.x, ai_position.y, ai_position.z, 1.0f);
            v.aPosition = Math::Vec3(transformed_position.x, transformed_position.y, transformed_position.z);

            if (mesh->mNormals) {
                const aiVector3D& pNormal = mesh->mNormals[j];
                Math::Vec4 transformed_normal = parent_transform * Math::Vec4(pNormal.x, pNormal.y, pNormal.z, 0.0f); // W component is 0 for vectors
                v.aNormal = Math::Vec3(transformed_normal.x, transformed_normal.y, transformed_normal.z).normalize(); // Normalize after transform
            } else {
                LOG_ERROR("Mesh doesn't have normals?\n");
            }

            if (mesh->mTangents) {
                const aiVector3D& pTangents = mesh->mTangents[j];
                Math::Vec4 transformed_tangents = parent_transform * Math::Vec4(pTangents.x, pTangents.y, pTangents.z, 0.0f);
                v.aTangent = Math::Vec3(transformed_tangents);
            }

            if (mesh->mBitangents) {
                const aiVector3D& pBitangents = mesh->mBitangents[j];
                Math::Vec4 transformed_bitangents = parent_transform * Math::Vec4(pBitangents.x, pBitangents.y, pBitangents.z, 0.0f);
                v.aBitangent = Math::Vec3(transformed_bitangents);
            }

            if (mesh->mColors[0]) {
                v.aColor = Math::Vec3(mesh->mColors[0]->r, mesh->mColors[0]->g, mesh->mColors[0]->b);
            } else { 
                aiColor4D diffuse;
                const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
                    v.aColor = Math::Vec3(diffuse.r, diffuse.g, diffuse.b);
                }
            }

            const aiVector3D& uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j] : Zero3D;
            v.aTexCoord = Math::Vec2(uv.x, uv.y);

            #if 0
                /* Fill in bone information */
                if (mesh->mBones && mesh->mNumBones > 0)
                {
                    RUNTIME_ASSERT_MSG(mesh->mNumBones <= MAX_BONES, 
                        "This mesh has %d bones but we only support %d",
                        mesh->mNumBones, MAX_BONES
                    );

                    float *indices = kuhl_malloc(sizeof(float)*mesh->mNumVertices*4);
                    float *weights = kuhl_malloc(sizeof(float)*mesh->mNumVertices*4);
                    /* For each vertex */
                    for (unsigned int i=0; i<mesh->mNumVertices; i++)
                    {
                        /* Zero out weights */
                        for (int j=0; j<4; j++)
                        {
                            // If weight is zero, it doesn't matter what the index
                            // is as long as it isn't out of bounds.
                            indices[i*4+j] = 0;
                            weights[i*4+j] = 0;
                        }

                        int count = 0; /* How many bones refer to this vertex? */
                            
                        /* For each bone */
                        for (unsigned int j=0; j<mesh->mNumBones; j++)
                        {
                            /* Each vertex that this bone refers to. */
                            for (unsigned int k=0; k<mesh->mBones[j]->mNumWeights; k++)
                            {
                                /* If this bone refers to a vertex that matches the one
                                that we are on, use the data and send it to the vertex program.
                                */
                                unsigned int idx = mesh->mBones[j]->mWeights[k].mVertexId;
                                float wght       = mesh->mBones[j]->mWeights[k].mWeight;
                                if (idx == i)
                                {
                                    indices[i*4+count] = (float) j;
                                    weights[i*4+count] = wght;
                                    count++;
                                } // end if vertices match
                            } // end for each vertex the bone refers to
                        } // end for each bone
                    } // end for each vertex in mesh

                    for (unsigned int i=0; i<mesh->mNumVertices; i++) {
                        if (weights[i*4+0] == 0) {
                            msg(MSG_FATAL, "Every vertex should have at least one weight but vertex %ud has no weights!", i);
                            exit(EXIT_FAILURE);
                        }
                    }
                    kuhl_geometry_attrib(geom, indices, 4, "in_BoneIndex", 0);
                    kuhl_geometry_attrib(geom, weights, 4, "in_BoneWeight", 0);
                    free(indices);
                    free(weights);
                } // end if there are bones 
            #endif

            this->vertices.push(v);
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            const aiFace& Face = mesh->mFaces[j];
            if (Face.mNumIndices == 3) {
                this->indices.push(Face.mIndices[0]);
                this->indices.push(Face.mIndices[1]);
                this->indices.push(Face.mIndices[2]);
            } else {
                LOG_ERROR("Mesh '%s' has non-triangular face with %d indices. Skipping.\n", mesh->mName.C_Str(), Face.mNumIndices);
            }
        }
    }

    void Geometry::loadMeshFromData(const DS::Vector<Vertex> &vertices, const DS::Vector<unsigned int> &indices) {
        this->vertex_count = (unsigned int)vertices.count();
        this->index_count  = (unsigned int)indices.count();
        this->base_vertex  = 0;
        this->base_index   = 0;

        this->vertices = vertices;
        this->indices = indices;

        this->setup(false);
    }
}

#if 0
    /** Recursively calls itself to create one or more kuhl_geometry
     * structs for all of the nodes in the scene.
     *
     * @param sc The scene that we want to render.
     *
     * @param nd The current node that we are rendering.
     */
    static kuhl_geometry* kuhl_private_load_model(const struct aiScene *sc,
                                                const struct aiNode* nd,
                                                GLuint program,
                                                float currentTransform[16],
                                                const char* modelFilename,
                                                const char* textureDirname)
    {
        /* Each node in the scene has a transform matrix that should
        * affect all of the nodes under it. The currentTransform matrix
        * is the current matrix based on any nodes above the one that we
        * are currently processing. Here, we update the currentTransform
        * to include the matrix in the node we are currently on. */

        /* Save our current transform so we can reapply it when we are
        * finished processing this node */
        float origTransform[16];
        mat4f_copy(origTransform, currentTransform);
        
        /* Get this node's transform matrix and convert it into a plain array. */
        float thisTransform[16];
        mat4f_from_aiMat4x4(thisTransform, nd->mTransformation);

        /* Apply this node's transformation to our current transform. */
        mat4f_mult_mat4f_new(currentTransform, currentTransform, thisTransform);

        kuhl_geometry *first_geom = NULL;
        
        /* Create a kuhl_geometry object for each of the meshes assigned
        * to this ASSIMP node. */
        for (unsigned int n=0; n < nd->mNumMeshes; n++)
        {
            const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

            /* Confirm that the mesh has only one primitive type. */
            if (mesh->mPrimitiveTypes == 0)
            {
                msg(MSG_ERROR, "Primitive type not set by ASSIMP in mesh.\n");
                continue;
            }
            // Check if more than one bit (i.e., primitive type) is in this mesh.
            if ((mesh->mPrimitiveTypes & (mesh->mPrimitiveTypes-1)) != 0) 
            {
                msg(MSG_ERROR, "This mesh has more than one primitive "
                    "type in it. The model should be loaded with the "
                    "aiProcess_SortByPType flag set.\n");
                continue;
            }

            /* We assume that each mesh has its own primitive type. Here
            * we identify that type by number and by the OpenGL name.. */
            unsigned int meshPrimitiveType;
            int meshPrimitiveTypeGL;
            if (mesh->mPrimitiveTypes & aiPrimitiveType_POINT)
            {
                meshPrimitiveType = 1;
                meshPrimitiveTypeGL = GL_POINTS;
            }
            else if (mesh->mPrimitiveTypes & aiPrimitiveType_LINE)
            {
                meshPrimitiveType = 2;
                meshPrimitiveTypeGL = GL_LINES;
            }
            else if (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)
            {
                meshPrimitiveType = 3;
                meshPrimitiveTypeGL = GL_TRIANGLES;
            }
            else if (mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON)
            {
                msg(MSG_WARNING, "Mesh %u (%u/%u meshes in node \"%s\"): We only "
                    "support drawing triangle, line, or point meshes. "
                    "This mesh contained polygons, and we are skipping it. "
                    "To resolve this problem, ensure that the file is loaded "
                    "with aiProcess_Triangulate to force ASSIMP to triangulate "
                    "the model.\n",
                    nd->mMeshes[n], n+1, nd->mNumMeshes, nd->mName.data);
                continue;
            }
            else
            {
                msg(MSG_ERROR, "Unknown primitive type in mesh.\n");
                continue;
            }
            
            /* Allocate space and initialize kuhl_geometry. One kuhl_geometry
            * will be used per mesh. We
            * allocate each one individually (instead of malloc()'ing one
            * large space for all of the meshes in this node so each of
            * the objects can be free()'d) */
            kuhl_geometry *geom = (kuhl_geometry*) kuhl_malloc(sizeof(kuhl_geometry));
            kuhl_geometry_new(geom, program, mesh->mNumVertices,
                            meshPrimitiveTypeGL);

            /* Set up kuhl_geometry linked list */
            first_geom = kuhl_geometry_append(first_geom, geom);

            geom->assimp_node = (struct aiNode*) nd;
            geom->assimp_scene = (struct aiScene*) sc;
            mat4f_copy(geom->matrix, currentTransform);

            /* Store the vertex position attribute into the kuhl_geometry struct */
            float *vertexPositions = kuhl_malloc(sizeof(float)*mesh->mNumVertices*3);
            for (unsigned int i=0; i<mesh->mNumVertices; i++)
            {
                vertexPositions[i*3+0] = (mesh->mVertices)[i].x;
                vertexPositions[i*3+1] = (mesh->mVertices)[i].y;
                vertexPositions[i*3+2] = (mesh->mVertices)[i].z;
            }
            kuhl_geometry_attrib(geom, vertexPositions, 3, "in_Position", 0);
            free(vertexPositions);

            /* Store the normal vectors in the kuhl_geometry struct */
            if (mesh->mNormals != NULL)
            {
                float *normals = kuhl_malloc(sizeof(float)*mesh->mNumVertices*3);
                for (unsigned int i=0; i<mesh->mNumVertices; i++)
                {
                    normals[i*3+0] = (mesh->mNormals)[i].x;
                    normals[i*3+1] = (mesh->mNormals)[i].y;
                    normals[i*3+2] = (mesh->mNormals)[i].z;
                }
                kuhl_geometry_attrib(geom, normals, 3, "in_Normal", 0);
                free(normals);
            }
            if (mesh->mTangents)
            {
                float *tangents = kuhl_malloc(sizeof(float)*mesh->mNumVertices*3);
                for (unsigned int i=0; i<mesh->mNumVertices; i++)
                {
                    tangents[i*3+0] = (mesh->mTangents)[i].x;
                    tangents[i*3+1] = (mesh->mTangents)[i].y;
                    tangents[i*3+2] = (mesh->mTangents)[i].z;
                }
                kuhl_geometry_attrib(geom, tangents, 3, "in_Tangent", 0);
                free(tangents);
            }
            if (mesh->mBitangents)
            {
                float *bitangents = kuhl_malloc(sizeof(float)*mesh->mNumVertices*3);
                for (unsigned int i=0; i<mesh->mNumVertices; i++)
                {
                    bitangents[i*3+0] = (mesh->mBitangents)[i].x;
                    bitangents[i*3+1] = (mesh->mBitangents)[i].y;
                    bitangents[i*3+2] = (mesh->mBitangents)[i].z;
                }
                kuhl_geometry_attrib(geom, bitangents, 3, "in_Bitangent", 0);
                free(bitangents);
            }


            /* Store the vertex color attribute */
            // Note: mesh->mColors is a C array, not a pointer
            if (mesh->mColors[0] != NULL)
            {
                /* Don't use alpha by default; changing this to 4 may
                require the size of in_Color the vertex program to be
                adjusted. */
                static const int colorComps = 3; 
                float *colors = kuhl_malloc(sizeof(float)*mesh->mNumVertices*colorComps);
                for (unsigned int i=0; i<mesh->mNumVertices; i++)
                {
                    colors[i*colorComps+0] = mesh->mColors[0][i].r;
                    colors[i*colorComps+1] = mesh->mColors[0][i].g;
                    colors[i*colorComps+2] = mesh->mColors[0][i].b;
                    if (colorComps == 4)
                        colors[i*colorComps+3] = mesh->mColors[0][i].a;
                }
                kuhl_geometry_attrib(geom, colors, colorComps, "in_Color", 0);
                free(colors);
            }
            /* If there are no vertex colors, try to use material colors instead */
            else
            {
                /* It would be more efficient to send material colors as a
                * uniform variable. However, by using this approach, we
                * don't need to use both a material color uniform and a
                * vertex color attribute in a GLSL program that displays
                * a model. */
                const struct aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];
                struct aiColor4D diffuse;
                if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
                {
                    static const int colorComps = 3;
                    float *colors = kuhl_malloc(sizeof(float)*mesh->mNumVertices*colorComps);
                    for (unsigned int i=0; i<mesh->mNumVertices; i++)
                    {
                        colors[i*colorComps+0] = diffuse.r;
                        colors[i*colorComps+1] = diffuse.g;
                        colors[i*colorComps+2] = diffuse.b;
                        // Alpha is not handled for now.
                    }
                    kuhl_geometry_attrib(geom, colors, colorComps, "in_Color", 0);
                    free(colors);
                }
            }
            
            /* Store the texture coordinate attribute */
            // Note: mesh->mTextureCoords is a C array, not a pointer
            if (mesh->mTextureCoords[0] != NULL)
            {
                float *texCoord = kuhl_malloc(sizeof(float)*mesh->mNumVertices*2);
                for (unsigned int i=0; i<mesh->mNumVertices; i++)
                {
                    texCoord[i*2+0] = mesh->mTextureCoords[0][i].x;
                    texCoord[i*2+1] = mesh->mTextureCoords[0][i].y;
                }
                kuhl_geometry_attrib(geom, texCoord, 2, "in_TexCoord", 0);
                free(texCoord);
            }

            /* Fill in bone information */
            if (mesh->mBones != NULL && mesh->mNumBones > 0)
            {
                if (mesh->mNumBones > MAX_BONES)
                {
                    msg(MSG_FATAL, "This mesh has %d bones but we only support %d",
                        mesh->mNumBones, MAX_BONES);
                    exit(EXIT_FAILURE);
                }
                
                float *indices = kuhl_malloc(sizeof(float)*mesh->mNumVertices*4);
                float *weights = kuhl_malloc(sizeof(float)*mesh->mNumVertices*4);
                /* For each vertex */
                for (unsigned int i=0; i<mesh->mNumVertices; i++)
                {
                    /* Zero out weights */
                    for (int j=0; j<4; j++)
                    {
                        // If weight is zero, it doesn't matter what the index
                        // is as long as it isn't out of bounds.
                        indices[i*4+j] = 0;
                        weights[i*4+j] = 0;
                    }

                    int count = 0; /* How many bones refer to this vertex? */
                        
                    /* For each bone */
                    for (unsigned int j=0; j<mesh->mNumBones; j++)
                    {
                        /* Each vertex that this bone refers to. */
                        for (unsigned int k=0; k<mesh->mBones[j]->mNumWeights; k++)
                        {
                            /* If this bone refers to a vertex that matches the one
                            that we are on, use the data and send it to the vertex program.
                            */
                            unsigned int idx = mesh->mBones[j]->mWeights[k].mVertexId;
                            float wght       = mesh->mBones[j]->mWeights[k].mWeight;
                            if (idx == i)
                            {
                                indices[i*4+count] = (float) j;
                                weights[i*4+count] = wght;
                                count++;
                            } // end if vertices match
                        } // end for each vertex the bone refers to
                    } // end for each bone
                } // end for each vertex in mesh

                for (unsigned int i=0; i<mesh->mNumVertices; i++)
                {
                    if (weights[i*4+0] == 0)
                    {
                        msg(MSG_FATAL, "Every vertex should have at least one weight but vertex %ud has no weights!", i);
                        exit(EXIT_FAILURE);
                    }
                }
                kuhl_geometry_attrib(geom, indices, 4, "in_BoneIndex", 0);
                kuhl_geometry_attrib(geom, weights, 4, "in_BoneWeight", 0);
                free(indices);
                free(weights);
            } // end if there are bones 


            /* Go through all texture types that ASSIMP supports. */
            for (int tt=0; tt<TEX_TYPE_LEN; tt++)
            {
                /* Find our texture and tell our kuhl_geometry object about
                * it. */
                struct aiString texPath;	//contains filename of texture
                int texIndex = 0;
                if (AI_SUCCESS == aiGetMaterialTexture(sc->mMaterials[mesh->mMaterialIndex],
                                                    texTypeList[tt], texIndex, &texPath,
                                                    NULL, NULL, NULL, NULL, NULL, NULL))
                {
                    GLuint texture = 0;
                    for (int i=0; i<textureIdMapSize; i++)
                    {
                        char *fullpath = kuhl_private_assimp_fullpath(texPath.data, modelFilename, textureDirname);
                        if (strcmp(textureIdMap[i].textureFileName, fullpath) == 0)
                            texture = textureIdMap[i].textureID;
                        free(fullpath);
                    }
                    if (texture == 0)
                    {
                        msg(MSG_WARNING, "Mesh %u uses %s texture '%s'."
                            "This texture should have been loaded earlier, but we can't find it now.",
                            nd->mMeshes[n], texTypeListStr[tt], texPath.data);
                    }
                    else
                    {
                        /* If model uses texture and we found the texture file,
                        Make sure we repeat instead of clamp textures */
                        glBindTexture(GL_TEXTURE_2D, texture);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        kuhl_errorcheck();

                        if (texTypeList[tt] == aiTextureType_DIFFUSE)
                            // use "tex" variable name for diffuse textures.
                            kuhl_geometry_texture(geom, texture, "tex", 0);
                        else
                        {
                            // use variable names like tex_SPECULAR, tex_NORMALS, etc.
                            char glsl_var_name[100]="";
                            snprintf(glsl_var_name, 100, "tex_%s", texTypeListStr[tt]);
                            kuhl_geometry_texture(geom, texture, glsl_var_name, 0);
                        }
                    }
                } // end if assimp provides this texture 
            } // end loop through all of assimp supported texture types.

            if (mesh->mNumFaces > 0)
            {
                /* Get indices to draw with */
                GLuint numIndices = mesh->mNumFaces * meshPrimitiveType;
                GLuint *indices = kuhl_malloc(sizeof(GLuint)*numIndices);
                for (unsigned int t = 0; t<mesh->mNumFaces; t++) // for each face
                {
                    const struct aiFace* face = &mesh->mFaces[t];
                    for (unsigned int x = 0; x < meshPrimitiveType; x++) // for each index
                        indices[t*meshPrimitiveType+x] = face->mIndices[x];
                }
                kuhl_geometry_indices(geom, indices, numIndices);
                free(indices);
            }


            /* Initialize list of bone matrices if this mesh has bones. */
            if (mesh->mNumBones > 0)
            {
                kuhl_bonemat *bones = (kuhl_bonemat*) kuhl_malloc(sizeof(kuhl_bonemat));
                bones->count = mesh->mNumBones;
                bones->mesh = n;
                for (unsigned int b=0; b < mesh->mNumBones; b++)
                    bones->boneList[b] = mesh->mBones[b];
                // set any unused bone matrices to the identity.
                for (unsigned int b=mesh->mNumBones; b < MAX_BONES; b++)
                    mat4f_identity(bones->matrices[b]);
                geom->bones = bones;
            }

            msg(MSG_DEBUG, "Mesh #%03u in node \"%s\" (node has %d meshes): verts=%d indices=%d primType=%d normals=%s colors=%s texCoords=%s bones=%d texture_count=%d",
                nd->mMeshes[n], nd->mName.data, nd->mNumMeshes,
                mesh->mNumVertices,
                mesh->mNumFaces*meshPrimitiveType,
                meshPrimitiveType,
                mesh->mNormals          == NULL ? "n" : "y",
                mesh->mColors[0]        == NULL ? "n" : "y", // mColors is an array of pointers
                mesh->mTextureCoords[0] == NULL ? "n" : "y",   // mTextureCoords is an array of pointers
                mesh->mNumBones,
                geom->texture_count);
            // TODO: We could print the filenames of all the textures that this mesh uses?
        } // end for each mesh in node

        /* Process all of the meshes in the aiNode's children too */
        for (unsigned int i = 0; i < nd->mNumChildren; i++)
        {
            kuhl_geometry *child_geom = kuhl_private_load_model(sc, nd->mChildren[i], program, currentTransform, modelFilename, textureDirname);
            first_geom = kuhl_geometry_append(first_geom, child_geom);
        }

        /* Restore the transform matrix to exactly as it was when this
        * function was called by the caller. */
        mat4f_copy(currentTransform, origTransform);

        return first_geom;
    }


    /** Setup a model to draw at a specific time.

        @param modelFilename Name of model file to update.

        @param animationNum The animation to use. If the file only
        contains one animation, set it to 0.

        @param time The time in seconds to set the animation to. Setting
        time to a negative displays the model in its bind pose.
    */
    void kuhl_update_model(kuhl_geometry *first_geom, unsigned int animationNum, float time)
    {
        for (kuhl_geometry *g = first_geom; g != NULL; g=g->next)
        {
            /* The aiScene object that this kuhl_geometry refers to. */
            struct aiScene *scene = g->assimp_scene;
            /* The aiNode object that this kuhl_geometry refers to. */
            struct aiNode *node = g->assimp_node;

            /* If the geometry contains no animations, isn't associated
            * with an ASSIMP scene or node, then there is no need to try
            * to animate it. */
            if (scene == NULL || scene->mNumAnimations == 0 || node == NULL)
                continue;

            /* If there are no bones, or if a negative time value was
            * provided, update g->matrix. If there are bones, we assume
            * that the bones will drive the animation. */
            if (g->bones == NULL )
            {
                /* Start at our current node and traverse up. Apply all of the
                * transformation matrices as we traverse up.
                *
                * TODO: By repeatedly traversing up, we repeatedly
                * recalculate the transformation matrices for the nodes
                * near the root---potentially reducing performance.
                */
                mat4f_identity(g->matrix);
                do
                {
                    float transform[16];
                    kuhl_private_node_matrix(transform, scene, node, animationNum, time);
                    mat4f_mult_mat4f_new(g->matrix, transform, g->matrix);
                    node = node->mParent;
                } while (node != NULL);

                // We may have overwritten a "fit" part of the matrix
                // intended to place the model on/near the origin in a
                // standard sized box. Re-apply the "fit" matrix here.
                mat4f_mult_mat4f_new(g->matrix, g->fitMatrix, g->matrix);
            }

            /* Don't process bones if there aren't any. */
            if (g->bones == NULL)
                continue;

            /* Update the list of bone matrices. */
            for (int b=0; b < g->bones->count; b++) // For each bone
            {
                // Find the bone node and the bone itself.
                const struct aiNode *node = kuhl_assimp_find_node(g->bones->boneList[b]->mName.data, scene->mRootNode);
                if (node == NULL)
                {
                    msg(MSG_FATAL, "Failed to find node that corresponded to bone: %s\n", g->bones->boneList[b]->mName.data);
                    exit(EXIT_FAILURE);
                }
                const struct aiBone *bone = g->bones->boneList[b];


                /* Start at our current node and traverse up. Apply all of the
                * transformation matrices as we traverse up.
                *
                * TODO: By repeatedly traversing up, we repeatedly
                * recalculate the transformation matrices for the nodes
                * near the root---potentially reducing performance.
                */
                mat4f_identity(g->bones->matrices[b]);
                do
                {
                    float transform[16];
                    kuhl_private_node_matrix(transform, scene, node, animationNum, time);
                    mat4f_mult_mat4f_new(g->bones->matrices[b], transform, g->bones->matrices[b]);
                    node = node->mParent; // move to next node up
                } while (node != NULL);

                /* Also apply the bone offset */
                float offset[16];
                mat4f_from_aiMat4x4(offset, bone->mOffsetMatrix);
                mat4f_mult_mat4f_new(g->bones->matrices[b], g->bones->matrices[b], offset);

                // If a "fit" matrix was used to make the model fit in box
                // on or centered at the origin, use that matrix too.
                mat4f_mult_mat4f_new(g->bones->matrices[b], g->fitMatrix, g->bones->matrices[b]);

            } // end for each bone
        } // end for each geometry
    }

    /** Loads a model without drawing it.
     *
     * @param modelFilename The filename of the model.
     *
     * @param textureDirname The directory that the model's textures are
     * saved in. If set to NULL, the textures are assumed to be in the
     * same directory as the model is in. If the model has already been
     * drawn/loaded, this parameter is unused.
     *
     * @param program The GLSL program to draw the model with.
     *
     * @param bbox To be filled in with the bounding box of the model
     * (xmin, xmax, ymin, etc). The bounding box may be incorrect if the
     * model includes animation.
     *
     * @return Returns a kuhl_geometry object that can be later drawn. If
     * the model contains multiple meshes, kuhl_geometry will be a linked
     * list (i.e., geom->next will not be NULL). Calls exit() on error.
     */
    kuhl_geometry* kuhl_load_model(const char *modelFilename, const char *textureDirname,
                                GLuint program, float bbox[6])
    {
        char *newModelFilename = kuhl_find_file(modelFilename);
        // Loads the model from the file and reads in all of the textures:
        const struct aiScene *scene = kuhl_private_assimp_load(newModelFilename, textureDirname);
        if (scene == NULL)
        {
            msg(MSG_ERROR, "ASSIMP was unable to import the model '%s'.\n", modelFilename);
            //return NULL;
            exit(EXIT_FAILURE);
        }

        // Convert the information in aiScene into a kuhl_geometry object.
        float transform[16];
        mat4f_identity(transform);
        kuhl_geometry *ret = kuhl_private_load_model(scene, scene->mRootNode,
                                                    program, transform,
                                                    newModelFilename, textureDirname);

        /* Ensure model shows up in bind pose if the caller doesn't
        * also call kuhl_update_model(). */
        kuhl_update_model(ret, 0, -1);

        /* Calculate bounding box information for the model */
        float bboxLocal[6];
        kuhl_private_calc_bbox(scene->mRootNode, NULL, scene, bboxLocal);
        float min[3],max[3],ctr[3];
        vec3f_set(min, bboxLocal[0], bboxLocal[2], bboxLocal[4]);
        vec3f_set(max, bboxLocal[1], bboxLocal[3], bboxLocal[5]);
        vec3f_add_new(ctr, min, max);
        vec3f_scalarDiv(ctr, 2);

        /* Print bounding box information to stout */
        msg(MSG_DEBUG, "%s: bbox min: %10.3f %10.3f %10.3f", modelFilename, min[0], min[1], min[2]);
        msg(MSG_DEBUG, "%s: bbox max: %10.3f %10.3f %10.3f", modelFilename, max[0], max[1], max[2]);
        msg(MSG_DEBUG, "%s: bbox ctr: %10.3f %10.3f %10.3f", modelFilename, ctr[0], ctr[1], ctr[2]);

        /* If the user requested bounding box information, give it to
        * them. */
        if (bbox != NULL)
        {
            for (int i=0; i<6; i++)
                bbox[i] = bboxLocal[i];
        }
        return ret;
    }
#endif
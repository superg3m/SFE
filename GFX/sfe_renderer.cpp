#include <glad/glad.h>

#include "sfe_renderer.hpp"
#include "sfe_gl_check.hpp"

namespace GFX {
    static unsigned int VAO = (unsigned int)-1;
    static unsigned int PROGRAM = (unsigned int)-1;
    static bool DEPTH = false;
    static bool STENCIL = false;
    static bool BLENDING = false;
    static bool WIREFRAME = false;
    static int DRAW_CALL_COUNT = 0;

    void BindVAO(unsigned int vao) {
        if (VAO == vao) {
            return;
        }

        VAO = vao;
        glCheckError(glBindVertexArray(VAO));
    }

    void BindProgram(unsigned int program) {
        if (PROGRAM == program) {
            return;
        }

        PROGRAM = program;
        glCheckError(glUseProgram(PROGRAM));
    }

    void SetDepthTest(bool depth) {
        if (DEPTH == depth) {
            return;
        }

        DEPTH = depth;
        if (DEPTH) {
            glCheckError(glEnable(GL_DEPTH_TEST));
        } else {
            glCheckError(glDisable(GL_DEPTH_TEST));
        }
    }

    void SetStencilTest(bool stencil) {
        if (STENCIL == stencil) {
            return;
        }

        STENCIL = stencil;
        if (STENCIL) {
            glCheckError(glEnable(GL_STENCIL_TEST));
        } else {
            glCheckError(glDisable(GL_STENCIL_TEST));
        }
    }

    void SetBlending(bool blending) {
        if (BLENDING == blending) {
            return;
        }

        BLENDING = blending;
        if (BLENDING) {
            glCheckError(glEnable(GL_BLEND));
        } else {
            glCheckError(glDisable(GL_BLEND));
        }
    }

    void SetWireFrame(bool wireframe) {
        if (WIREFRAME == wireframe) {
            return;
        }

        WIREFRAME = wireframe;
        GLenum mode = WIREFRAME ? GL_LINE : GL_FILL;
        glCheckError(glPolygonMode(GL_FRONT_AND_BACK, mode));
    }

    Math::Mat4 GetProjectionMatrix2D(unsigned int width, unsigned int height) {
        float l = 0.0f; 
        float r = (float)width; 
        float b = (float)height;
        float t = 0.0f; 
        float near_plane = -1.0f; 
        float far_plane = 1.0f;

        return Math::Mat4::Orthographic(l, r, b, t, near_plane, far_plane);
    }
        
    Math::Mat4 GetProjectionMatrix3D(unsigned int width, unsigned int height, float fov) {
        float aspect = width / height;
        float near_plane = 0.1f;
        float far_plane = 3000.0f;

        return Math::Mat4::Perspective(fov, aspect, near_plane, far_plane);
    }

    int GetDrawCallCount() {
        return DRAW_CALL_COUNT;
    }

    void IncrementDrawCallCount() {
        DRAW_CALL_COUNT += 1;
    }

    void ClearTelemetry() {
        DRAW_CALL_COUNT = 0;
    }


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
    void DrawGeometry(Geometry &geometry, ShaderBase* shader) {
        shader->use();
        geometry.VAO.bind();

        for (Geometry* geo = &geometry; geo != nullptr; geo = geo->next) {
            if (geo->vertex_count == 0) {
                continue;
            }

            shader->setMaterial(geo->material);

            if (geo->index_count > 0) {
                GFX::IncrementDrawCallCount();
                glCheckError(glDrawElementsBaseVertex(
                    geometry.draw_type, geo->index_count, GL_UNSIGNED_INT, 
                    (void*)(sizeof(unsigned int) * geo->base_index), 
                    geo->base_vertex
                ));
            } else {
                GFX::IncrementDrawCallCount();
                glCheckError(glDrawArrays(
                    geometry.draw_type,
                    geo->base_vertex,
                    geo->vertex_count
                ));
            }
        }
    }

    void DrawGeometryInstanced(Geometry &geometry, ShaderBase* shader, int instance_count) {
        shader->use();
        geometry.VAO.bind();

        for (Geometry* geo = &geometry; geo != nullptr; geo = geo->next) {
            if (geo->vertex_count == 0) {
                continue;
            }

            shader->setMaterial(geo->material);

            if (geo->index_count > 0) {
                GFX::IncrementDrawCallCount();
                glCheckError(glDrawElementsInstancedBaseVertex(
                    geometry.draw_type, geo->index_count,
                    GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * geo->base_index),
                    instance_count, geo->base_vertex
                ));
            } else {
                GFX::IncrementDrawCallCount();
                glCheckError(glDrawArraysInstanced(
                    geometry.draw_type,
                    geo->base_vertex,
                    geo->vertex_count,
                    instance_count
                ));
            }
        }
    }

    void DrawText(Font font, const char* text, int x, int y, ShaderBase* shader) {
        int x_position = x;
        int y_position = y;
        for (int i = 0; i < String::Length(text); i++) {
            char codepoint = text[i];
            if (codepoint == '\n') {
                y_position += font.line_height;
                x_position = x;
                continue;
            }

            Glyph g = font.glyphs.get(codepoint);
            if (codepoint == ' ') {
                x_position += g.x_advance;
                continue;
            }

            Math::Mat4 model = Math::Mat4::Identity();
            model = Math::Mat4::Scale(model, 1);
            model = Math::Mat4::Translate(model, x_position, y_position, 0.0f);

            shader->setModel(model);
            shader->setTexture2D("uTexture", 0, g.texture);
            

            // Date: December 26, 2025
            // TODO(Jovanni): Obviously this is bad its a different VAO for each draw call at least fix that please...
            // But also maybe find a way to do instancing here?
            // What you could do is have a single quad and just scale it and translate it depending on the codepoint (yes this is smart and will work with instancing)
            GFX::SetBlending(true);
            GFX::DrawGeometry(g.quad, shader);
            GFX::SetBlending(false);

            x_position += g.x_advance;
        }
    }
}
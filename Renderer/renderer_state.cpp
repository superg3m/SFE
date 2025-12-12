#include <glad/glad.h>

#include <Common/common.hpp>
#include <renderer_state.hpp>
#include <gl_check.hpp>

namespace Renderer {
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
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void SetStencilTest(bool stencil) {
        if (STENCIL == stencil) {
            return;
        }

        STENCIL = stencil;
        if (STENCIL) {
            glEnable(GL_STENCIL_TEST);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
    }

    void SetBlending(bool blending) {
        if (BLENDING == blending) {
            return;
        }

        BLENDING = blending;
        if (BLENDING) {
            glEnable(GL_BLEND);
        } else {
            glDisable(GL_BLEND);
        }
    }

    void SetWireFrame(bool wireframe) {
        if (WIREFRAME == wireframe) {
            return;
        }

        WIREFRAME = wireframe;
        GLenum mode = WIREFRAME ? GL_LINE : GL_FILL;
        glPolygonMode(GL_FRONT_AND_BACK, mode);
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
}



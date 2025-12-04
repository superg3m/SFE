#include <renderer.hpp>

namespace Renderer {
    static bool WIREFRAME = false;
    static unsigned int VAO;
    static unsigned int PROGRAM;

    void BindVAO(unsigned int vao) {
        if (VAO == vao) {
            return;
        }

        VAO = vao;
        glBindVertexArray(VAO);
    }

    void BindProgram(unsigned int program) {
        if (PROGRAM == program) {
            return;
        }

        PROGRAM = program;
        glUseProgram(PROGRAM);
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
        float far_plane = 200.0f;

        return Math::Mat4::Perspective(fov, aspect, near_plane, far_plane);
    }
}



#pragma once

#include <Math/math.hpp>


// ACTIVE_VAO
// WIRE_FRAME_MODE
// projection_mode (3d or 2d)

// the goal would be to render 3d stuff then render a small 2d square

// lower the overhead of the driver calls by checking first

void glCheckError_(const char *file, int line);
#define GL_ERROR_CHECK
#if defined(GL_ERROR_CHECK)
    #define glCheckError(glCall) glCall; glCheckError_(__FILE__, __LINE__) 
#else
    #define glCheckError(glCall) glCall
#endif

namespace Renderer {
    enum ProjectionMode {
        PROJECTION_2D,
        PROJECTION_3D
    };

    void BindVAO(unsigned int vao);
    void BindProgram(unsigned int program);
    void SetDepthTest(bool depth);
    void SetStencilTest(bool stencil);
    void SetBlending(bool blending);
    void SetWireFrame(bool wireframe);
    Math::Mat4 GetProjectionMatrix2D(unsigned int width, unsigned int height);
    Math::Mat4 GetProjectionMatrix3D(unsigned int width, unsigned int height, float fov);
}
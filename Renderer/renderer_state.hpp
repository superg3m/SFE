#pragma once

#include <Math/math.hpp>

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

    int GetDrawCallCount();
    void IncrementDrawCallCount();
    void ClearTelemetry();
}
#pragma once

#include "Glyph/sfe_glyph.hpp"
#include "Shader/Base/sfe_shader_base.hpp"
#include "../../Core/Math/sfe_math.hpp"

namespace GFX {
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

    void DrawGeometry(Geometry &geometry, ShaderBase* shader);
    void DrawGeometryInstanced(Geometry &geometry, ShaderBase* shader, int instance_count);
    void DrawText(Font font, const char* text, int x, int y, ShaderBase* shader);

    int GetDrawCallCount();
    void IncrementDrawCallCount();
    void ClearTelemetry();
}
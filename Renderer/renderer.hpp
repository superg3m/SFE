#pragma once

#include <Camera/camera.hpp>
#include <Entity/entity.hpp>
#include <Geometry/geometry.hpp>
#include <Geometry/material.hpp>

#include <Shader/shader_base.hpp>
#include <Shader/shader_model.hpp>
#include <Shader/shader_uniform_color.hpp>
#include <Shader/shader_diffuse.hpp>

#include <Math/math.hpp>


// ACTIVE_VAO
// WIRE_FRAME_MODE
// projection_mode (3d or 2d)

// the goal would be to render 3d stuff then render a small 2d square

// lower the overhead of the driver calls by checking first
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
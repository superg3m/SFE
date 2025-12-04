#pragma once

#include <Camera/camera.hpp>
#include <Entity/entity.hpp>
#include <Geometry/geometry.hpp>
#include <Geometry/material.hpp>

#include <Shader/shader_base.hpp>
#include <Shader/shader_model.hpp>
#include <Shader/shader_uniform_color.hpp>
#include <Shader/shader_diffuse.hpp>

/*
// ACTIVE_VAO
// WIRE_FRAME_MODE
// projection_mode (3d or 2d)

// the goal would be to render 3d stuff then render a small 2d square

// lower the overhead of the driver calls by checking first
namespace Renderer {
    void BindVAO();
    void SetWireFrame(bool toggle);s
};

// inside renderer.cpp file
    static unsigned int VAO;

// drawSquare(x, y, width, height, color);
// drawCircle(center, radius, color);
*/
#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>

#include <Geometry/material.hpp>
#include <Math/matrix.hpp>

struct ShaderBase {
    unsigned int program_id = 0;
    virtual void compile() = 0;

    void use() const;

    // Vertex Uniforms
    void setModel(Math::Matrix4 &model) const;
    void setView(Math::Matrix4 &view) const;
    void setProjection(Math::Matrix4 &projection) const;

    // Fragment Uniforms
    void setMaterial(Material &material) const;
protected:
    std::vector<const char*> shader_paths;
    ShaderBase() = default;
    GLenum typeFromPath(const char* path);
    void checkCompileError(unsigned int source_id, const char* path);
    unsigned int shaderSourceCompile(const char* path);
    unsigned int getUniformLocation(const char* name, bool log_error = true) const;
    unsigned int createShaderProgram(std::vector<const char*> shader_paths);
};
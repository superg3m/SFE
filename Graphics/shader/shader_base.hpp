#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>

#include <DataStructure/ds.hpp>
#include <Geometry/material.hpp>
#include <Math/matrix.hpp>

static const char* glEnumToString(GLenum type) {
    switch (type) {
        case GL_BOOL:          return "bool";
        case GL_INT:           return "int";
        case GL_FLOAT:         return "float";
        case GL_FLOAT_VEC2:    return "vec2";
        case GL_FLOAT_VEC3:    return "vec3";
        case GL_FLOAT_VEC4:    return "vec4";
        case GL_FLOAT_MAT4:    return "mat4";
        case GL_SAMPLER_2D:    return "sampler2D";
        case GL_SAMPLER_CUBE:  return "samplerCube";
        
        default:               return "unknown";
    }
}

struct ShaderBase {
    unsigned int program_id = 0;
    virtual void compile() = 0;

    void use() const;

    // Vertex Uniforms
    void setModel(Math::Matrix4 &model) const;
    void setView(Math::Matrix4 &view) const;
    void setProjection(Math::Matrix4 &projection) const;

    void setBool(const char* name, bool value) const;
    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;
    void setVec2(const char* name, const Math::Vector2& value) const;
    void setVec2(const char* name, float x, float y) const;
    void setVec3(const char* name, const Math::Vector3& value) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setVec4(const char* name, const Math::Vector4& value) const;
    void setVec4(const char* name, float x, float y, float z, float w) const;
    void setMat4(const char* name, const Math::Matrix4& mat) const;
protected:
    DS::Vector<const char*> shader_paths;
    DS::Hashmap<const char*, GLenum> uniforms;

    ShaderBase() = default;
    GLenum typeFromPath(const char* path);
    void checkCompileError(unsigned int source_id, const char* path);
    unsigned int shaderSourceCompile(const char* path);
    unsigned int getUniformLocation(const char* name, GLenum type) const;
    unsigned int createShaderProgram(DS::Vector<const char*> shader_paths);

    void setBool(unsigned int location, bool value) const;
    void setInt(unsigned int location, int value) const;
    void setFloat(unsigned int location, float value) const;
    void setVec2(unsigned int location, const Math::Vector2& value) const;
    void setVec2(unsigned int location, float x, float y) const;
    void setVec3(unsigned int location, const Math::Vector3& value) const;
    void setVec3(unsigned int location, float x, float y, float z) const;
    void setVec4(unsigned int location, const Math::Vector4& value) const;
    void setVec4(unsigned int location, float x, float y, float z, float w) const;
    void setMat4(unsigned int location, const Math::Matrix4& mat) const;
};
#pragma once

#include <glad/glad.h>

#include "../../Core/DataStructure/sfe_ds.hpp"
#include "../../Core/Math/sfe_matrix.hpp"
#include "../../Geometry/sfe_material.hpp"
#include "../../sfe_gl_check.hpp"

struct UniformDesc {
    GLenum type;
    GLint location;
};

struct ShaderBase {
    unsigned int program_id = 0;

    virtual void compile() = 0;
    virtual void setMaterial(const Material &material) = 0;

    void use() const;
    void setModel(Math::Mat4 &model);
    void setView(Math::Mat4 &view);
    void setProjection(Math::Mat4 &projection);

    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
    void setVec2(const char* name, const Math::Vec2& value);
    void setVec2(const char* name, float x, float y);
    void setVec3(const char* name, const Math::Vec3& value);
    void setVec3(const char* name, float x, float y, float z);
    void setVec4(const char* name, const Math::Vec4& value);
    void setVec4(const char* name, float x, float y, float z, float w);
    void setMat4(const char* name, const Math::Mat4& mat);
protected:
    DS::Vector<const char*> shader_paths;
    DS::Hashmap<const char*, UniformDesc> uniforms;

    ShaderBase() = default;
    GLenum typeFromPath(const char* path);
    void checkCompileError(unsigned int source_id, const char* path);
    unsigned int shaderSourceCompile(const char* path);
    unsigned int getUniformLocation(const char* name, GLenum type);
    unsigned int createShaderProgram(DS::Vector<const char*> shader_paths);

    void setBool(unsigned int location, bool value);
    void setInt(unsigned int location, int value);
    void setFloat(unsigned int location, float value);
    void setVec2(unsigned int location, const Math::Vec2& value);
    void setVec2(unsigned int location, float x, float y);
    void setVec3(unsigned int location, const Math::Vec3& value);
    void setVec3(unsigned int location, float x, float y, float z);
    void setVec4(unsigned int location, const Math::Vec4& value);
    void setVec4(unsigned int location, float x, float y, float z, float w);
    void setMat4(unsigned int location, const Math::Mat4& mat);
};
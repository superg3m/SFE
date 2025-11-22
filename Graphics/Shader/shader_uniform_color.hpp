#pragma once

#include <Shader/shader_base.hpp>

struct ShaderUniformColor : public ShaderBase {
    ShaderUniformColor() = default;
    ShaderUniformColor(DS::Vector<const char*> shader_paths);
    void compile() override;
    void setColor(Math::Vector3 color);
private:
    unsigned int uColor_Location = 0;
};
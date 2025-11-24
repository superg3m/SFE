#pragma once

#include <Shader/shader_base.hpp>

struct ShaderDiffuse : public ShaderBase {
    ShaderDiffuse() = default;
    ShaderDiffuse(DS::Vector<const char*> shader_paths);
    void compile() override;

    void setLightPosition(const Math::Vec3& light_postion);
private:
    unsigned int uLightPosition_Location;
};
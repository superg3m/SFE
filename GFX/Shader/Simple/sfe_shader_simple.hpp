#pragma once

#include "../Base/sfe_shader_base.hpp"

struct ShaderSimple : public ShaderBase {
    ShaderSimple() = default;
    ShaderSimple(DS::Vector<const char*> shader_paths);
    void compile() override;
    void setMaterial(const Material &material) const override;
};
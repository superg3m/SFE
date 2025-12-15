#pragma once

#include "../Base/sfe_shader_base.hpp"

struct ShaderOneTexture : public ShaderBase {
    ShaderOneTexture() = default;
    ShaderOneTexture(DS::Vector<const char*> shader_paths);
    void compile() override;
    void setMaterial(const Material &material) const override;

    void setTexture(Texture &texture);
private:
    unsigned int uTexture_Location;
};
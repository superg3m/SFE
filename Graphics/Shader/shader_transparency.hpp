#pragma once

#include <vector>

#include <Shader/shader_base.hpp>

struct ShaderTransparency : public ShaderBase {
    ShaderTransparency() = default;
    ShaderTransparency(DS::Vector<const char*> shader_paths);
    void compile() override;
    
    void setTexture(GLTextureID texture) const;
    void setOpacity(float opacity) const;
};
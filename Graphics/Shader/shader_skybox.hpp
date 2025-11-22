#pragma once

#include <vector>

#include <Shader/shader_base.hpp>

struct ShaderSkybox : public ShaderBase {
    ShaderSkybox() = default;
    ShaderSkybox(DS::Vector<const char*> shader_paths);
    void compile() override;
    
    void setSkyboxTexture(GLTextureID texture) const;

private:
    unsigned int uSkyboxTexture_Location;
};
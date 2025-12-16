#pragma once

#include "../Base/sfe_shader_base.hpp"
#include "../../Lighting/sfe_lighting.hpp"

#define LIGHT_COUNT 4

struct ShaderMaterial : public ShaderBase {
    ShaderMaterial() = default;
    ShaderMaterial(DS::Vector<const char*> shader_paths);

    void compile() override;

    // Fragment Uniforms
    void setLightPosition(Math::Vec3 &light_position);
    void setCameraPosition(Math::Vec3 &camera_position);
    void setMaterial(const Material &material) override;
    void setEmissiveMaterial(bool should_emit);
private:
    // Fragment Uniforms
    unsigned int uCameraPosition_Location;
    unsigned int uApplyEmissiveMaterial_Location;
    MaterialLocation uMaterial_Location;
};
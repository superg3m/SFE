#include <Shader/shader_base.hpp>
#include <Lighting/lighting.hpp>

#define LIGHT_COUNT 4

struct ShaderModel : public ShaderBase {
    ShaderModel() = default;
    ShaderModel(DS::Vector<const char*> shader_paths);
    void compile() override;
    
    // Fragment Uniforms
    void setSpotLight(SpotLight &spot_light) const;
    void setDirectionalLight(DirectionalLight &directional_light) const;
    void setPointLight(PointLight &point_light, int index) const;
    void setViewPosition(Math::Vector3 &view_position) const;
    void setUseFlashlight(bool useFlashlight) const;
    void setMaterial(Material &material) const;
private:
    // Fragment Uniforms
    SpotLightLocation uSpotLight_Location;
    DirectionalLightLocation uDirectionalLight_Location;
    PointLightLocation uPointLight_Locations[LIGHT_COUNT];
    unsigned int uViewPosition_Location;
    unsigned int uUseFlashlight_Location;

    MaterialLocation uMaterial_Location;
};
#include "../Base/sfe_shader_base.hpp"
#include "../../Lighting/sfe_lighting.hpp"

#define LIGHT_COUNT 4

struct ShaderNoMaterial : public ShaderBase {
    ShaderNoMaterial() = default;
    ShaderNoMaterial(DS::Vector<const char*> shader_paths);

    void compile() override;
    void setMaterial(const Material &material) override;
};
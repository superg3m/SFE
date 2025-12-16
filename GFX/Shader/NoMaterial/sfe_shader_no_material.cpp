#include "sfe_shader_no_material.hpp"
ShaderNoMaterial::ShaderNoMaterial(DS::Vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderNoMaterial::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);
}

void ShaderNoMaterial::setMaterial(const Material &material) {}
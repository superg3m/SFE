#include "sfe_shader_simple.hpp"

ShaderSimple::ShaderSimple(DS::Vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderSimple::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);
}

void ShaderSimple::setMaterial(const Material& material) const {}
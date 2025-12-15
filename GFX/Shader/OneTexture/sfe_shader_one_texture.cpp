#include "sfe_shader_one_texture.hpp"

ShaderOneTexture::ShaderOneTexture(DS::Vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderOneTexture::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);
}

void ShaderOneTexture::setMaterial(const Material& material) const {}

void ShaderOneTexture::setTexture(Texture &texture) {
    this->use();
    glCheckError(glActiveTexture(GL_TEXTURE0 + 0));
    glCheckError(glBindTexture(GL_TEXTURE_2D, texture.id));
    glUniform1i(this->uTexture_Location, 0);
}
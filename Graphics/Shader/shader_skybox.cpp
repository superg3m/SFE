#include <Shader/shader_skybox.hpp>

ShaderSkybox::ShaderSkybox(DS::Vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderSkybox::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);

    this->uSkyboxTexture_Location = this->getUniformLocation("uSkyboxTexture");
}

void ShaderSkybox::setSkyboxTexture(GLTextureID texture) const {
    this->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glUniform1i(this->uSkyboxTexture_Location, 0); 
}
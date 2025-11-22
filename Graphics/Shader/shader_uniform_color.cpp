#include <Shader/shader_uniform_color.hpp>

ShaderUniformColor::ShaderUniformColor(DS::Vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderUniformColor::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);
    this->uColor_Location = getUniformLocation("uColor");
}

void ShaderUniformColor::setColor(Math::Vector3 color) {
    this->use();

    this->setVec3(this->uColor_Location, 1, &color.r);
}
#include <Shader/shader_diffuse.hpp>

ShaderDiffuse::ShaderDiffuse(DS::Vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderDiffuse::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);

    this->uLightPosition_Location = this->getUniformLocation("uLightPosition", GL_FLOAT_VEC3);
}

void ShaderDiffuse::setLightPosition(const Math::Vec3& light_postion) {
    this->setVec3(this->uLightPosition_Location, light_postion);
}
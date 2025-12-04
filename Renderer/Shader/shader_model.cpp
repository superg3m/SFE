#include <String/string.hpp>
#include <Shader/shader_model.hpp>
#include <glad/glad.h>

ShaderModel::ShaderModel(DS::Vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderModel::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);

    /*
    this->uSpotLight_Location.position = this->getUniformLocation("uSpotLight.position", GL_FLOAT_VEC3);
    this->uSpotLight_Location.direction = this->getUniformLocation("uSpotLight.direction", GL_FLOAT_VEC3);
    this->uSpotLight_Location.cutOff = this->getUniformLocation("uSpotLight.cutOff", GL_FLOAT);
    this->uSpotLight_Location.outerCutOff = this->getUniformLocation("uSpotLight.outerCutOff", GL_FLOAT);
    this->uSpotLight_Location.constant = this->getUniformLocation("uSpotLight.constant", GL_FLOAT);
    this->uSpotLight_Location.linear = this->getUniformLocation("uSpotLight.linear", GL_FLOAT);
    this->uSpotLight_Location.diffuse = this->getUniformLocation("uSpotLight.quadratic", GL_FLOAT);
    this->uSpotLight_Location.ambient = this->getUniformLocation("uSpotLight.ambient", GL_FLOAT_VEC3);
    this->uSpotLight_Location.diffuse = this->getUniformLocation("uSpotLight.diffuse", GL_FLOAT_VEC3);
    this->uSpotLight_Location.specular = this->getUniformLocation("uSpotLight.specular", GL_FLOAT_VEC3);

    this->uDirectionalLight_Location.direction = this->getUniformLocation("uDirLight.direction", GL_FLOAT_VEC3);
    this->uDirectionalLight_Location.ambient = this->getUniformLocation("uDirLight.ambient", GL_FLOAT_VEC3);
    this->uDirectionalLight_Location.diffuse = this->getUniformLocation("uDirLight.diffuse", GL_FLOAT_VEC3);
    this->uDirectionalLight_Location.specular = this->getUniformLocation("uDirLight.specular", GL_FLOAT_VEC3);

    for (int i = 0; i < LIGHT_COUNT; i++) {
        char* position = String::sprintf(nullptr ,"uPointLights[%d].position", i);
        char* constant = String::sprintf(nullptr ,"uPointLights[%d].constant", i);
        char* linear = String::sprintf(nullptr ,"uPointLights[%d].linear", i);
        char* quadratic = String::sprintf(nullptr ,"uPointLights[%d].quadratic", i);
        char* ambient = String::sprintf(nullptr ,"uPointLights[%d].ambient", i);
        char* diffuse = String::sprintf(nullptr ,"uPointLights[%d].diffuse", i);
        char* specular = String::sprintf(nullptr ,"uPointLights[%d].specular", i);

        this->uPointLight_Locations[i].position = this->getUniformLocation(position, GL_FLOAT_VEC3);
        this->uPointLight_Locations[i].constant = this->getUniformLocation(constant, GL_FLOAT);
        this->uPointLight_Locations[i].linear = this->getUniformLocation(linear, GL_FLOAT);
        this->uPointLight_Locations[i].quadratic = this->getUniformLocation(quadratic, GL_FLOAT);
        this->uPointLight_Locations[i].ambient = this->getUniformLocation(ambient, GL_FLOAT_VEC3);
        this->uPointLight_Locations[i].diffuse = this->getUniformLocation(diffuse, GL_FLOAT_VEC3);
        this->uPointLight_Locations[i].specular = this->getUniformLocation(specular, GL_FLOAT_VEC3);

        Memory::Free(position);
        Memory::Free(constant);
        Memory::Free(linear);
        Memory::Free(quadratic);
        Memory::Free(ambient);
        Memory::Free(diffuse);
        Memory::Free(specular);
    }

    this->uViewPosition_Location = this->getUniformLocation("uViewPosition", GL_FLOAT_VEC3);
    this->uUseFlashlight_Location = this->getUniformLocation("uUseFlashlight", GL_BOOL);
    */

    this->uMaterial_Location.color_map = this->getUniformLocation("uMaterial.diffuse_map", GL_SAMPLER_2D);
    this->uMaterial_Location.specular_map = this->getUniformLocation("uMaterial.specular_map", GL_SAMPLER_2D);
    this->uMaterial_Location.shininess = this->getUniformLocation("uMaterial.shininess", GL_FLOAT);

    this->uMaterial_Location.opacity = this->getUniformLocation("uMaterial.opacity", GL_FLOAT);
    // this->uMaterial_Location.color = this->getUniformLocation("uMaterial.color", GL_FLOAT_VEC3);
}

void ShaderModel::setMaterial(const Material &material) const {
    this->use();

    if (material.textures[TEXTURE_TYPE_DIFFUSE].id != 0) {
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, material.textures[TEXTURE_TYPE_DIFFUSE].id);
        this->setInt(uMaterial_Location.color_map, 0);
    }

    if (material.textures[TEXTURE_TYPE_SPECULAR].id != 0) {
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, material.textures[TEXTURE_TYPE_SPECULAR].id);
        this->setInt(uMaterial_Location.specular_map, 1);
    }

    this->setVec3(uMaterial_Location.color, material.ambient_color);
    this->setVec3(uMaterial_Location.color, material.diffuse_color);
    this->setVec3(uMaterial_Location.color, material.specular_color);

    this->setFloat(uMaterial_Location.shininess, material.shininess);
    this->setFloat(uMaterial_Location.opacity, material.opacity);
}

void ShaderModel::setSpotLight(SpotLight &spot_light) const {
    this->use();

    this->setVec3(this->uSpotLight_Location.position, spot_light.position);
    this->setVec3(this->uSpotLight_Location.direction, spot_light.direction);
    this->setFloat(this->uSpotLight_Location.cutOff, spot_light.cutOff);
    this->setFloat(this->uSpotLight_Location.outerCutOff, spot_light.outerCutOff);

    this->setFloat(this->uSpotLight_Location.constant, spot_light.constant);
    this->setFloat(this->uSpotLight_Location.linear, spot_light.linear);
    this->setFloat(this->uSpotLight_Location.quadratic, spot_light.quadratic);

    this->setVec3(this->uSpotLight_Location.ambient, spot_light.ambient);
    this->setVec3(this->uSpotLight_Location.diffuse, spot_light.diffuse);
    this->setVec3(this->uSpotLight_Location.specular, spot_light.specular);
}

void ShaderModel::setDirectionalLight(DirectionalLight &directional_light) const {
    this->use();

    this->setVec3(this->uDirectionalLight_Location.direction, directional_light.direction);

    this->setVec3(this->uDirectionalLight_Location.ambient, directional_light.ambient);
    this->setVec3(this->uDirectionalLight_Location.diffuse, directional_light.diffuse);
    this->setVec3(this->uDirectionalLight_Location.specular, directional_light.specular);
}

void ShaderModel::setPointLight(PointLight &point_light, int index) const {
    this->use();

    this->setVec3(this->uPointLight_Locations[index].position, point_light.position);

    this->setFloat(this->uPointLight_Locations[index].constant, point_light.constant);
    this->setFloat(this->uPointLight_Locations[index].linear, point_light.linear);
    this->setFloat(this->uPointLight_Locations[index].quadratic, point_light.quadratic);

    this->setVec3(this->uPointLight_Locations[index].ambient, point_light.ambient);
    this->setVec3(this->uPointLight_Locations[index].diffuse, point_light.diffuse);
    this->setVec3(this->uPointLight_Locations[index].specular, point_light.specular);
}

void ShaderModel::setViewPosition(Math::Vec3 &view_position) const {
    this->use();
    this->setVec3(this->uViewPosition_Location, view_position);
}

void ShaderModel::setUseFlashlight(bool useFlashlight) const {
    this->use();
    this->setBool(this->uUseFlashlight_Location, useFlashlight);
}
#include <glad/glad.h>
#include <stb_image.h>

#include <Common/common.hpp>
#include <Platform/platform.hpp>
#include <Texture/texture.hpp>

Texture::Texture(char* path) {
    RUNTIME_ASSERT_MSG(Platform::filePathExists(path), "Texture path: '%s' doesn't exist!\n", path);
    
    GLenum MIPMAP_TYPE = GL_LINEAR; // GET_BIT(texture_flags, 0) ? GL_NEAREST : GL_LINEAR;
    GLenum TEXTURE_VERTICAL_FLIP = true; // GET_BIT(texture_flags, 1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIPMAP_TYPE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MIPMAP_TYPE);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(TEXTURE_VERTICAL_FLIP);
    u8 *data = stbi_load(path, &width, &height, &nrChannels, 0);

    GLenum internal_format = 0;
    GLenum pixel_format = 0;
    if (nrChannels == 1) {
        internal_format = GL_RED;
        pixel_format = GL_RED;
    } else if (nrChannels == 3) {
        internal_format = GL_RGB; // gammaCorrected ? GL_SRGB : GL_RGB;
        pixel_format = GL_RGB;
    } else if (nrChannels == 4) {
        internal_format = GL_RGBA; // gammaCorrected ? GL_SRGB_ALPHA : GL_RGBA;
        pixel_format = GL_RGBA;
    } else {
        RUNTIME_ASSERT_MSG(false, "TextureLoader | Failed to pick a stb format, most likely related to assimp, try to link your libraries in a different order\n");
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, pixel_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        LOG_ERROR("TextureLoader | Failed to load texture\n");
    }

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);

    if (texture == 0) {
        LOG_ERROR("TextureLoader | id is invalid!\n");
    }

    this->id = texture;
    this->width = width;
    this->height = height;
}

Texture::Texture(const u8* data, int width, int height, int nrChannels) {
    if (!data || width <= 0 || height <= 0 || nrChannels == 0) {
        RUNTIME_ASSERT_MSG(false, "TextureLoader | Invalid input data for loadTextureFromMemory!\n");
    }

    GLenum MIPMAP_TYPE = GL_LINEAR; // GET_BIT(texture_flags, 0) ? GL_NEAREST : GL_LINEAR;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIPMAP_TYPE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MIPMAP_TYPE);

    GLenum format = 0;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        RUNTIME_ASSERT_MSG(false, "TextureLoader | Failed to pick a format for memory texture (channels: %d)\n", nrChannels);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (texture == 0) {
        RUNTIME_ASSERT_MSG(false, "TextureLoader | id is invalid for memory texture!\n");
    }

    this->id = texture;
    this->width = width;
    this->height = height;
}
#pragma once

struct Texture {
    unsigned int id = 0;
    unsigned int width = 0;
    unsigned int height = 0;


    Texture() = default;
    Texture(char* path);
    Texture(const u8* data, int width, int height, int nrChannels);
};
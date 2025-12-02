#pragma once

struct Texture {
    unsigned int id = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    // sampler2d or cubemap


    Texture();
    static Texture LoadFromFile(char* path);
    static Texture LoadFromMemory(const u8* data, int width, int height, int nrChannels);
};
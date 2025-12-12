#version 330 core

in vec2 v_TextureCoord;

out vec4 FragColor;

uniform sampler2D uCloudTexture;

void main() {
    vec4 texel = texture(uCloudTexture, v_TextureCoord + vec2(0.1, 0.2));
    texel.a = texel.r;

    FragColor = texel;
}


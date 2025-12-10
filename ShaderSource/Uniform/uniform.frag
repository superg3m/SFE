#version 330 core

uniform sampler2D uTexture;

in vec2 v_TexCoord;
out vec4 FragColor;

void main() {
    vec3 texel = texture(uTexture, v_TexCoord).rgb;
    FragColor = vec4(texel, 1);
}
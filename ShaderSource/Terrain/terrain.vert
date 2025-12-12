#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 v_FragPosition;
out vec2 v_TextureCoord;
out vec3 v_Normal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform int uHeightBoost;
uniform float uWidth;
uniform float uHeight;
uniform sampler2D uColorTexture;
uniform sampler2D uHeightTexture;

void main() {
    float X_STEP = 1.0 / uWidth;
    float Z_STEP = 1.0 / uHeight;

    vec3 tc_position = aPos;
    vec3 tr_position = vec3(tc_position.x + 1, tc_position.y, tc_position.z);
    vec3 tu_position = vec3(tc_position.x, tc_position.y, tc_position.z + 1); // should this be - Z_STEP?

    vec2 tc_uv = aTexCoord;
    vec2 tr_uv = vec2(tc_uv.x + X_STEP, tc_uv.y);
    vec2 tu_uv = vec2(tc_uv.x, tc_uv.y + Z_STEP); // should this be + Z_STEP?

    float tc_height = texture(uHeightTexture, tc_uv).r;
    float tr_height = texture(uHeightTexture, tr_uv).r;
    float tu_height = texture(uHeightTexture, tu_uv).r;

    tc_position.y += uHeightBoost * tc_height;
    tr_position.y += uHeightBoost * tr_height;
    tu_position.y += uHeightBoost * tu_height;

    vec3 approximate_normal = normalize(cross(tu_position - tc_position, tr_position - tc_position));
    mat3 normal_matrix = transpose(inverse(mat3(uModel)));

    v_FragPosition = vec3(uModel * vec4(tc_position, 1.0));
    v_TextureCoord = tc_uv;
    v_Normal = normal_matrix * approximate_normal;

    gl_Position = uProjection * uView * uModel * vec4(tc_position, 1);
}
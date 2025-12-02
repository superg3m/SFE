#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 v_FragPosition;
out vec3 v_Normal;

void main() {
    v_FragPosition = vec3(uModel * vec4(aPos, 1.0f));
    v_Normal = mat3(transpose(inverse(uModel))) * aNormal;

    gl_Position = uProjection * uView * vec4(v_FragPosition, 1.0f);
}
#version 330 core

in vec3 v_FragPosition;
in vec3 v_Normal;
out vec4 FragColor;

uniform vec3 uLightPosition;

void main() {
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(uLightPosition - v_FragPosition);
    float lambertian_mask = max(dot(N, L), 0.0);
	vec4 color = vec4(v_Normal, 1);
    vec4 ambient_light = (color * 0.25);
    vec4 diffuse_light = (color * lambertian_mask);

    FragColor = ambient_light + diffuse_light;
}
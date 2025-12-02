#version 330 core

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;

    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    
    float shininess;
    float opacity;
};

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoord;
out vec4 FragColor;

uniform Material uMatieral;
uniform vec3 uLightPosition;

void main() {
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(uLightPosition - v_FragPosition);
    float lambertian_mask = max(dot(N, L), 0.0);

    vec3 diffuse_texture = texture(uMatieral.diffuse_map, v_TexCoord).rgb;

	vec4 color = vec4(diffuse_texture, 1); // vec4(v_Normal, 1);
    vec4 ambient_light = (color * 0.25);
    vec4 diffuse_light = (color * lambertian_mask);

    FragColor = ambient_light + diffuse_light;
}
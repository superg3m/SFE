#version 330 core

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D emissive_map;

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

uniform Material uMaterial;
uniform vec3 uLightPosition;
uniform bool uApplyEmissiveMaterial;

void main() {
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(uLightPosition - v_FragPosition);
    float lambertian_mask = max(dot(N, L), 0.0);

    vec3 diffuse_texture = texture(uMaterial.diffuse_map, v_TexCoord).rgb;

	vec3 color = diffuse_texture;
    vec3 ambient_light = (color * 0.25);
    vec3 diffuse_light = (color * lambertian_mask);
    vec3 emissive = vec3(0);
    if (uApplyEmissiveMaterial) {
        emissive = texture(uMaterial.emissive_map, v_TexCoord).rgb;
    }

    FragColor = vec4(ambient_light + diffuse_light + emissive, 1);
}
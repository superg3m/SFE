#version 330 core

struct Material {
    sampler2D diffuse_map;
    bool has_diffuse_map;

    sampler2D specular_map;
    bool has_specular_map;

    sampler2D emissive_map;
    bool has_emissive_map;

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

    vec3 base_color;
    if (uMaterial.has_diffuse_map) {
        base_color = texture(uMaterial.diffuse_map, v_TexCoord).rgb;
    } else {
        base_color = uMaterial.diffuse_color;
    }

    vec3 ambient_light = (base_color * uMaterial.ambient_color);
    vec3 diffuse_light = (base_color * lambertian_mask);

    /*
    vec3 specular_light = vec3(0.0);
    if (uMaterial.has_specular_map || uMaterial.shininess > 0.0) {
        vec3 H = normalize(L + V);
        float specular_mask = pow(max(dot(N, H), 0.0), uMaterial.shininess);
        vec3 specular_map_color = uMaterial.has_specular_map ? texture(uMaterial.specular_map, v_TexCoord).rgb : uMaterial.specular_color;
            
        specular_light = specular_map_color * specular_mask;
    }
    */

    vec3 emissive = vec3(0.0);
    if (uMaterial.has_emissive_map && uApplyEmissiveMaterial) {
        emissive = texture(uMaterial.emissive_map, v_TexCoord).rgb;
    }

    vec3 final_rgb = ambient_light + diffuse_light + emissive;
    FragColor = vec4(final_rgb, uMaterial.opacity);
}
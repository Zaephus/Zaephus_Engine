#version 330 core

struct Material {
    vec4 color;
    float shininess;
};

struct Light {
    vec3 position;

    vec4 color;
    float ambientStrength;
    float specularStrength;
};

out vec4 fragColor;

in vec3 fragPos;
in vec4 vertexColor;
in vec3 normal;
in vec2 uv;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

vec3 norm;

void main() {
    vec4 ambient = light.ambientStrength * light.color * material.color * vertexColor;

    if(gl_FrontFacing) { norm = normalize(normal); }
    else { norm = -normalize(normal); }

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = light.color * diff * material.color * vertexColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(normal, reflectDir), 0.0), material.shininess);
    vec4 specular = light.color * light.specularStrength * spec * material.color;

    vec4 result = ambient + diffuse;
    result.a = material.color.a;
    fragColor = result;
}
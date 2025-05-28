#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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

void main() {
    vec4 ambient = light.ambientStrength * light.color * texture(material.diffuse, uv);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = light.color * diff * texture(material.diffuse, uv);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = light.color * light.specularStrength * spec * texture(material.specular, uv);

    vec4 result = ambient + diffuse + specular;
    result.a = texture(material.diffuse, uv).a;
    fragColor = result;
}
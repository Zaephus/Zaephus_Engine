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
    vec3 ambient = light.ambientStrength * light.color.rgb * vec3(texture(material.diffuse, uv));

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color.rgb * diff * vec3(texture(material.diffuse, uv));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color.rgb * light.specularStrength * spec * vec3(texture(material.specular, uv));

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
//    fragColor = texture(material.specular, uv);
//    fragColor = mix(texture(texture_diffuse1, uv), texture(texture_diffuse2, uv), 0.2);
}
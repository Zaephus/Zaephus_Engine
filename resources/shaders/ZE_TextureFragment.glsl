#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec4 color;
    float ambientStrength;
    float specularStrength;
};

struct PointLight {
    vec3 position;

    vec4 color;
    float ambientStrength;
    float specularStrength;

    float constant;
    float linear;
    float quadratic;
};

out vec4 fragColor;

in vec3 fragPos;
in vec4 vertexColor;
in vec3 normal;
in vec2 uv;

uniform Material material;

uniform DirectionalLight dirLight;
uniform PointLight pointLight;

uniform vec3 viewPos;

vec4 calcDirLight(vec3 _normal);
vec4 calcPointLight(vec3 _normal);

void main() {
    vec3 norm = normalize(normal);

    vec4 result = calcDirLight(norm) + calcPointLight(norm);
    result.a = texture(material.diffuse, uv).a;
    fragColor = result;
}

vec4 calcDirLight(vec3 _normal) {
    vec4 ambient = dirLight.ambientStrength * dirLight.color * texture(material.diffuse, uv);

    vec3 lightDir = normalize(dirLight.direction);
    float diff = max(dot(_normal, lightDir), 0.0);
    vec4 diffuse = dirLight.color * diff * texture(material.diffuse, uv);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = dirLight.color * dirLight.specularStrength * spec * texture(material.specular, uv);

    return ambient + diffuse + specular;
}

vec4 calcPointLight(vec3 _normal) {
    vec4 ambient = pointLight.ambientStrength * pointLight.color * texture(material.diffuse, uv);

    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(_normal, lightDir), 0.0);
    vec4 diffuse = pointLight.color * diff * texture(material.diffuse, uv);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = pointLight.color * pointLight.specularStrength * spec * texture(material.specular, uv);

    float dist = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * dist * dist);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
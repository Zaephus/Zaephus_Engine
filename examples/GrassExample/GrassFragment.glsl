#version 330 core

struct Material {
    vec4 color;
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
    vec3 norm = gl_FrontFacing ? normalize(normal) : normalize(vec3(-normal.x, normal.y, -normal.z));

    vec4 result = calcDirLight(norm);// + calcPointLight(norm);
    result.a = material.color.a;
    fragColor = result;
}
vec4 calcDirLight(vec3 _normal) {
    vec4 ambient = dirLight.ambientStrength * dirLight.color * material.color;

    vec3 lightDir = normalize(dirLight.direction);
    float diff = max(dot(_normal, lightDir), 0.0);
    vec4 diffuse = dirLight.color * diff * material.color;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = dirLight.color * dirLight.specularStrength * spec * material.color;

    return ambient + diffuse + specular;
}

vec4 calcPointLight(vec3 _normal) {
    vec4 ambient = pointLight.ambientStrength * pointLight.color * material.color;

    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(_normal, lightDir), 0.0);
    vec4 diffuse = pointLight.color * diff * material.color;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = pointLight.color * pointLight.specularStrength * spec * material.color;

    float dist = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * dist * dist);

    if(attenuation < 0.0) { return vec4(1.0, 0.0, 0.0, 1.0); }

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

#version 330 core

struct Material {
    vec4 color;
    float shininess;
};
uniform Material material;

struct DirectionalLight {
    vec3 direction;

    vec4 color;

    float intensity;
    float ambientStrength;
    float specularStrength;
};
#define MAX_DIR_LIGHTS 5
uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform int dirLightAmount;

struct PointLight {
    vec3 position;

    vec4 color;

    float intensity;
    float ambientStrength;
    float specularStrength;

    float constant;
    float linear;
    float quadratic;
};
#define MAX_POINT_LIGHTS 12
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightAmount;

out vec4 fragColor;

in vec3 fragPos;
in vec4 vertexColor;
in vec3 normal;
in vec2 uv;

uniform vec3 viewPos;

vec4 calcDirLight(DirectionalLight _dirLight, vec3 _normal, vec3 _fragPos, vec3 _viewDir);
vec4 calcPointLight(PointLight _pointLight, vec3 _normal, vec3 _fragPos, vec3 _viewDir);

void main() {
    vec3 norm = gl_FrontFacing ? normalize(normal) : normalize(vec3(-normal.x, normal.y, -normal.z));
    vec3 viewDir = normalize(viewPos - fragPos);
    vec4 result;

    for(int i = 0; i < MAX_DIR_LIGHTS; i++) {
        if(i >= dirLightAmount) { break; }
        result += calcDirLight(dirLights[i], norm, fragPos, viewDir);
    }
    for(int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if(i >= pointLightAmount) { break; }
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
    }

    result.a = material.color.a;
    fragColor = result;
}

vec4 calcDirLight(DirectionalLight _dirLight, vec3 _normal, vec3 _fragPos, vec3 _viewDir) {
    vec4 ambient = _dirLight.ambientStrength * _dirLight.color * material.color;

    vec3 lightDir = normalize(_dirLight.direction);
    float diff = max(dot(_normal, lightDir), 0.0);
    vec4 diffuse = _dirLight.color * diff * material.color;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = _dirLight.color * _dirLight.specularStrength * spec * material.color;

    return _dirLight.intensity * (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight _pointLight, vec3 _normal, vec3 _fragPos, vec3 _viewDir) {
    vec4 ambient = _pointLight.ambientStrength * _pointLight.color * material.color;

    vec3 lightDir = normalize(_pointLight.position - _fragPos);
    float diff = max(dot(_normal, lightDir), 0.0);
    vec4 diffuse = _pointLight.color * diff * material.color;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = _pointLight.color * _pointLight.specularStrength * spec * material.color;

    float dist = length(_pointLight.position - _fragPos);
    float attenuation = 1.0 / (_pointLight.constant + _pointLight.linear * dist + _pointLight.quadratic * dist * dist);

    if(attenuation < 0.0) { return vec4(1.0, 0.0, 0.0, 1.0); }

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return _pointLight.intensity * (ambient + diffuse + specular);
}
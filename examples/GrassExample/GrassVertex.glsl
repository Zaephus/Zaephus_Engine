#version 330 core

#define PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in mat4 instanceMatrix;

out vec3 fragPos;
out vec4 vertexColor;
out vec3 normal;
out vec2 uv;

uniform float TIME;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float speed;
uniform float range;

void main() {
    float posZ = range * mix(0.0, sin(speed * 0.5f * PI * TIME), aPos.y);
    vec3 pos = vec3(aPos.x, aPos.y, aPos.z + posZ);

    gl_Position = vec4(pos, 1.0) * instanceMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(pos, 1.0) * instanceMatrix);

    mat3 normalMatrix = mat3(instanceMatrix);
    normal = aNormal * normalMatrix;

    vertexColor = aColor;
    uv = aUV;
}
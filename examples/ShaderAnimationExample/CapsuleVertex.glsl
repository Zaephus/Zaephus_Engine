#version 330 core

#define PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out vec3 fragPos;
out vec4 vertexColor;
out vec3 normal;
out vec2 uv;

uniform float TIME;

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float speed;

void main() {
    float posX = 0.2 * mix(0.0, sin(speed * 0.5f * PI * TIME), aPos.y + 1.0);
    float posY = 0.1 * mix(0.0, cos(speed * 0.5f * PI * 2*(TIME + 0.5*PI)), aPos.y + 1.0);
    vec3 pos = vec3(aPos.x + posX, aPos.y + posY, aPos.z);

    gl_Position = vec4(pos, 1.0) * modelMatrix * viewMatrix * projectionMatrix;

    fragPos = aPos;
    normal = vec3(vec4(aNormal, 0.0) * normalMatrix);

    vertexColor = aColor;
    uv = aUV;
}
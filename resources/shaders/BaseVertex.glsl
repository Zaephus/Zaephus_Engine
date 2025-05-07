#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out vec3 fragPos;
out vec4 vertexColor;
out vec3 normal;
out vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = vec4(aPos, 1.0) * modelMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(aPos, 1.0) * modelMatrix);
    normal = vec3(vec4(aNormal, 0.0) * normalMatrix);

    vertexColor = aColor;
    uv = aUV;
}

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in mat4 instanceMatrix;

out vec3 fragPos;
out vec4 vertexColor;
out vec3 normal;
out vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    mat4 localMatrix = instanceMatrix * modelMatrix;

    gl_Position = vec4(aPos, 1.0) * localMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(aPos, 1.0) * localMatrix);

    mat3 normalMatrix = mat3(localMatrix);
    normal = aNormal * normalMatrix;

    vertexColor = aColor;
    uv = aUV;
}
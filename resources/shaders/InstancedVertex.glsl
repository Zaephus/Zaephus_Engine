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

//uniform mat4 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = vec4(aPos, 1.0) * instanceMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(aPos, 1.0) * instanceMatrix);
//    normal = vec3(normalMatrix * vec4(aNormal, 0.0));
    normal = aNormal;

    vertexColor = aColor;
    uv = aUV;
}
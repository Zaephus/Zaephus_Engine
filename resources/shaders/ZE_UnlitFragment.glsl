#version 330 core

out vec4 fragColor;

in vec3 fragPos;
in vec4 vertexColor;
in vec3 normal;
in vec2 uv;

uniform vec4 objectColor;

void main() {
    fragColor = objectColor;
}
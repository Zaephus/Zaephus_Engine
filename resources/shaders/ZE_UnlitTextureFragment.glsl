
#version 330 core

struct Material {
    sampler2D texture;
};
uniform Material material;

out vec4 fragColor;

in vec3 fragPos;
in vec4 vertexColor;
in vec3 normal;
in vec2 uv;

void main() {
    fragColor = texture(material.texture, uv);
}
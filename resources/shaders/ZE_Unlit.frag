
struct Material {
    vec4 color;
};
uniform Material material;

void main() {
    fragColor = material.color;
}
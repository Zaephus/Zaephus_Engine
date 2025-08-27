
struct Material {
    vec4 color;
    float shininess;
};
uniform Material material;

void main() {
    fragColor = calcLighting(material.color, material.color, material.shininess);
}
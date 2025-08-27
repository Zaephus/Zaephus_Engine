
struct Material {
    sampler2D texture;
};
uniform Material material;

void main() {
    fragColor = texture(material.texture, uv);
}
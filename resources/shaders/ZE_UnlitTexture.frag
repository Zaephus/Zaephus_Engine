
struct Material {
    sampler2D texture;
    float uvScale;
};
uniform Material material;

void main() {
    fragColor = texture(material.texture, uv * material.uvScale);
}
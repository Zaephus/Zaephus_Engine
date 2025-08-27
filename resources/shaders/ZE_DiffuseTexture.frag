
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float uvScale;
};
uniform Material material;

void main() {
    fragColor = calcLighting(texture(material.diffuse, uv * material.uvScale), texture(material.specular, uv * material.uvScale), material.shininess);
}
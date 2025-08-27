
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

void main() {
    fragColor = calcLighting(texture(material.diffuse, uv), texture(material.specular, uv), material.shininess);
}
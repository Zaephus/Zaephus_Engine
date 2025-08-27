
uniform float speed;

void main() {
    float posX = 0.2 * mix(0.0, sin(speed * 0.5f * PI * TIME), aPos.y + 1.0);
    float posY = 0.1 * mix(0.0, cos(speed * 0.5f * PI * 2*(TIME + 0.5*PI)), aPos.y + 1.0);
    vec3 pos = vec3(aPos.x + posX, aPos.y + posY, aPos.z);

    gl_Position = vec4(pos, 1.0) * modelMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(pos, 1.0) * modelMatrix);
    normal = vec3(vec4(aNormal, 0.0) * normalMatrix);

    vertexColor = aColor;
    uv = aUV;
}
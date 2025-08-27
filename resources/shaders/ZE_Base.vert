
void main() {
    gl_Position = vec4(aPos, 1.0) * modelMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(aPos, 1.0) * modelMatrix);
    normal = vec3(vec4(aNormal, 0.0) * normalMatrix);

    vertexColor = aColor;
    uv = aUV;
}
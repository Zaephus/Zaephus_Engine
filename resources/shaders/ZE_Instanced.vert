
void main() {
    mat4 localMatrix = instanceMatrix * modelMatrix;

    gl_Position = vec4(aPos, 1.0) * localMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(aPos, 1.0) * localMatrix);

    mat3 normMatrix = mat3(localMatrix);
    normal = aNormal * normMatrix;

    vertexColor = aColor;
    uv = aUV;
}
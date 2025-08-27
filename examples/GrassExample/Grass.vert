
uniform float speed;
uniform float range;

void main() {
    float posZ = range * mix(0.0, sin(speed * 0.5f * PI * TIME), aPos.y);
    vec3 pos = vec3(aPos.x, aPos.y, aPos.z + posZ);

    mat4 localMatrix = instanceMatrix * modelMatrix;

    gl_Position = vec4(pos, 1.0) * localMatrix * viewMatrix * projectionMatrix;

    fragPos = vec3(vec4(pos, 1.0) * localMatrix);

    mat3 normMatrix = mat3(localMatrix);
    normal = aNormal * normMatrix;

    vertexColor = aColor;
    uv = aUV;
}
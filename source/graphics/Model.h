
#pragma once

class Mesh;
class Shader;

struct Model {
    Model(Mesh* _mesh, Shader* _shader);

    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
};
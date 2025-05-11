
target_sources(Zaephus_Engine PRIVATE
    Color.cpp
    Color.h
    Light.cpp
    Light.h
    Mesh.cpp
    Mesh.h
    MeshRenderer.cpp
    MeshRenderer.h
    MultiMeshRenderer.cpp
    MultiMeshRenderer.h
    RenderItem.cpp
    RenderItem.h
    Model.cpp
    Model.h
    Texture2D.cpp
    Texture2D.h
    Vertex.h
)

add_subdirectory(shader)

target_include_directories(Zaephus_Engine PUBLIC
    shader
)
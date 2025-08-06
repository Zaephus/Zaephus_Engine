
target_sources(Zaephus_Engine PRIVATE
    Color.cpp
    Color.h
    Mesh.cpp
    Mesh.h
    MeshRenderer.cpp
    MeshRenderer.h
    MultiMeshRenderer.cpp
    MultiMeshRenderer.h
    RenderObject.cpp
    RenderObject.h
    Model.cpp
    Model.h
    Vertex.h
)

add_subdirectory(shader)
add_subdirectory(lighting)
add_subdirectory(texture)

target_include_directories(Zaephus_Engine PUBLIC
    shader
    lighting
    texture
)
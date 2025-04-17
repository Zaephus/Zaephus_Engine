
message(hello world from source)

add_library(Zaephus_Engine STATIC
    BoundsRenderer.cpp
    Input.cpp
    RayCast3D.cpp
    RayCast3D.h
    Object.cpp
    Object.h
    Random.h
        ModelLoader.cpp
        ModelLoader.h
)

target_sources(Zaephus_Engine PUBLIC
    Bounds.cpp
    Camera.cpp
    Color.cpp
    Component.cpp
    GameObject.cpp
    Scene.cpp
    TimeUtils.cpp
    Transform.cpp
    Window.cpp
    Action.h
    graphics/Light.cpp
    graphics/Mesh.cpp
    graphics/MeshRenderer.cpp
    graphics/Shader.cpp
    graphics/Texture2D.cpp
    graphics/Vertex.h
)
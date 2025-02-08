
add_library(Zaephus_Engine STATIC)

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

add_custom_target(copy_resources VERBATIM)

add_custom_command(
    TARGET copy_resources
    COMMAND ${CMAKE_COMMAND} -E rm -r -f -- ${PROJECT_BINARY_DIR}/source/resources
    COMMENT "Deleting old resources"
    VERBATIM
)

add_custom_command(
    TARGET copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/source/resources ${PROJECT_BINARY_DIR}/source/resources
    COMMENT "Copying resources"
    VERBATIM
)

add_dependencies(Zaephus_Engine copy_resources)
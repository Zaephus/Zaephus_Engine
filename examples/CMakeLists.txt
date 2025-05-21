
link_libraries(Zaephus_Engine)

message(hello world from examples)

add_custom_target(examples ALL)

CopyResources(examples ${CMAKE_SOURCE_DIR}/examples)

add_executable(clickExample
    ClickExample/main.cpp
    ClickExample/ClickExampleScene.h
    ClickExample/ClickableObject.cpp
)
add_dependencies(clickExample examples)

add_executable(lightingExample
    LightingExample/main.cpp
    LightingExample/LightingScene.h
)
add_dependencies(lightingExample examples)

add_executable(transformExample
    TransformExample/main.cpp
    TransformExample/TransformScene.h
)
add_dependencies(transformExample examples)

add_executable(cubesExample
    CubesExample/CubesScene.h
    CubesExample/main.cpp
)
add_dependencies(cubesExample examples)

add_executable(shaderAnimationExample
        ShaderAnimationExample/ShaderAnimScene.h
        ShaderAnimationExample/main.cpp
)
add_dependencies(shaderAnimationExample examples)

add_executable(grassExample
    GrassExample/main.cpp
    GrassExample/GrassScene.h
)

add_dependencies(grassExample examples)
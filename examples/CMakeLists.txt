
link_libraries(Zaephus_Engine)

message(hello world from examples)

add_custom_target(examples ALL)

CopyResources(examples ${CMAKE_SOURCE_DIR}/examples)

add_executable(clickExample
    ClickExample/ClickExampleScene.h
    ClickExample/ClickableObject.cpp
    ClickExample/main.cpp
)
add_dependencies(clickExample examples)

add_executable(lightingExample
    LightingExample/LightingScene.h
    LightingExample/main.cpp
)
add_dependencies(lightingExample examples)

add_executable(transformExample
    TransformExample/TransformScene.h
    TransformExample/main.cpp
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
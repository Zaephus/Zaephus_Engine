
link_libraries(Zaephus_Engine)

add_executable(clickExample
    ClickExample/main.cpp
    ClickExample/ClickExampleScene.h
    ClickExample/ClickableObject.cpp
)

add_executable(lightingExample
    LightingExample/main.cpp
    LightingExample/LightingScene.h
)

add_executable(transformExample
    TransformExample/main.cpp
    TransformExample/TransformScene.h
)

add_executable(cubesExample
    CubesExample/CubesScene.h
    CubesExample/main.cpp
)

add_executable(shaderAnimationExample
        ShaderAnimationExample/ShaderAnimScene.h
        ShaderAnimationExample/main.cpp
)

add_executable(grassExample
    GrassExample/main.cpp
    GrassExample/GrassScene.h
)

add_executable(paperTest
    PaperTest/main.cpp
    PaperTest/TestScene.h
)
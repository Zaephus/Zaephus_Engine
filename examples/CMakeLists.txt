
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
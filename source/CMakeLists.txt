
add_library(Zaephus_Engine STATIC
        ZEngine.h
        ZEngineFwd.h
)

if(ENABLE_PROFILING)
    target_compile_definitions(Zaephus_Engine PUBLIC ENABLE_PROFILING)
endif()

add_subdirectory(graphics)
add_subdirectory(helper)
add_subdirectory(objects)
add_subdirectory(physics)
add_subdirectory(renderer)
add_subdirectory(scene)

target_include_directories(Zaephus_Engine PUBLIC
    graphics
    helper
    objects
    physics
    renderer
    scene
)